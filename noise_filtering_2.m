%% Real Noise Filtering 
% Project ATLAS
% Data taken from liDAR system looking at wall and corner
% Code created: 10 Nov 2020
% Code updated: 30 Nov 2020
% -------------------------------------------------------------------------

tic
format long g
close all
% Format Figures 
    Screen = get(0,'ScreenSize');
    ScnW = Screen(3);
    ScnH = Screen(4);
    toolbar_height = 77;
    window_border  = 5;
    top_left = [window_border, (ScnH/2)+toolbar_height-2, (1/2)*ScnW - 2*window_border, (ScnH/2)-2*toolbar_height-4*window_border];
    top_center = top_left + [ScnW/4, 0, 0, 0];
    top_right = top_center + [ScnW/4, 0, 0, 0];
    bottom_left = [window_border, window_border+2*toolbar_height-2, (1/2)*ScnW - 2*window_border, (ScnH/2)-2*toolbar_height-4*window_border];
    bottom_center = bottom_left + [ScnW/4, 0, 0, 0];
    bottom_right = bottom_center + [ScnW/4, 0, 0, 0];
    full_screen = [window_border, 0, ScnW - 2*window_border, (ScnH)-2*toolbar_height-4*window_border];
% Input data
    data = importdata('LidarData.txt');
             x = rmmissing(data(:,1))./1000;
             y = rmmissing(data(:,2))./1000;
             y(1055) = 3;
             z = rmmissing(data(:,3))./1000;
             pre_arith_mean = sum(z)/length(z);
% Plot initial data
    figure (1)
        plot3(x,y,z,'k.');
        axis equal
        grid minor
        hold on
        plot3(0,0,0,'r*')
        xlabel('x (m)')
        ylabel('y (m)')
        zlabel('z (m)')
        set(gcf, 'Position', top_left)
% fig = figure(2);
% plot3(x,y,z,'w.')
% set(gcf, 'Position', full_screen)
% v = VideoWriter('LiDAR_collection_animation.avi');
% v.FrameRate = 140;
% open(v);
%      for q = 1:length(x)
%          plot3(x(q),y(q),z(q),'r.','MarkerSize',14)
%          axis([0, 10, 0, 3, 0, 4.5])
%          grid on
%          hold on
%     drawnow
%     writeVideo(v, getframe(fig))
%      end 
%   close(v);
 %% FWCFM
tic
for i = 1:10
        if i == 1
            fun = z;
            xbar = pre_arith_mean;
            Cv = 1;
            Cs = 1;
        else 
            fun = FW_m;
            xbar = sum(fun)/length(fun);
            Cv = Cv;
            Cs = Cs + 15;
        end 
    FW_m = FWOCFM(fun,Cv,Cs,xbar);
end 
%% 3DLS
filtered = zeros(size([x,y,z]));
for dim = [1,2,3]
    if dim == 1
        fun = x;
    elseif dim == 2
        fun = y;
    elseif dim ==3
        fun = FW_m;
    end 
    for k = 1:10
        if k == 1
            search_surface = zeros(size(z));
        else 
            search_surface = filtered(:,dim);
        end 
    filtered(:,dim) = THDLS(fun,search_surface)';
%         ind = find(max(filtered(:,2))==filtered(:,2));
%         filtered(ind,2) = mean([filtered(ind,2), mean(filtered,2)]);
%         filt_x = filtered(:,dim);
%         scaling_x = max(fun)/max(filt_x);
%         filtered(:,dim) = scaling_x*filt_x;
    end 
end
% post-process filtered data
    filt_x = filtered(:,1);
        scaling_x = max(x)/max(filt_x);
        filt_x_n = scaling_x*filt_x;
    filt_y = filtered(:,2);
        scaling_y = max(y)/max(filt_y);
        filt_y_n = scaling_y*filt_y;
        ind = find(max(filt_y_n) == filt_y_n);
        filt_y_n(ind) = mean(filt_y_n);
    filt_z = filtered(:,3);
        scaling_z = max(z)/max(filt_z);
        filt_z_n = scaling_z*filt_z;
    err_x = abs(filt_x_n - x);
    err_y = abs(filt_y_n - y);
    err_z = abs(filt_z_n - z);
% Plot filtered data
    figure (3)
        stem3(x,y,filt_z_n,'b.');
        axis equal
        grid minor
        hold on
        plot3(filt_x_n, filt_y_n, filt_z_n, 'r.')
        plot3(x,y,z,'k.')
        xlabel('x (m)')
        ylabel('y (m)')
        zlabel('z (m)')
        title ('Filtered Terrain Data with Original Data Overlay')
        legend('Data Filtered -- z coordinate only', 'Data Filtered -- all XYZ coordinates', 'Original Data')
        %          plot3(filt_x_n-err_x,filt_y_n-err_y,filt_z_n-err_z,'r+')
%          plot3(filt_x_n+err_x,filt_y_n+err_y,filt_z_n+err_z,'r+')
        set(gcf, 'Position', top_right)
        
% filt = [filt_x_n, filt_y_n, filt_z_n];
% filt = sortrows(filt, [1 2]);
% xv = linspace(min(filt(:,1)), max(filt(:,1)), length(filt));
% yv = linspace(min(filt(:,2)), max(filt(:,2)), length(filt));
% [X,Y] = meshgrid(xv, yv);
% Z = griddata(x,y,filt(:,3),X,Y);
% figure (3)
% mesh(X,Y,Z)
% 
% logic4 = double(([x,y,filt_z_n] - [x,y,z]) >= 0.001);
% filt_noise_filt_1 = (1 - norm(logic3)/(length(logic3))) *100;
% fprintf('NOISE FILTERED BY FIRST ANALYSIS: %5.3f %% \n',filt_noise_filt_1)
% logic3 = double((filt - [x,y,z]) >= 0.001);
% filt_noise_filt = (1 - norm(logic3)/(length(logic3))) *100;
% fprintf('NOISE FILTERED BY SECOND ANALYSIS: %5.3f %% \n',filt_noise_filt)
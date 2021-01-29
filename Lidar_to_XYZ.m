clear
close

data = readmatrix("scan3.txt");


steps = 200*8;
degperstepYAW = 360/steps;
figure
scatter3(0,0,0,'blue')
xlabel("X")
ylabel("Y")
zlabel("Z")
hold on
xlim([-200,400])
ylim([-300,300])
zlim([-100,100])
%axis equal
for i = 1:length(data)
    yaw(i) = degperstepYAW*data(i,1); %yaw and pitch positions IN DEGREES
    pitch(i) = -1.8*data(i,2)+90;
    dist(i) = data(i,3);
    
    x(i) = dist(i)*cosd(yaw(i))*sind(pitch(i)); %X
    y(i) = dist(i)*sind(yaw(i))*sind(pitch(i)); %Y
    z(i) = dist(i)*cosd(pitch(i)); %Z
    %scatter3(x(i),y(i),z(i),'red','Filled');
    
end

scatter3(x,y,z,'red','Filled');



%% Write XYZ coordinates to file
% id = fopen('xyzscan3.txt','w'); %Specify file name
% for i = 1:length(data)
%     fprintf(id,'%.3f,%.3f,%.3f\n',x(i),y(i),z(i));
% end


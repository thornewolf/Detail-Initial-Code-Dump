% function val = THDLS(fun,search_surface)
%     a = linspace(-100,100,length(fun));
%      b = linspace(-100,100,length(fun));
%     dif = zeros(size(a));
%     for i = 1:length(a)
%         dif(i) = ((a(i))).*((fun(i)-search_surface(i)).^2);
%         %dif(i) = (((a(i).*fun(i)))- search_surface(i)).^2;
%     end 
%     index = find(dif == min(dif));
%     a_val = a(index); %#ok<FNDSB>
%     b_val = b(index);
%     val = -(a_val).*(fun);
% end 
function val = THDLS(fun,search_surface)
    a = linspace(-100,100,length(fun));
    b = linspace(-100,100,length(fun));
    dif = zeros(size(a));
    for i = 1:length(a)
        dif(i) = (a(i)+b(i)^2).*((fun(i)-search_surface(i)).^2);
    end 
    index = find(dif == min(dif));
    a_val = a(index);
    b_val = b(index);
    val = -(a_val(1)+b_val(1)^2).*(fun);
%     for j = 1:length(a)
%         if j > 1 && abs(val(j) - val(j-1)) >= 3
%             val(j) = val(j-1);
%         else 
%             val(j) = val(j);
%         end 
%     end 
%     ind = find(max(val) == val);
%     val(ind) = mean(val);
% end
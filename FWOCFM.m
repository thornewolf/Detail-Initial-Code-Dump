function FW_m = FWOCFM(fun,Cv,Cs,xbar)
syms t
        a0 = xbar*(1-((2*Cv)/Cs));
        alpha = 4/(Cs^2);
        beta = 2/(xbar*Cv*Cs);
        P = (abs(beta)/gamma(alpha)).*(beta*(fun'-a0).^(alpha-1)).*exp(-beta.*(fun'-a0));
        Phi = (Cs/2)*gaminv(1-P,alpha,1) - (2/Cs);
        Phi(isinf(Phi)|isnan(Phi)) = 0;
        x_pm = xbar*(1+Cv.*(Phi));
        sig_m = std(x_pm);
        X = (fun'-x_pm)./(sqrt(2)*sig_m);
        S = std(X);
        n = length(X);
        B = S/(sqrt(n)*sig_m);
            %preallocation of numbers
                a = zeros(size(fun));
                b = zeros(size(fun));
                FW_m = zeros(size(fun));
                    for j = 1:length(fun)
                        a(j) = (fun(j) + B - x_pm(j))./(sqrt(2)*sig_m);
                        b(j) = (fun(j) - B - x_pm(j))./(sqrt(2)*sig_m);
                        FW_m(j) = fun(j) - 2/(sqrt(pi))*1*integral(@(X) exp(-X.^2),a(j),b(j),'ArrayValued',1);
                    end 
end 
% ORIGINAL:
% syms t 
% for i = 1:1
%     % coeficient definition
%     if i == 1
%         Cv(i)  = 0.5;
%         Cs(i)  = 0.6;
%         xbar(i) = pre_arith_mean;
%     else 
%         Cv(i) = Cv(i-1) - 0.05;
%         Cs(i) = Cs(i-1) - 0.07;
%         xbar(i) = pre_arith_mean - 0.01;
%     end 
%     % anticipated frequency 
%         a0(i) = xbar(i)*(1-((2*Cv(i))/Cs(i)));
%         alpha(i) = 4/(Cs(i)^2);
%         beta(i) = 2/(xbar(i)*Cv(i)*Cs(i));
%         P(i,:) = (abs(beta(i))/gamma(alpha(i))).*(beta(i)*(z_noise'-a0(i)).^(alpha(i)-1)).*exp(-beta(i).*(z_noise'-a0(i)));
%         Phi(i,:) = (Cs(i)/2)*gaminv(1-P(i,:),alpha(i),1) - (2/Cs(i));
%         Phi(isinf(Phi(i,:)|isnan(Phi(i,:)))) = 0;
%         x_pm(i,:) = xbar(i)*(1+Cv(i).*(Phi(i,:)));
%         sig_m(i) = std(x_pm(i,:));
%         X(i,:) = (z_noise'-x_pm(i))./(sqrt(2)*sig_m(i));
%         S(i) = std(X(i));
%         n = length(X(i));
%         B(i) = S(i)/(sqrt(n)*sig_m(i));
%         for j = 1:length(z)
%             a(i,j) = (z_noise(i,j)' + B - x_pm(i,j))./(sqrt(2)*sig_m(i));
%             b(i,j) = (z_noise(i,j)' - B - x_pm(i,j))./(sqrt(2)*sig_m(i));
%             FW_m(i,j) = z_noise(j) - 2/(sqrt(pi))*100*integral(@(X) exp(-X(i,:).^2),a(i,j),b(i,j),'ArrayValued',1);
%         end 
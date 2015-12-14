i=importdata('Results/v1_5/Critical/debondt_v1-5_runtimepop.dat');

x = i(:,1);
y = i(:,5);
[maxval, maxidx] = max(y);

p = plot(x,y);
hold on;

% Not really extensible
linearIdx = 6;
linearfit = polyfit(x(1:linearIdx),y(1:linearIdx),1)
logfit = polyfit(log(x(1:maxidx)),y(1:maxidx),1)

%[linearfit,S1,mu1] = polyfit(x(1:linearIdx),y(1:linearIdx),1)
%[logfit,S,mu] = polyfit(log(x(1:maxidx)),y(1:maxidx),1)
%[y,delta] = polyval(linearfit,x(1:maxidx),S,mu);

plot(x(1:2*linearIdx), linearfit(1)*x(1:2*linearIdx) + linearfit(2));
plot(x, logfit(1)*log(x) + logfit(2));


title('Fitting Equations to Speedup');
xlabel('# of Threads') 
ylabel('Speedup') 


legend('Initial Data', 'Linear Fit', 'Logarithmic Fit');



%a = linspace(0, 40, 36);
%b = linspace(426, 426, 36);
%plot(a,b,'k-','linewidth',1.5,'color','red')
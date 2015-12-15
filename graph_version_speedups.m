i=importdata('Results/v1_1/eil_51/debondt_v1-1_speedup.dat'); 
i1=importdata('Results/v1_4/debondt_v1-4_speedup.dat'); 
i2=importdata('Results/v1_5/Critical/debondt_v1-5_criticalspeedup_3.dat'); 


x = i(:,1);
y = i(:,5);
y1 = i1(:,5);
y2 = i2(:,5);

p = plot(x,y,x,y1,x,y2);

hold on;

%title('Graph of Speedup for Lenovo');
title('Debondt Speedups for Different Versions');
xlabel('# of Threads') 
ylabel('Speedup') 

legend('v1.1', 'v1.4', 'v1.5 Critical');



a = linspace(0, 40, 36);
b = linspace(1, 1, 36);
plot(a,b,'k-','linewidth',1.5,'color','red')
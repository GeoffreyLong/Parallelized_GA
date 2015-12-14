i=importdata('Results/v1_5/Critical/debondt_v1-5_criticalspeedup_3.dat'); 
i1=importdata('Results/v1_5/Critical/debondt_v1-5_neighborswap.dat'); 
i2=importdata('Results/v1_5/Critical/debondt_v1-5_scramble.dat'); 
i3=importdata('Results/v1_5/Critical/debondt_v1-5_inversion.dat'); 



x = i(:,1);
y = i(:,6);
y1 = i1(:,6);
y2 = i2(:,6);
y3 = i3(:,6);

p = plot(x,y,x,y1,x,y2,x,y3);

hold on;

%title('Graph of Speedup for Lenovo');
title('Debondt Speedups over Different Mutations');
xlabel('# of Threads') 
ylabel('Runtime') 


legend('Swap', 'Neighbor Swap', 'Scramble', 'Inversion');



%a = linspace(0, 40, 36);
%b = linspace(426, 426, 36);
%plot(a,b,'k-','linewidth',1.5,'color','red')
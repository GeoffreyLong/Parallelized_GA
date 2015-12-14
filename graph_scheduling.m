i=importdata('Results/v1_5/Critical/debondt_v1-5_criticalspeedup_3.dat'); 
i1=importdata('Results/v1_5/Critical/debondt_v1-5_staticchunk1.dat'); 
i2=importdata('Results/v1_5/Critical/debondt_v1-5_runtimepop.dat'); 
i3=importdata('Results/v1_5/Critical/debondt_v1-5_dynamicpop.dat'); 
i4=importdata('Results/v1_5/Critical/debondt_v1-5_guidedpop.dat'); 



x = i(:,1);
y = i(:,5);
y1 = i1(:,5);
y2 = i2(:,5);
y3 = i3(:,5);
y4 = i4(:,5);

p = plot(x,y,x,y1,x,y2,x,y3,x,y4);

hold on;

%title('Graph of Speedup for Lenovo');
title('Debondt Speedups over Different Schedulings');
xlabel('# of Threads') 
ylabel('Speedup') 

p(1).Marker = '*';
p(3).Marker = 'o';
p(5).Marker = '>';

legend('Default', 'Static (Chunk=1)', 'Runtime', 'Dynamic', 'Guided');



%a = linspace(0, 40, 36);
%b = linspace(1, 1, 36);
%plot(a,b,'k-','linewidth',1.5,'color','red')
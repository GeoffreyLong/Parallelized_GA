%i=importdata('Results/eil_51/lenovo_v1-1_speedup.dat'); 
i = {
    importdata('Results/eil_51/debondt_v1-1_speedup.dat')
    importdata('Results/eil_101/debondt_v1-1_speedup.dat')
    importdata('Results/pr_76/debondt_v1-1_speedup.dat')
    importdata('Results/pr_107/debondt_v1-1_speedup.dat')
%    importdata('Results/pr_124/debondt_v1-1_speedup.dat') 
%    importdata('Results/pr_152/debondt_v1-1_speedup.dat')
    importdata('Results/pr_299/debondt_v1-1_speedup.dat')
    importdata('Results/pr_1007/debondt_v1-1_speedup.dat'); 
    importdata('Results/pr_2392/debondt_v1-1_speedup.dat'); 
    importdata('Results/a_280/debondt_v1-1_speedup.dat')
    importdata('Results/burma_14/debondt_v1-1_speedup.dat')
};

markers = ['*', '*', '.', '.', '.', '.', '.', '>', 'd', '.'];


for j = 1:size(i)
    table = i{j}
    x = table(:,1); 
    y = table(:,5); 
    
    p = plot(x,y);
    
    p(1).Marker = markers(j);    
    hold on;
end


%title('Graph of Speedup for Lenovo');
title('Debondt Speedups over Several Datasets');
xlabel('# of Threads') 
ylabel('Speedup') 

legend('EIL51', 'EIL101', 'PR76', 'PR107', 'PR299', 'PR1007', 'PR2392', 'A280', 'BURMA14');



a = linspace(0, 40, 36);
b = linspace(1, 1, 36);
plot(a,b,'k-','linewidth',1.5,'color','red')
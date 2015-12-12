i=importdata('Results/eil_51/lenovo_v1-1_speedup.dat'); 
%i=importdata('Results/eil_51/debondt_v1-1_speedup.dat'); 

x = i(:,1);
y = i(:,5); 

p = plot(x,y);
p(1).Marker = '*';
p(1).Color = 'blue';

title('Graph of Speedup for Lenovo on EIL51 Dataset');
%title('Graph of Speedup for Debondt on EIL51 Dataset');
xlabel('# of Threads') 
ylabel('Speedup') 
hold on;

a = linspace(0, 40, 36);
b = linspace(1, 1, 36);
plot(a,b,'k-','linewidth',1.5,'color','red')
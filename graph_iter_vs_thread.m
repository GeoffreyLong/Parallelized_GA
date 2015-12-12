%i=importdata('Results/eil_51/lenovo_v1-1_itervsthread.dat'); 
i=importdata('Results/eil_51/debondt_v1-1_itervsthread.dat'); 

x = i(:,1);
y = i(:,3);
z = i(:,5);

% scatter3(x,y,z);



[xi,yi] = meshgrid(0:5:160, 0:100:1600);
zi = griddata(x,y,z,xi,yi);
surf(xi,yi,zi);
xlabel('Number of Threads')
ylabel('Number of Iterations')
zlabel('Speedup')

view([45 45]);

title('Debondt Speedup over varied Iterations and Thread Counts');
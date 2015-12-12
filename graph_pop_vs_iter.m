i=importdata('Results/eil_51/lenovo_v1-1_4thread.dat'); 



i = i(i(:,1)==4,:); % Select only sequential data points

x = i(:,2);
y = i(:,3);
z = i(:,5);

% scatter3(x,y,z);



[xi,yi] = meshgrid(0:50:1000, 0:1000:25000);
zi = griddata(x,y,z,xi,yi);
surf(xi,yi,zi);
xlabel('Population Size')
ylabel('Number of Iterations')
zlabel('Speedup')

view([15 45]);

title('Speedup for Lenovo 4 Threads');
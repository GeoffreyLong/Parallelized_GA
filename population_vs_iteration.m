i1=importdata('Results/lenovo_v1-1_iter_1thread.dat'); 
i2=importdata('Results/lenovo_v1-1_iter_2thread.dat'); 
p1=importdata('Results/lenovo_v1-1_pop_1thread.dat'); 
p2=importdata('Results/lenovo_v1-1_pop_2thread.dat'); 



xi = i1(:,2);
yi = i1(:,4); 
yi2 = i2(:,4);

xp = p1(:,1);
yp = p1(:,4);
yp2 = p2(:,4);

p = plot(xi,yi,xi,yi2,xp,yp,xp,yp2);
p(1).Marker = '*';
p(1).Color = 'blue';
p(2).Marker = '*';
p(2).Color = 'blue';

p(3).Marker = 'o';
p(3).Color = 'red';
p(4).Marker = 'o';
p(4).Color = 'red';

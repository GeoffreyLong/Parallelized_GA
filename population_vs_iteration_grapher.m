% Can use either data set
%i=importdata('Results/lenovo_v1-1.dat'); 
i=importdata('Results/lenovo_v1-1_2.dat'); 

% Change the parameters to compare different data points
numThreads = 4;
population = 250;
iteration = 250;

it1 = i(i(:,1)==1,:); % Select only sequential data points
it2 = i(i(:,1)==numThreads,:); % Select only X threaded data points
p1 = it1(it1(:,2)==population,:); % Select only population of 1000
p2 = it2(it2(:,2)==population,:); % Select only population of 1000
i1 = it1(it1(:,3)==iteration,:); % Select only iterations of 1000
i2 = it2(it2(:,3)==iteration,:); % Select only iterations of 1000

x = i1(:,2);
yi = i1(:,5); 
yi2 = i2(:,5);

yp = p1(:,5);
yp2 = p2(:,5);

p = plot(x,yi,x,yi2,x,yp,x,yp2);
p(1).Marker = '*';
p(1).Color = 'blue';
p(2).Marker = '*';
p(2).Color = 'blue';

p(3).Marker = 'o';
p(3).Color = 'red';
p(4).Marker = 'o';
p(4).Color = 'red';

legend([p(1), p(3)], 'iteration', 'population', 'Location', 'northwest');
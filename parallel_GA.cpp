#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <math.h> 
#include <limits>
#include <sys/time.h>
#include <ctime>
#include <omp.h>
#include <algorithm>

using namespace std;
typedef unsigned long long timestamp_t;
int numT;

// Method for the timing
static timestamp_t get_timestamp () {
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

class City {
  public:
    // nodeNum-1 to change tour index to index from 0
    City(int nodeNum, double x, double y) : nodeNum(nodeNum-1), x(x), y(y) {};
    int getNodeNum() {return nodeNum;}
    double getX() {return x;}
    double getY() {return y;}
  private:
    int nodeNum;
    double x;
    double y;
};

void print_tour(vector<int> tour){
  for (int i = 0; i < tour.size(); i++){
    cout << tour[i] << " "; 
  }
  cout << endl;
}

vector<int> mutate_swap(vector<int> tour){
  // Swap the cities at two randomly chosen indices
  int indexOne = rand() % tour.size();
  int indexTwo = rand() % tour.size();
  

  int temp = tour[indexOne];
  tour[indexOne] = tour[indexTwo];
  tour[indexTwo] = temp;

  return tour;
}

vector<int> mutate_swapNeighbors(vector<int> tour){
  // Swap adjacent cities in the tour
  int indexOne = rand() % tour.size();
  int indexTwo = (indexOne+1) % tour.size();
  
  int temp = tour[indexOne];
  tour[indexOne] = tour[indexTwo];
  tour[indexTwo] = temp;

  return tour;
}

vector<int> mutate_scramble(vector<int> tour){
  // Scramble the elements between two randomly chosen indices
  int indexOne = rand() % tour.size();
  int indexTwo = rand() % tour.size();
  
  if (indexOne > indexTwo){
    int temp = indexOne;
    indexOne = indexTwo;
    indexTwo = temp;
  }

  int intervalSize = indexTwo - indexOne;
  for (int i = indexOne; i < indexTwo; i++){
    int index = rand() % intervalSize + indexOne;
    int temp = tour[i];
    tour[i] = tour[index];
    tour[index] = temp;
  }

  return tour;
}

vector<int> mutate_inversion(vector<int> tour){
  // Reverse the elements between two randomly chosen indices
  int indexOne = rand() % tour.size();
  int indexTwo = rand() % tour.size();
  
  if (indexOne > indexTwo){
    int temp = indexOne;
    indexOne = indexTwo;
    indexTwo = temp;
  }


  int intervalSize = indexTwo - indexOne;
  for (int i = indexOne; i < indexTwo; i++){
    int temp = tour[i];
    tour[i] = tour[indexTwo];
    tour[indexTwo] = temp;
    indexTwo --;
  }

  return tour;
}

double calculate_fitness(vector<int> tour, vector<City> cities){
  // Iterate through the tour and find euclidean distance between points
  // Sum these up to get the overall tour length
  double fitness = 0.0;
  for (int i = 1; i < tour.size(); i++){
    City startCity = cities[tour[i-1]];
    City endCity = cities[tour[i]];
    fitness += sqrt(pow(endCity.getX()-startCity.getX(),2.0) 
        + pow(endCity.getY() - startCity.getY(),2.0));
    startCity = endCity;
  }

  City startCity = cities[tour[0]];
  City endCity = cities[tour[tour.size()-1]];
  fitness += sqrt(pow(endCity.getX() - startCity.getX(),2.0) 
        + pow(endCity.getY() - startCity.getY(),2.0));


  return fitness;
}

vector<City> initialize_cities(){
  // Use knowledge of the dataset size...
  //ifstream inFile("DataSets/burma14.tsp"); int tourSize = 14;
  ifstream inFile("DataSets/eil51.tsp"); int tourSize = 51;
  //ifstream inFile("DataSets/eil101.tsp"); int tourSize = 101;
  //ifstream inFile("DataSets/a280.tsp"); int tourSize = 280;
  //ifstream inFile("DataSets/fl417.tsp"); int tourSize = 417;
  //ifstream inFile("DataSets/pr76.tsp"); int tourSize = 76;
  //ifstream inFile("DataSets/pr107.tsp"); int tourSize = 107;
  //ifstream inFile("DataSets/pr299.tsp"); int tourSize = 299;
  //ifstream inFile("DataSets/pr1002.tsp"); int tourSize = 1002;
  //iifstream inFile("DataSets/pr2392.tsp"); int tourSize = 2392;
  
  // Reserve the number of cities
  vector<City> cities;
  cities.reserve(tourSize);

  if (!inFile) {
    cerr << "Input file not found." << endl;
    return cities;
  }


  string line;
  while (std::getline(inFile, line)){
    if (line.empty()) continue;
  
    istringstream iss(line);
    int nodeNum;
    double x, y;


    // Not entirely sure that this will work for all files...
    // If the input stream does not have three args it is not a tour location
    // All of the files seem to be formatted as nodeNum xPos yPos though
    if (!(iss >> nodeNum >> x >> y)) { continue; }


    cities.push_back(City(nodeNum, x, y));
  }

  return cities;
}

vector< vector< int > > initialize_population(vector<City> cities, int populationSize){
  vector< vector< int > > population;
  population.reserve(populationSize);

  int tourSize = cities.size();

  // NOTE1: Please note that this requires openmp 4.0, which requires gcc 4.9+
  // Alternatively can compile by removing the reduction on the for loop below 
  // And by uncommenting the line at the next NOTE1
#pragma omp declare reduction (merge : std::vector< vector<int> > : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

#pragma omp parallel num_threads(numT)
{
#pragma omp parallel for reduction(merge: population)
  for (int i = 0; i < populationSize; i++){
    vector<int> tour;
    tour.reserve(tourSize);

    // Set the arrays
    // I haven't found a way to do this dynamically yet
    for (int j = 0; j < tourSize; j++){
      tour.push_back(j);
    }

    // Scramble the values of the arrays
    for (int j = 0; j < tourSize; j++){
      int index = rand() % tourSize;
      int temp = tour[j];
      tour[j] = tour[index];
      tour[index] = temp;
    }

// #pragma omp critical // NOTE1: uncomment if issues as per above NOTE1
    population.push_back(tour);
  }
}

  return population;
}

int main(){
  vector<City> cities = initialize_cities();
  int tourSize = cities.size();

  // Initialize the cities from the data file
  if (tourSize == 0){
    return -1;
  }

  // Set the parameters
  int numT = 18;
  int populationSize = 500;
  int maxNumIterations = 10000;

  // TODO replace with omp_get_wtime()?
  double t0 = get_timestamp();
  srand(time(0));


  // Initialize population randomly
  vector< vector< int > > population = initialize_population(cities, populationSize);

  // Run the algorithm for a specific number of iterations
#pragma omp parallel num_threads(numT)
{
  for (int nIteration = 0; nIteration < maxNumIterations; nIteration++){
#pragma omp for schedule(runtime)
    for (int i = 0; i < populationSize; i++){
      vector<int> tour = population[i];

      // Mutation Operators
      //vector<int> newTour = mutate_swap(tour);
      //vector<int> newTour = mutate_swapNeighbors(tour);
      //vector<int> newTour = mutate_scramble(tour);
      vector<int> newTour = mutate_inversion(tour);

      // Calculate the fitness of both tours
      double fitnessOne = calculate_fitness(tour, cities);
      double fitnessTwo = calculate_fitness(newTour, cities);

      // Take the better performing individual
      if (fitnessTwo < fitnessOne){
        population[i] = newTour;
      }
    }
  }
}

  // Get the fitness of the best performing individual
  double bestFitness = std::numeric_limits<double>::max();
#pragma omp parallel num_threads(numT)
{
#pragma omp parallel for reduction(min : bestFitness)
  for (int i = 0; i < populationSize; i++){
    vector<int> tour = population[i];
    double curFitness = calculate_fitness(tour, cities);

    if (curFitness < bestFitness){
      bestFitness = curFitness;
    }
  }
}


  // Get the runtime 
  timestamp_t t1 = get_timestamp();
  double execTime = (t1 - t0) / 1000000.0L;

  // Print the results and parameters
  cout << "Threads=" << numT << " PopulationSize=" << populationSize << " maxNumIterations="
    << maxNumIterations << " Fitness=" << bestFitness << " Runtime=" << execTime << endl;

  return 0;

}

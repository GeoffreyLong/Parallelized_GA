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

using namespace std;
typedef unsigned long long timestamp_t;


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

vector<City> cities;

vector<int> mutate_swap(vector<int> tour){
  int indexOne = rand() % tour.size();
  int indexTwo = rand() % tour.size();
  

  int temp = tour[indexOne];
  tour[indexOne] = tour[indexTwo];
  tour[indexTwo] = temp;

  return tour;
}

vector<int> mutate_swapNeighbors(vector<int> tour){
  int indexOne = rand() % tour.size();
  int indexTwo = (indexOne+1) % tour.size();
  
  int temp = tour[indexOne];
  tour[indexOne] = tour[indexTwo];
  tour[indexTwo] = temp;

  return tour;
}


double calculate_fitness(vector<int> tour){
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

bool initialize_cities(){
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
  
  

  if (!inFile) {
    cerr << "Input file not found." << endl;
    return false;
  }


  string line;
  while (std::getline(inFile, line)){
    if (line.empty()) continue;
  
    istringstream iss(line);
    int nodeNum;
    double x, y;


    // Not entirely sure that this will work for all files...
    // If the input stream does not have three args it is not a tour location
    if (!(iss >> nodeNum >> x >> y)) { continue; }


    cities.push_back(City(nodeNum, x, y));

    // Check output
    //cout << nodeNum << " " << x << " " << y << endl;
  }

  return true;
}

vector< vector< int > > initialize_population(int populationSize){
  vector< vector< int > > population;
  int tourSize = cities.size();

  for (int i = 0; i < populationSize; i++){
    vector<int> tour;
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

    population.push_back(tour);
  }

  return population;
}

int main(){
  // Initialize the cities from the data file
  if (!initialize_cities()){
    return -1;
  }
  int tourSize = cities.size();

  // Some bookkeeping and loopers for testing
  int nTimes = 3;
  int sizes[10] = {25,50,100,250,500,1000,2500,5000,10000,25000};
  int threads[36] = {1,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
    21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};
  double sequentialTime = 0;


  for (int pop = 0; pop < 1; pop ++){
    for (int iter = 0; iter < 1; iter ++){
      for (int num_threads = 0; num_threads < 2; num_threads ++){
        int numT = threads[num_threads];
        //int populationSize = sizes[pop];
        //int maxNumIterations = sizes[iter];

        int populationSize = 100;
        int maxNumIterations = 1000;

        double overallFitness = 0;
        double overallTime = 0;

        for (int x = 0; x < nTimes; x ++){
          timestamp_t t0 = get_timestamp();
          srand(time(0));


          // Initialize population randomly
          vector< vector< int > > population = initialize_population(populationSize);

          // Run the algorithm for a specific number of iterations
#pragma omp parallel num_threads(numT)
{
          for (int nIteration = 0; nIteration < maxNumIterations; nIteration++){
#pragma omp for
            for (int i = 0; i < populationSize; i++){
              vector<int> tour = population[i];

              // Mutation Operators
              //vector<int> newTour = mutate_swap(tour);
              vector<int> newTour = mutate_swapNeighbors(tour);

              // Calculate the fitness of both tours
              double fitnessOne = calculate_fitness(tour);
              double fitnessTwo = calculate_fitness(newTour);

              // Take the better performing individual
              if (fitnessTwo < fitnessOne){
                population[i] = newTour;
              }
            }
          }
}

          double bestFitness = std::numeric_limits<double>::max();
          for (int i = 0; i < populationSize; i++){
            vector<int> tour = population[i];
            double curFitness = calculate_fitness(tour);

            if (curFitness < bestFitness){
              bestFitness = curFitness;
            }
          }


          timestamp_t t1 = get_timestamp();
          double execTime = (t1 - t0) / 1000000.0L;

          overallFitness += bestFitness;
          overallTime += execTime;
        } 

        double time = overallTime / nTimes;
        double fitness = overallFitness / nTimes;

        if (numT == 1){
          sequentialTime = (double) time;
        }

        cout << "Threads=" << numT << " PopulationSize=" << populationSize << " maxNumIterations="
          << maxNumIterations << " Fitness=" << fitness << " Speedup=" << (double) sequentialTime / (double) time << endl;
      }
    }
  }

  return 0;

}

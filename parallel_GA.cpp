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

double calculate_fitness(vector<int> tour){
  double fitness = 0.0;
  for (int i = 1; i < tour.size(); i++){
    City startCity = cities[tour[i-1]];
    City endCity = cities[tour[i]];
    fitness += sqrt(pow(endCity.getX()-startCity.getX(),2.0) 
        + pow(endCity.getY() - startCity.getY(),2.0));
  }

  fitness += sqrt(pow(cities[tour[tour.size()-1]].getX() - cities[tour[0]].getX(),2.0) 
        + pow(cities[tour[tour.size()-1]].getY() - cities[tour[0]].getY(),2.0));


  return fitness;
}

int main(){
  int nTimes = 10;
  int sizes[5] = {100,200,400,800,1600};
  int threads[12] = {1,2,3,4,6,8,12,16,24,32,64,128};

  double sequentialTime = 0;
  for (int pop = 0; pop < 5; pop ++){
    for (int iter = 0; iter < 1; iter ++){
      for (int num_threads = 0; num_threads < 12; num_threads ++){
        int numT = threads[num_threads];
        
        int populationSize = sizes[pop];
        int maxNumIterations = 1000;

        double overallFitness = 0;
        double overallTime = 0;

        for (int x = 0; x < nTimes; x ++){
          timestamp_t t0 = get_timestamp();

          srand(time(0));
          // Use knowledge of the dataset size...
          ifstream inFile("DataSets/eil51.tsp"); int tourSize = 51;
          
          if (!inFile) {
            cerr << "Input file not found." << endl;
            return -1;
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

          // Initialize population randomly
          vector< vector< int > > population;
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

          // Run the algorithm for a specific number of iterations
          int nIteration = 0;
          while (nIteration < maxNumIterations){
#pragma omp parallel num_threads(numT)
{
#pragma omp for
            for (int i = 0; i < populationSize; i++){
              vector<int> tour = population[i];
              vector<int> newTour = mutate_swap(tour);

              // Calculate the fitness of both tours
              double fitnessOne = calculate_fitness(tour);
              double fitnessTwo = calculate_fitness(newTour);

              // Take the better performing individual
              if (fitnessTwo < fitnessOne){
                population[i] = newTour;
              }
            }
}

            nIteration ++;
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

        cout << sequentialTime << " " << time << endl;

        cout << "Threads=" << numT << " PopulationSize=" << populationSize << " maxNumIterations="
          << maxNumIterations << " Fitness=" << fitness << " Speedup=" << (double) sequentialTime / (double) time << endl;
      }
    }
  }

  return 0;

}

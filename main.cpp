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

using namespace std;


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

vector<int> mutate(vector<int> tour){
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
  srand(time(0));
  // Use knowledge of the dataset size...
  ifstream inFile("DataSets/eil51.tsp"); int tourSize = 51;
  
  
  int populationSize = 100;
  int maxNumIterations = 20000;

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
  

  double bestFitness = 100000000.0;
  int nIteration = 0;
  while (nIteration < maxNumIterations){
    for (int i = 0; i < populationSize; i++){
      vector<int> tour = population[i];
      


      /*
      // Clone the original
      vector<int> newTour;
      for (int j = 0; j < tourSize; j++){
        newTour.push_back(tour[j]);
      }
      */

      vector<int> newTour = mutate(tour);

      double fitnessOne = calculate_fitness(tour);
      double fitnessTwo = calculate_fitness(newTour);
      //cout << "First " << fitnessOne << endl;
      //cout << "Second " << fitnessTwo << endl;

      if (fitnessTwo < fitnessOne){
        population[i] = newTour;
      }

      double fitnessBetter = calculate_fitness(population[i]);
    
      if (fitnessBetter < bestFitness){
        bestFitness = fitnessBetter;
        cout << bestFitness << endl;
      }
    }
  

    nIteration ++;
  }



  return 0;
}

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;


class City {
  public:
    City(int nodeNum, double x, double y) : nodeNum(nodeNum), x(x), y(y) {};
    int getNodeNum() {return nodeNum;}
    double getX() {return x;}
    double getY() {return y;}
  private:
    int nodeNum;
    double x;
    double y;
};


int main(){
  // Use knowledge of the dataset size...
  ifstream inFile("DataSets/eil51.tsp"); int size = 51;
  
  
  if (!inFile) {
    cerr << "Input file not found." << endl;
    return -1;
  }

  vector<City> cities;

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


  for (unsigned int i = 0; i < cities.size(); i++){
    City city = cities.at(i);
    cout << city.getNodeNum() << " " << city.getX() << " " << city.getY() << endl;
  }
  
   return 0;
}

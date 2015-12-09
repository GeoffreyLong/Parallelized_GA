#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;





int main(){
  ifstream inFile("DataSets/eil51.tsp");
  if (!inFile) {
    cerr << "Input file not found." << endl;
    return -1;
  }

  

  std::string line;
  while (std::getline(inFile, line)){
    if (line.empty()) continue;
    std::istringstream iss(line);
    double a, b, c;

    // Not entirely sure that this will work for all files...
    // If the input stream does not have three args it is not a tour location
    if (!(iss >> a >> b >> c)) { continue; }

    // Check output
    // cout << a << " " << b << " " << c << std::endl;
  }

   return 0;
}

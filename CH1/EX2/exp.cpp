#include <cmath>
#include <iostream>
#include <sstream>

int main (int argc, char **argv) {

  if(argc < 2){
    std::cout << "Missing Arguments!" << std::endl;
    std::cerr << "Usage: " << argv[0] << " <input>" << std::endl;
    return 1;
  }
  else{
    int integer;
    std::istringstream stream(argv[1]);
    stream >> integer;
    std::cout << exp(integer) << std::endl;

    return 0;
  }
}

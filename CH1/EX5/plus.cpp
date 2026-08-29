#include <iostream>
#include <sstream>

int main(int argc, char ** argv) {
  if(argc < 3){
    std::cout << "Missing Arguments!" << std::endl;
    std::cerr << "Usage: " << argv[0] << " <input 1> <input 2>" << std::endl;
  }
  else{
  
    std::istringstream xstream(argv[1]), ystream(argv[2]);
    double x, y;

    xstream >> x; ystream >> y;
    std::cout << x+y << std::endl;
  
    return 0;}
}

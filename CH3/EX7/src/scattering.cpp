#include <vector>
#include <iostream>
#include "getCoefficients.h"

int main(){
  double ka = 1.0;
  std::vector<double> potentials = {0.0,0.1,0.5, 0.9, 1.1, 2.0, 10.0};
  
  for(double v : potentials){
  auto [R, T] = getCoefficients(v);
  std::cout << "For V/E = " << v << ", R = " << R << ", T = " << T << std::endl;
  }
  

  return 0;
}

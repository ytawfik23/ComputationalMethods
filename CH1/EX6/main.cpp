#include <vector>
#include "Libmain.h"

int main(int argc, char ** argv){
  double x, y, sum;
  std::vector<double> input = parse();
  x = input[0];
  y = input[1];
  
  sum = add(x,y);
  print(sum);
  return 0;}

#include <vector>
#include <iostream>

std::vector<double> parse(){
  double input1, input2;
  std::cin >> input1 >> input2;
  
  std::vector<double> input_vector = {input1,input2};
  return input_vector;
}

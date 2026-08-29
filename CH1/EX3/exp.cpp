#include <cmath>
#include <iostream>
#include <sstream>

double factorial(int N){
  double result = 1;
  
  for (int j = 1; j<N+1; j++) {
      result *= j;
  }
  return result; 
}

double exp_taylor (double x, int NumberofTerms){
  double partial_sum = 0;
  
  for (int i=0; i<=NumberofTerms; i++){
    partial_sum += pow(x, i)/factorial(i);
  }
  return partial_sum;
}

int main (int argc, char **argv) {
  int x;
  for (int x=-10; x<12; x+=2){
    std::cout << x << " | " << exp_taylor(x,100) << std::endl;
  }
  return 0;}

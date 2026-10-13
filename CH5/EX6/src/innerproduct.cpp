#include <cstdlib>
#include <iostream>
#include <string>

#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/HermitePolynomial.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/Gamma.h"
#include "QatGenericFunctions/Sqrt.h"

#include "QatGenericFunctions/GaussIntegrator.h"
#include "QatGenericFunctions/GaussQuadratureRule.h"

#include "eigen3/Eigen/Dense"


int main (int argc, char **argv) {
  using namespace Genfun;

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  //Number of funcitons in the basis
  int Nbasis = 5;

  Variable X;
  GENFUNCTION exp = Exp();
  GENFUNCTION sqrt = Sqrt();
  GENFUNCTION factorial = Gamma()(X+1);
  GENFUNCTION f = exp(-X);

  //Implementing the basis functions into an array
  std::vector<std::unique_ptr<Genfun::AbsFunction>> phiList;
  for(int n=0; n<Nbasis; n++){
    GENFUNCTION phi = pow(M_PI,-0.25)/(sqrt(pow(2,n)*factorial(n)))*HermitePolynomial(n)*exp(-X*X/2);
    phiList.push_back(std::unique_ptr<Genfun::AbsFunction>(phi.clone()));
  }
  
  int NPoints = 5;
  GaussIntegrator integrator{GaussHermiteRule(NPoints)};

  Eigen::MatrixXd innerProductMatrix = Eigen::MatrixXd::Zero(Nbasis,Nbasis);
  for (int m=0; m<Nbasis; m++){
    for (int n=0; n<Nbasis; n++){
      innerProductMatrix(m,n) = integrator((*phiList[n])*(*phiList[m]));
    }
  }
  std::cout << "Inner Product Matrix for number of points = " << NPoints << std::endl; 
  std::cout << innerProductMatrix << std::endl;  
  
  return 0;

}
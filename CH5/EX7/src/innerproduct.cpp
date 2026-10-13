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

  //Implementing the basis functions into an array
  std::vector<std::unique_ptr<Genfun::AbsFunction>> phiList;
  for(int n=0; n<Nbasis; n++){
    GENFUNCTION phi = pow(M_PI,-0.25)/(sqrt(pow(2,n)*factorial(n)))*HermitePolynomial(n)*exp(-X*X/2);
    phiList.push_back(std::unique_ptr<Genfun::AbsFunction>(phi.clone()));
  }
  
  //Initializing Matricies
  Eigen::MatrixXd xMatrix = Eigen::MatrixXd::Zero(Nbasis,Nbasis);
  Eigen::MatrixXd x2Matrix = Eigen::MatrixXd::Zero(Nbasis,Nbasis);
  Eigen::MatrixXd DMatrix = Eigen::MatrixXd::Zero(Nbasis,Nbasis);
  Eigen::MatrixXd HMatrix = Eigen::MatrixXd::Zero(Nbasis,Nbasis);  

  int NPoints = 5;
  GaussIntegrator integrator{GaussHermiteRule(NPoints)};

  Eigen::MatrixXd innerProductMatrix = Eigen::MatrixXd::Zero(Nbasis,Nbasis);
  for (int m=0; m<Nbasis; m++){
    for (int n=0; n<Nbasis; n++){
      xMatrix(m,n) = integrator((*phiList[n])*(*phiList[m])*X);
      x2Matrix(m,n) = integrator((*phiList[m])*(*phiList[n])*X*X);
      DMatrix(m,n) = integrator((*phiList[m])*(*phiList[n]).prime());
      HMatrix(m,n) = -(1/2.0)*integrator((*phiList[m])*((*phiList[n]).prime().prime()))+
                      (1/2.0)*integrator((*phiList[m])*(*phiList[n])*X*X);
    }
  }
  std::cout << "[x] = " << std::endl; 
  std::cout << xMatrix << std::endl;

  std::cout << "[x^2] = " << std::endl; 
  std::cout << x2Matrix << std::endl;
  
  std::cout << "[d/dx] = " << std::endl; 
  std::cout << DMatrix << std::endl;

  std::cout << "[H] = " << std::endl; 
  std::cout << HMatrix << std::endl;
  

  return 1;

}
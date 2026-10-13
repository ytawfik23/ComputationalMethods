#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotProfile.h"

#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/F1D.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/RootFinder.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/FixedConstant.h"


#include "QatGenericFunctions/SimpleIntegrator.h"
#include "QatGenericFunctions/QuadratureRule.h"

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <algorithm>


std::vector<double> getTurningPointsCompanion(double energy, double l){
  double alpha{0.38};
  double mc2{1370};
  double a{2.43e-3};

  double c0 = (l+0.5)*(l+0.5) * a*a / mc2;
  double c1 = -4.0 * alpha * a*a /3.0;
  double c2 = -energy * a*a;

  Eigen::Matrix3d CompanionMatrix{{0.0, 0.0, -c0}, {1.0, 0.0, -c1}, {0.0,1.0, -c2}};
  Eigen::EigenSolver<Eigen::Matrix3d> solver(CompanionMatrix);
  Eigen::Vector3d eigenvalues = solver.eigenvalues().array().real();
  std::sort(eigenvalues.begin(), eigenvalues.end());
  double xmin = eigenvalues[1];
  double xmax = eigenvalues[2];

  return {xmin, xmax};
}


double action(double energy, double l){
  double alpha{0.38};
  double mc2{1370};
  double a{2.43e-3};

  std::vector<double> turningPoints = getTurningPointsCompanion(energy,l);
  
  Genfun::Variable X;
  Genfun::GENFUNCTION sqrt = Genfun::Sqrt();
  
  Genfun::GENFUNCTION integrand = (1/M_PI) * sqrt(mc2) * sqrt(energy - X/(a*a) + 4*alpha/3 /X - (l+0.5)*(l+0.5) /mc2 /(X*X));
  Genfun::SimpleIntegrator integrator(turningPoints[0],turningPoints[1], Genfun::OpenTrapezoidRule(), 1000);

  return (integrator(integrand)-0.5);
}

template<int L>
double action(double energy) {return action(energy, L);}

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  
  Genfun::F1D actionFunctionS{action<0>};
  Genfun::F1D actionFunctionP{action<1>};
  
  PlotProfile energyLevels;
  double mc2{1370};

  for(double quantumNumber=0.0; quantumNumber<3.0; quantumNumber++){

    Genfun::FixedConstant n{quantumNumber};  
    Genfun::GENFUNCTION f = actionFunctionS-n;

    Genfun::NewtonRaphson solver(f , 1.0E-8, 100);
    solver.lowerBound() = 0.0;
    double energy = solver.root(300.);

    std::cout << quantumNumber+1 << "S: E = " << energy << " MeV " << ", Mass = " << 2*mc2+energy << std::endl;
    energyLevels.addPoint(energy, quantumNumber);

  }

  for(double quantumNumber=0.0; quantumNumber<3.0; quantumNumber++){

  Genfun::FixedConstant n{quantumNumber};  
  Genfun::GENFUNCTION f = actionFunctionP-n;

  Genfun::NewtonRaphson solver(f , 1.0E-10, 100);
  solver.lowerBound() = 0.0;
  double energy = solver.root(480.);  
  std::cout << quantumNumber+1 << "P: E = " << energy << " MeV " << ", Mass = " << 2*mc2+energy << std::endl;
  energyLevels.addPoint(energy, quantumNumber);

  }
  return 1;
}
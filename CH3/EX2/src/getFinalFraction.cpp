#include <iostream>
#include <numbers>
#include <eigen3/Eigen/Dense>
#include <eigen3/unsupported/Eigen/MatrixFunctions>

Eigen::Vector3d getFinalFraction(Eigen::Vector3d initialFractions, double decayTime) {
  using namespace std::numbers;
  double tau_b = 45.6 / ln2;
  double tau_t = 132./60. / ln2;
  double tau_p = 195.0/ln2;
  Eigen::Matrix3d Lambda {
    {-1/tau_b, 0.0, 0.0},
    {1/tau_b, -1/tau_t, 0.0},
    {0.0, 1/tau_t,-1/tau_p}
  };
    Eigen::Vector3d finalFractions = (Lambda * decayTime).exp() * initialFractions;
    
    return finalFractions;
}

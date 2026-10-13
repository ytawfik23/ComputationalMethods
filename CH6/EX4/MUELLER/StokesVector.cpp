#include "StokesVector.h"
#include <stdexcept>

StokesVector::StokesVector() : stokes(Eigen::Vector4d::Zero()) {}

StokesVector::StokesVector(StokesVector::Type type) {

  if      (type==Horizontal)   stokes << 1,  1,  0,  0;
  else if (type==Vertical)     stokes << 1, -1,  0,  0;
  else if (type==Diagonal)     stokes << 1,  0,  1,  0;
  else if (type==Antidiagonal) stokes << 1,  0, -1,  0;
  else if (type==Right)        stokes << 1,  0,  0,  1;
  else if (type==Left)         stokes << 1,  0,  0, -1;
  else if (type==Unpolarized)  stokes << 1,  0,  0,  0;
  else throw std::invalid_argument("Unknown StokesVector type");

}

StokesVector::StokesVector(double s0, double s1, double s2, double s3)
  : stokes(s0,s1,s2,s3) {}

StokesVector::StokesVector(const Eigen::Vector4d & v) : stokes(v) {}

double & StokesVector::operator () (unsigned int i) {
  if (i>3) throw std::range_error("Index out of range");
  return stokes(i);
}

const double & StokesVector::operator () (unsigned int i) const {
  if (i>3) throw std::range_error("Index out of range");
  return stokes(i);
}

const Eigen::Vector4d & StokesVector::data() const {
  return stokes;
}

std::ostream & operator << (std::ostream & o, const StokesVector & v) {
  return o << "[" << v(0) << ", " << v(1) << ", " << v(2) << ", " << v(3) << "]";
}
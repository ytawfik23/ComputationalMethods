#include "MuellerMatrix.h"
#include <cmath>
#include <stdexcept>

namespace {

  // Rotation of the Stokes frame by angle theta:
  Eigen::Matrix4d rotation(double theta) {
    double c=std::cos(2.0*theta), s=std::sin(2.0*theta);
    Eigen::Matrix4d R = Eigen::Matrix4d::Identity();
    R(1,1)= c;  R(1,2)=s;
    R(2,1)=-s;  R(2,2)=c;
    return R;
  }

  Eigen::Matrix4d horizontalPolarizer() {
    Eigen::Matrix4d M = Eigen::Matrix4d::Zero();
    M(0,0)=M(0,1)=M(1,0)=M(1,1)=0.5;
    return M;
  }

  Eigen::Matrix4d horizontalRetarder(double delta) {
    Eigen::Matrix4d M = Eigen::Matrix4d::Identity();
    M(2,2)= std::cos(delta);  M(2,3)=-std::sin(delta);
    M(3,2)= std::sin(delta);  M(3,3)= std::cos(delta);
    return M;
  }

}

MuellerMatrix::MuellerMatrix() : mueller(Eigen::Matrix4d::Identity()) {}

MuellerMatrix::MuellerMatrix(MuellerMatrix::Type type, double angle) {

  Eigen::Matrix4d M;

  if      (type==Polarizer)        M=horizontalPolarizer();
  else if (type==QuarterWavePlate) M=horizontalRetarder(M_PI/2.0);
  else if (type==HalfWavePlate)    M=horizontalRetarder(M_PI);
  else throw std::invalid_argument("Unknown MuellerMatrix type");

  // Rotate the element from horizontal to the requested angle:
  mueller = rotation(-angle) * M * rotation(angle);
}

MuellerMatrix::MuellerMatrix(Eigen::Matrix4d m) : mueller(m) {}

double & MuellerMatrix::operator () (unsigned int i, unsigned int j) {
  if (i>3 || j>3) throw std::range_error("Index out of range");
  return mueller(i,j);
}

const double & MuellerMatrix::operator () (unsigned int i, unsigned int j) const {
  if (i>3 || j>3) throw std::range_error("Index out of range");
  return mueller(i,j);
}

const Eigen::Matrix4d & MuellerMatrix::data() const {
  return mueller;
}

std::ostream & operator << (std::ostream & o, const MuellerMatrix & m) {
  return o << m.data();
}

MuellerMatrix operator* (const MuellerMatrix & m1, const MuellerMatrix & m2) {
  Eigen::Matrix4d r = m1.data() * m2.data();
  return MuellerMatrix(r);
}

StokesVector operator* (const MuellerMatrix & m, const StokesVector & v) {
  Eigen::Vector4d r = m.data() * v.data();
  return StokesVector(r);
}
#ifndef _MUELLERMATRIX_H_
#define _MUELLERMATRIX_H_

#include <iostream>
#include <eigen3/Eigen/Dense>
#include "StokesVector.h"

// 4x4 Mueller matrix acting on a StokesVector.
class MuellerMatrix {

 public:

  enum Type {Polarizer, QuarterWavePlate, HalfWavePlate};

  // Construct the identity matrix:
  MuellerMatrix();

  // Construct an optical element type.  "angle" is the angle of the
  // transmission axis (Polarizer) or fast axis (wave plates), measured
  MuellerMatrix(Type type, double angle);

  // Construct from an Eigen matrix:
  MuellerMatrix(Eigen::Matrix4d m);

  // Access to individual elements:
  double & operator       () (unsigned int i, unsigned int j);
  const double & operator () (unsigned int i, unsigned int j) const;

  // Access to the underlying Eigen matrix:
  const Eigen::Matrix4d & data() const;

 private:

  Eigen::Matrix4d mueller;

};

std::ostream & operator << (std::ostream & o, const MuellerMatrix & m);
MuellerMatrix operator* (const MuellerMatrix & m1, const MuellerMatrix & m2);
StokesVector  operator* (const MuellerMatrix & m,  const StokesVector & v);

#endif
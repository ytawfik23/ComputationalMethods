#ifndef _STOKESVECTOR_H_
#define _STOKESVECTOR_H_

#include <iostream>
#include <eigen3/Eigen/Dense>

// Stokes vector (S0,S1,S2,S3) = (I, Q, U, V).  S3>0 is right circular.
class StokesVector {

 public:

  enum Type {Horizontal,Vertical,Diagonal,Antidiagonal,Right,Left,Unpolarized};

  // Construct a zero vector:
  StokesVector();

  // Construct a unit-intensity vector of a predefined type:
  StokesVector(Type type);

  // Construct from the four Stokes parameters:
  StokesVector(double s0, double s1, double s2, double s3);

  // Construct from an Eigen vector:
  explicit StokesVector(const Eigen::Vector4d & v);

  // Access to individual elements:
  double & operator () (unsigned int i);
  const double & operator () (unsigned int i) const;

  // Access to the underlying Eigen vector:
  const Eigen::Vector4d & data() const;

 private:

  Eigen::Vector4d stokes;

};

std::ostream & operator << (std::ostream & o, const StokesVector & v);

#endif
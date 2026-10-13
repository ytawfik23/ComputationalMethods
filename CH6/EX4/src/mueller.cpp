#include "MuellerMatrix.h"
#include <cmath>
#include <iostream>

int main (int argc, char **argv) {

  StokesVector vInitial(StokesVector::Horizontal);

  MuellerMatrix HWP(MuellerMatrix::HalfWavePlate, M_PI/4);     // Diagonal Quarter Wave Plate
  MuellerMatrix PH(MuellerMatrix::Polarizer,        M_PI/4.0);    // Diagonal Polarizer

  std::cout << "Horizontal Polarized Light -> Diagonal Half Wave Plate" << std::endl;
  std::cout << HWP * vInitial << " (Vertical Polarization)" <<  std::endl;
  
  std::cout << "Horizontal Polarized Light -> Diagonal Polarizer" << std::endl;
  std::cout << PH * vInitial << " (Attenuated Diagonal Polarization) "<< std::endl;

  return 0;
}
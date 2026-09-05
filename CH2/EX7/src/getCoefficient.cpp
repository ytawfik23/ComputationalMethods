#include <complex>
#include <iostream>
#include <utility>

std::pair<double, double> coefficient (double v) {
    std::complex<double> n = std::sqrt(std::complex<double>{1 - v, 0.0});
    
    double R = 1. - norm(2.0/(1.0+n))*n.real();
    double T = norm(2.0/(1.0+n))*n.real();
    
    return std::make_pair(R, T);
}
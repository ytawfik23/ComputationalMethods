#include <iostream>
#include <iomanip>
#include <cmath>
#include "Gamma.h"

int main(int argc, char **argv) {
    int n = 16;
    double epsilon = 1.0;
    double beta = 1.0 - epsilon;

    std::cout << std::left 
    << std::setw(15) << "epsilon" 
    << std::setw(20) << "gamma (poor)" 
    << std::setw(20) << "gamma (good)" 
    << std::setw(24) << "relative difference" << std::endl;


    for (int i = 0; i < n; i++) {
    epsilon = epsilon/10.;
    beta = 1.0 - epsilon;
    std::cout << std::left 
    << std::setw(15) << epsilon 
    << std::setw(20) << gamma_1(beta)
    << std::setw(20) << gamma_2(epsilon)
    << std::setw(24) << std::fabs(gamma_1(beta) - gamma_2(epsilon)) / gamma_2(epsilon)
    << std::endl;
    }
    return 0;
}
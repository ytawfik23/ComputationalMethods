#include <complex>
#include <iostream>
#include <iomanip>
#include "Step.h" 

int main (int argc, char **argv) {
    double v{};
    for (v=0.9; v<1.1; v+=0.005) {
        auto [R, T] = coefficient(v);
       
        std::cout << std::left <<
        std::setw(5) << "v = " << std::setw(5) << v <<
        std::setw(5) <<", R = " << std::setw(5) << R <<
        std::setw(5) <<", T = " << std::setw(5) << T << std::endl;
    }
    return 0;}  
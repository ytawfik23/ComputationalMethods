#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>
#include <complex>

int main(int argc, char ** argv) {
    const double pi = 4 * std::atan(1.0);
    std::vector<double> angles = {0.0, pi/6, pi/4, pi/3, pi/2, 2*pi/3, 3*pi/4, 5*pi/6, pi};
    std::complex<double> i(0.0, 1.0);

    std::cout << std::setw(15) << "x" << std::setw(15) 
    << "sinh(x)"  << std::setw(15) << "sin(ix)" << std::setw(15) 
    << "cosh(x)" << std::setw(15) << "cos(ix)" << std::endl;
    for (double angle : angles) {
        std::cout << 
        std::setw(15) << angle << 
        std::setw(15) << std::sinh(angle) << std::setw(15) << std::sin(i * angle) << 
        std::setw(15) << std::cosh(angle) << std::setw(15) << std::cos(i * angle) <<
        std::endl;
    }
    return 0;
}
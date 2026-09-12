#include <iostream>
#include <eigen3/Eigen/Dense>
#include "decay.h"


int main(){
    Eigen::Vector3d initialFractions {1.0, 0.0, 0.0};
    
    std::vector<double> decayTimes = {0., 50., 150., 200., 500., 1000.};

    for(double time : decayTimes) {
        Eigen::Vector3d finalFraction = getFinalFraction(initialFractions, time);
        std::cout << "Final fractions after decay time " << time << " min" << ": " << std::endl << finalFraction << std::endl;
    }
return 0;}


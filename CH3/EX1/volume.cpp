#include <iostream>
#include <vector>
#include <numeric>
#include <eigen3/Eigen/Dense>


int main() {
    std::vector<double> vectorA = {3.0,0.0,0.0};
    std::vector<double> vectorB = {0.5,2.0,0.0};
    std::vector<double> vectorC = {0.3,0.2,1.5};

    std::vector<double> crossProductAB = 
    {
        vectorA[1] * vectorB[2] - vectorA[2] * vectorB[1],
        vectorA[2] * vectorB[0] - vectorA[0] * vectorB[2],
        vectorA[0] * vectorB[1] - vectorA[1] * vectorB[0]
    };
    double volume_vector = std::inner_product(crossProductAB.begin(), crossProductAB.end(), vectorC.begin(), 0.0);
    std::cout << "The volume of the parallelepiped is: " << volume_vector << std::endl;

    Eigen::Matrix3d volume_matrix { 
                                    {3.0,0.0,0.0},
                                    {0.5,2.0,0.0},
                                    {0.3,0.2,1.5}
                                  };
    
    double volume_eigen = volume_matrix.determinant();
    
    std::cout << "The volume of the parallelepiped is: " << volume_eigen << std::endl;
    
    return 0;
}
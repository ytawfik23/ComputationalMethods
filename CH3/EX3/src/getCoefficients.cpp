#include <iostream>
#include <eigen3/Eigen/Dense>
#include <complex>

std::pair<double,double> getCoefficients(double ka) {
    std::complex<double> I{0.0, 1.0};
    double v = 1.1;
    std::complex<double> n1 = sqrt(std::complex(1.0 - v));
    std::complex<double> n2 = sqrt(std::complex(1.0 - 2*v));

    Eigen::MatrixXcd S 
    {
    { -std::exp(2.0*I*ka),         std::exp(-2.0*I*n1*ka),    std::exp(2.0*I*n1*ka),      0.0,                        0.0,                      0.0,                        0.0,                       0.0 },
    {  std::exp(2.0*I*ka),         n1*std::exp(-2.0*I*n1*ka), -n1*std::exp(2.0*I*n1*ka),  0.0,                        0.0,                      0.0,                        0.0,                       0.0 },
    {  0.0,                        std::exp(-I*n1*ka),        std::exp(I*n1*ka),         -std::exp(-I*n2*ka),        -std::exp(I*n2*ka),        0.0,                        0.0,                       0.0 },
    {  0.0,                        n1*std::exp(-I*n1*ka),     -n1*std::exp(I*n1*ka),     -n2*std::exp(-I*n2*ka),      n2*std::exp(I*n2*ka),      0.0,                        0.0,                       0.0 },
    {  0.0,                        0.0,                       0.0,                        std::exp(I*n2*ka),         std::exp(-I*n2*ka),       -std::exp(I*n1*ka),         -std::exp(-I*n1*ka),        0.0 },
    {  0.0,                        0.0,                       0.0,                        n2*std::exp(I*n2*ka),     -n2*std::exp(-I*n2*ka),     -n1*std::exp(I*n1*ka),       n1*std::exp(-I*n1*ka),      0.0 },
    {  0.0,                        0.0,                       0.0,                        0.0,                        0.0,                       std::exp(2.0*I*n1*ka),      std::exp(-2.0*I*n1*ka),   -std::exp(2.0*I*ka) },
    {  0.0,                        0.0,                       0.0,                        0.0,                        0.0,                       n1*std::exp(2.0*I*n1*ka),  -n1*std::exp(-2.0*I*n1*ka), -std::exp(2.0*I*ka) }
    };

    Eigen::VectorXcd input {{std::exp(-2.0*I*ka)}, {std::exp(-2.0*I*ka)}, {0.0}, {0.0}, {0.0}, {0.0}, {0.0}, {0.0}};

    Eigen::VectorXcd coefficients ((S.inverse())*input);
    return {norm(coefficients[0]), norm(coefficients[7]) };
    }

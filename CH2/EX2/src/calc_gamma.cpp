#include <cmath>

double gamma_1(double beta) {
    return 1.0 / std::sqrt(1.0 - beta * beta);
}
double gamma_2(double epsilon) {
    return 1/std::sqrt((2-epsilon)*(epsilon));
}

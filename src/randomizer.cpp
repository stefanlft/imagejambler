#include <randomizer.hpp>

int Randomizer::getNextInt(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}
int Randomizer::getNextInt(int max) {
    std::uniform_int_distribution<int> distribution(0, max);
    return distribution(generator);
}
int Randomizer::getNextInt() {
    std::uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);
    return distribution(generator);
}

double Randomizer::getNextReal(double min, double max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}
double Randomizer::getNextReal(double max) {
    std::uniform_real_distribution<double> distribution(0.0, max);
    return distribution(generator);
}
double Randomizer::getNextReal() {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}
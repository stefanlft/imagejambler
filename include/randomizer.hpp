#pragma once

#include <random>
#include <climits>

class Randomizer {
private:
    std::random_device rd;
    std::mt19937 generator;
    
public:
    Randomizer() : generator(rd()) { }

    int getNextInt();
    int getNextInt(int max);
    int getNextInt(int min, int max);

    double getNextReal();
    double getNextReal(double max);
    double getNextReal(double min, double max);
};
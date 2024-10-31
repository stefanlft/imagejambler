#pragma once

#include <random>
#include <climits>

class Randomizer {
private:
    std::random_device rd;
    std::mt19937 generator;
    
public:
    Randomizer() : generator(rd()) { }
    ~Randomizer() { }

    int getNextInt();
    int getNextInt(int max);
    int getNextInt(int min, int max);

    double getNextReal();
    double getNextReal(double max);
    double getNextReal(double min, double max);

    void random3D(std::vector<std::vector<std::pair<int, int>>> &vec, const int height, const int width, const int min, const int max);
};
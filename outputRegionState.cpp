#include "simulation.h"
#include "outputRegionState.h"
#include <iostream>
#include <fstream>
#include <sstream>

void outputRegionState(const std::vector<std::vector<Cell>>& region, int timeStep) {
    // std::cout << "Time Step: " << timeStep << std::endl;
    for (const auto& row : region) {
        for (const auto& cell : row) {
            if (cell.population == 0) {
                std::cout << cell.type << " ";
            } else {
                std::cout << cell.population << " ";
            }
        }
        std::cout << std::endl;
    }
}
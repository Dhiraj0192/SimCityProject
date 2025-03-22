#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>

struct Cell {
    char type;
    int population;
    int pollution;
};



void spreadPollution(std::vector<std::vector<Cell>>& region);
void outputFinalState(const std::vector<std::vector<Cell>>& region);
void analyzeRegion(const std::vector<std::vector<Cell>>& region);

#endif // SIMULATION_H

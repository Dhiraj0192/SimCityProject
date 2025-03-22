#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>

struct Cell {
    char type;
    int population;
    int pollution;
};

void readConfiguration(const std::string& filename, int& maxTimeSteps, int& refreshRate);
void readRegionLayout(const std::string& filename, std::vector<std::vector<Cell>>& region);
void outputRegionState(const std::vector<std::vector<Cell>>& region, int timeStep);
void simulateCityDevelopment(std::vector<std::vector<Cell>>& region, int maxTimeSteps, int refreshRate);
void updateResidential(std::vector<std::vector<Cell>>& region, int& availableWorkers);
void updateIndustrial(std::vector<std::vector<Cell>>& region, int& availableWorkers, int& availableGoods);
void updateCommercial(std::vector<std::vector<Cell>>& region, int& availableWorkers, int& availableGoods);
void spreadPollution(std::vector<std::vector<Cell>>& region);
void outputFinalState(const std::vector<std::vector<Cell>>& region);
void analyzeRegion(const std::vector<std::vector<Cell>>& region);

#endif // SIMULATION_H

#include "simulation.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Function declarations
void updateResidential(std::vector<std::vector<Cell>>& region, int& availableWorkers);
void updateIndustrial(std::vector<std::vector<Cell>>& region, int& availableWorkers, int& availableGoods);
void updateCommercial(std::vector<std::vector<Cell>>& region, int& availableWorkers, int& availableGoods);
void spreadPollution(std::vector<std::vector<Cell>>& region);

void readConfiguration(const std::string& filename, int& maxTimeSteps, int& refreshRate) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line); // Skip the first line (filename)
        std::getline(file, line);
        
        maxTimeSteps = std::stoi(line);
        std::getline(file, line);
        refreshRate = std::stoi(line);
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void readRegionLayout(const std::string& filename, std::vector<std::vector<Cell>>& region) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<Cell> row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                Cell c = {cell[0], 0, 0};
                row.push_back(c);
            }
            region.push_back(row);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

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

void simulateCityDevelopment(std::vector<std::vector<Cell>>& region, int maxTimeSteps, int refreshRate) {
    int availableWorkers = 0;
    int availableGoods = 0;
    for (int timeStep = 1; timeStep <= maxTimeSteps; ++timeStep) {
        
        updateResidential(region, availableWorkers);
        updateIndustrial(region, availableWorkers, availableGoods);
        updateCommercial(region, availableWorkers, availableGoods);
        spreadPollution(region);

        if (timeStep % refreshRate == 0) {
            outputRegionState(region, timeStep);
        }
    }
}

// New helper function definitions
void updateResidential(std::vector<std::vector<Cell>>& region, int& availableWorkers) {
    
    for (auto& row : region) {
        for (auto& cell : row) {
            if (cell.type == 'R') {
                if (cell.population == 0) {
                    bool adjacentPowerline = false;
                    bool adjacentPopulated = false;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size()) {
                                if (region[y][x].type == 'T' || region[y][x].type == '#') {
                                    adjacentPowerline = true;
                                }
                                if (region[y][x].population > 0) {
                                    adjacentPopulated = true;
                                }
                            }
                        }
                    }
                    if (adjacentPowerline || adjacentPopulated) {
                        cell.population++;
                        availableWorkers++;
                    }
                } else if (cell.population == 1) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 1) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 2) {
                        cell.population++;
                        availableWorkers++;
                    }
                } else if (cell.population == 2) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 2) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 4) {
                        cell.population++;
                        availableWorkers++;
                    }
                } else if (cell.population == 3) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 3) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 6) {
                        cell.population++;
                        availableWorkers++;
                    }
                } else if (cell.population == 4) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 4) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 8) {
                        cell.population++;
                        availableWorkers++;
                    }
                }
            }
        }
    }
}

void updateIndustrial(std::vector<std::vector<Cell>>& region, int& availableWorkers, int& availableGoods) {
    // std::cout << "Updating Industrial Areas" << std::endl; // Debug print
    for (auto& row : region) {
        for (auto& cell : row) {
            if (cell.type == 'I') {
                if (cell.population == 0 && availableWorkers >= 2) {
                    bool adjacentPowerline = false;
                    bool adjacentPopulated = false;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size()) {
                                if (region[y][x].type == 'T' || region[y][x].type == '#') {
                                    adjacentPowerline = true;
                                }
                                if (region[y][x].population > 0) {
                                    adjacentPopulated = true;
                                }
                            }
                        }
                    }
                    if (adjacentPowerline || adjacentPopulated) {
                        cell.population++;
                        availableWorkers -= 2;
                        availableGoods++;
                    }
                } else if (cell.population == 1 && availableWorkers >= 2) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 1) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 2) {
                        cell.population++;
                        availableWorkers -= 2;
                        availableGoods++;
                    }
                } else if (cell.population == 2 && availableWorkers >= 2) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 2) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 4) {
                        cell.population++;
                        availableWorkers -= 2;
                        availableGoods++;
                    }
                }
            }
        }
    }
}

void updateCommercial(std::vector<std::vector<Cell>>& region, int& availableWorkers, int& availableGoods) {
    // std::cout << "Updating Commercial Areas" << std::endl; // Debug print
    for (auto& row : region) {
        for (auto& cell : row) {
            if (cell.type == 'C') {
                if (cell.population == 0 && availableWorkers >= 1 && availableGoods >= 1) {
                    bool adjacentPowerline = false;
                    bool adjacentPopulated = false;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size()) {
                                if (region[y][x].type == 'T' || region[y][x].type == '#') {
                                    adjacentPowerline = true;
                                }
                                if (region[y][x].population > 0) {
                                    adjacentPopulated = true;
                                }
                            }
                        }
                    }
                    if (adjacentPowerline || adjacentPopulated) {
                        cell.population++;
                        availableWorkers--;
                        availableGoods--;
                    }
                } else if (cell.population == 1 && availableWorkers >= 1 && availableGoods >= 1) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 1) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 2) {
                        cell.population++;
                        availableWorkers--;
                        availableGoods--;
                    }
                } else if (cell.population == 2 && availableWorkers >= 1 && availableGoods >= 1) {
                    int adjacentPopulatedCount = 0;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            int x = &cell - &row[0] + i;
                            int y = &row - &region[0] + j;
                            if (x >= 0 && x < row.size() && y >= 0 && y < region.size() && region[y][x].population >= 2) {
                                adjacentPopulatedCount++;
                            }
                        }
                    }
                    if (adjacentPopulatedCount >= 4) {
                        cell.population++;
                        availableWorkers--;
                        availableGoods--;
                    }
                }
            }
        }
    }
}

void spreadPollution(std::vector<std::vector<Cell>>& region) {
    // std::cout << "Spreading Pollution" << std::endl; // Debug print
    std::vector<std::vector<int>> pollutionMap(region.size(), std::vector<int>(region[0].size(), 0));
    for (int y = 0; y < region.size(); ++y) {
        for (int x = 0; x < region[y].size(); ++x) {
            if (region[y][x].type == 'I') {
                int pollution = region[y][x].population;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        int newX = x + i;
                        int newY = y + j;
                        if (newX >= 0 && newX < region[y].size() && newY >= 0 && newY < region.size()) {
                            pollutionMap[newY][newX] += std::max(0, pollution - std::abs(i) - std::abs(j));
                        }
                    }
                }
            }
        }
    }
    for (int y = 0; y < region.size(); ++y) {
        for (int x = 0; x < region[y].size(); ++x) {
            region[y][x].pollution = pollutionMap[y][x];
        }
    }
}

void outputFinalState(const std::vector<std::vector<Cell>>& region) {
    int totalResidentialPopulation = 0;
    int totalIndustrialPopulation = 0;
    int totalCommercialPopulation = 0;
    int totalPollution = 0;

    for (const auto& row : region) {
        for (const auto& cell : row) {
            if (cell.type == 'R') {
                totalResidentialPopulation += cell.population;
            } else if (cell.type == 'I') {
                totalIndustrialPopulation += cell.population;
            } else if (cell.type == 'C') {
                totalCommercialPopulation += cell.population;
            }
            totalPollution += cell.pollution;
        }
    }

    std::cout << "Final Region State:" << std::endl;
    outputRegionState(region, -1); // -1 to indicate final state

    // Only show output after the final region state
    std::cout << "Total Residential Population: " << totalResidentialPopulation << std::endl;
    std::cout << "Total Industrial Population: " << totalIndustrialPopulation << std::endl;
    std::cout << "Total Commercial Population: " << totalCommercialPopulation << std::endl;
    std::cout << "Total Pollution: " << totalPollution << std::endl;

    // Prompt for area analysis
    int x1, y1, x2, y2;
    std::cout << "Enter the coordinates of the rectangular area to analyze (x1 y1 x2 y2): ";
    std::cin >> x1 >> y1 >> x2 >> y2;

    int areaResidentialPopulation = 0;
    int areaIndustrialPopulation = 0;
    int areaCommercialPopulation = 0;
    int areaPollution = 0;

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            const Cell& cell = region[y][x];
            if (cell.type == 'R') {
                areaResidentialPopulation += cell.population;
            } else if (cell.type == 'I') {
                areaIndustrialPopulation += cell.population;
            } else if (cell.type == 'C') {
                areaCommercialPopulation += cell.population;
            }
            areaPollution += cell.pollution;
        }
    }

    std::cout << "Analysis of the specified area:" << std::endl;
    std::cout << "Total Residential Population: " << areaResidentialPopulation << std::endl;
    std::cout << "Total Industrial Population: " << areaIndustrialPopulation << std::endl;
    std::cout << "Total Commercial Population: " << areaCommercialPopulation << std::endl;
    std::cout << "Total Pollution: " << areaPollution << std::endl;
}

void analyzeRegion(const std::vector<std::vector<Cell>>& region) {
    int x1, y1, x2, y2;
    while (true) {
        std::cout << "Enter the coordinates of the rectangular area to analyze (x1 y1 x2 y2): ";
        std::cin >> x1 >> y1 >> x2 >> y2;
        if (x1 >= 0 && x1 < region[0].size() && x2 >= 0 && x2 < region[0].size() &&
            y1 >= 0 && y1 < region.size() && y2 >= 0 && y2 < region.size() && x1 <= x2 && y1 <= y2) {
            break;
        } else {
            std::cout << "Invalid coordinates. Please try again." << std::endl;
        }
    }

    int totalResidentialPopulation = 0;
    int totalIndustrialPopulation = 0;
    int totalCommercialPopulation = 0;
    int totalPollution = 0;

    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            const Cell& cell = region[y][x];
            if (cell.type == 'R') {
                totalResidentialPopulation += cell.population;
            } else if (cell.type == 'I') {
                totalIndustrialPopulation += cell.population;
            } else if (cell.type == 'C') {
                totalCommercialPopulation += cell.population;
            }
            totalPollution += cell.pollution;
        }
    }

    std::cout << "Analysis of the specified area:" << std::endl;
    std::cout << "Total Residential Population: " << totalResidentialPopulation << std::endl;
    std::cout << "Total Industrial Population: " << totalIndustrialPopulation << std::endl;
    std::cout << "Total Commercial Population: " << totalCommercialPopulation << std::endl;
    std::cout << "Total Pollution: " << totalPollution << std::endl;
}

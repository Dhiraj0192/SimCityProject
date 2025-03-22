#include "simulation.h"
#include "outputRegionState.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

#include "simulation.h"
#include "spreadPollution.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
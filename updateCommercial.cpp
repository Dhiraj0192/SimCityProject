#include "simulation.h"
#include "updateCommercial.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
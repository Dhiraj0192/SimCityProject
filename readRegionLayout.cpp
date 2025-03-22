#include "simulation.h"
#include "readRegionLayout.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
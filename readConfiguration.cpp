#include "simulation.h"
#include "readConfiguration.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
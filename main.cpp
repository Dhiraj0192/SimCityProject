#include "simulation.h"
#include "readConfiguration.h"
#include "readRegionLayout.h"
#include "outputRegionState.h"
#include "simulateCityDevelopment.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <configuration_file> <region_layout_file>" << std::endl;
        return 1;
    }

    std::string configFilename = argv[1];
    std::string regionFilename = argv[2];

    int maxTimeSteps;
    int refreshRate;
    readConfiguration(configFilename, maxTimeSteps, refreshRate);

    std::vector<std::vector<Cell>> region;
    readRegionLayout(regionFilename, region);

    outputRegionState(region, 0); // Output initial state

    simulateCityDevelopment(region, maxTimeSteps, refreshRate);
    outputFinalState(region);
    analyzeRegion(region);

    return 0;
}

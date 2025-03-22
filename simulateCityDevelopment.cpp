#include "simulation.h"
#include "simulateCityDevelopment.h"
#include "updateResidential.h"
#include "outputRegionState.h"
#include "updateIndustrial.h"
#include "updateCommercial.h"
#include "spreadPollution.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
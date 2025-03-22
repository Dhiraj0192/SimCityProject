#ifndef REGION_H
#define REGION_H

#include <vector>
#include <string>

enum class CellType {
    RESIDENTIAL,
    INDUSTRIAL,
    COMMERCIAL,
    ROAD,
    POWERLINE,
    POWERLINE_ROAD,
    POWERPLANT,
    EMPTY
};

struct Cell {
    CellType type;
    int population;
    double pollution;
    bool hasPower;
    
    Cell() : type(CellType::EMPTY), population(0), pollution(0), hasPower(false) {}
};

class Region {
private:
    std::vector<std::vector<Cell>> grid;
    int rows;
    int cols;
    int availableWorkers;
    int availableGoods;

    void updatePowerStatus(); // New method to update power status
    void loadLayout(const std::string& layoutFile); // New method to load layout

public:
    Region(const std::string& layoutFile);
    void loadFromFile(const std::string& filename);
    void simulate();
    void printState() const;
    bool isValidCoordinate(int x, int y) const;
    std::vector<Cell*> getAdjacentCells(int x, int y);
    void updatePollution();
    void calculateResources();
    
    // Getters
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    const Cell& getCell(int x, int y) const { return grid[y][x]; }
    int getTotalPopulation() const;
    int getZonePopulation(CellType type) const;
    double getTotalPollution() const;
    void getAreaStats(int x1, int y1, int x2, int y2, int& resPop, int& indPop, int& comPop, double& pollution) const;
};

#endif

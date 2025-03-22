#include "Region.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <queue>
#include <stdexcept>

Region::Region(const std::string& filename) {
    loadFromFile(filename);
    availableWorkers = 0;
    availableGoods = 0;
}

void Region::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    std::vector<std::vector<Cell>> tempGrid;
    
    while (std::getline(file, line)) {
        std::vector<Cell> row;
        std::istringstream ss(line);
        char c;
        
        while (ss >> c) {
            Cell cell;
            switch (c) {
                case 'R': cell.type = CellType::RESIDENTIAL; break;
                case 'I': cell.type = CellType::INDUSTRIAL; break;
                case 'C': cell.type = CellType::COMMERCIAL; break;
                case '-': cell.type = CellType::ROAD; break;
                case 'T': cell.type = CellType::POWERLINE; break;
                case '#': cell.type = CellType::POWERLINE_ROAD; break;
                case 'P': cell.type = CellType::POWERPLANT; break;
                default: cell.type = CellType::EMPTY;
            }
            row.push_back(cell);
            ss.ignore(); // Skip delimiter
        }
        if (!row.empty()) {
            tempGrid.push_back(row);
        }
    }
    
    if (!tempGrid.empty()) {
        rows = tempGrid.size();
        cols = tempGrid[0].size();
        grid = tempGrid;
    } else {
        throw std::runtime_error("Layout file is empty or invalid: " + filename);
    }
}

std::vector<Cell*> Region::getAdjacentCells(int x, int y) {
    std::vector<Cell*> adjacent;
    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < 8; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];
        if (isValidCoordinate(newX, newY)) {
            adjacent.push_back(&grid[newY][newX]);
        }
    }
    return adjacent;
}

bool Region::isValidCoordinate(int x, int y) const {
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

void Region::updatePollution() {
    std::vector<std::vector<double>> newPollution(rows, std::vector<double>(cols, 0));
    
    // Calculate pollution spread
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x].type == CellType::INDUSTRIAL) {
                // Pollution equal to population
                double pollution = grid[y][x].population;
                
                // Spread pollution to adjacent cells
                std::queue<std::pair<int, std::pair<int, int>>> q;
                q.push({0, {x, y}});
                std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
                
                while (!q.empty()) {
                    int dist = q.front().first;
                    int currX = q.front().second.first;
                    int currY = q.front().second.second;
                    q.pop();
                    
                    if (!visited[currY][currX]) {
                        visited[currY][currX] = true;
                        newPollution[currY][currX] += std::max(0.0, pollution - dist);
                        
                        if (dist < pollution) {
                            for (int i = 0; i < 8; ++i) {
                                int newX = currX + (i % 3) - 1;
                                int newY = currY + (i / 3) - 1;
                                if (isValidCoordinate(newX, newY) && !visited[newY][newX]) {
                                    q.push({dist + 1, {newX, newY}});
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Update pollution values
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            grid[y][x].pollution = newPollution[y][x];
        }
    }
}

void Region::calculateResources() {
    availableWorkers = 0;
    availableGoods = 0;
    
    // Calculate available workers from residential zones
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x].type == CellType::RESIDENTIAL) {
                availableWorkers += grid[y][x].population;
            }
        }
    }
    
    // Calculate available goods from industrial zones
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x].type == CellType::INDUSTRIAL) {
                availableGoods += grid[y][x].population;
            }
        }
    }
}

void Region::updatePowerStatus() {
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;

    // Initialize power from power plants
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x].type == CellType::POWERPLANT) {
                q.push({x, y});
                visited[y][x] = true;
                grid[y][x].hasPower = true;
            }
        }
    }

    // BFS to spread power through powerlines and powerline roads
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        const int dx[] = {-1, 1, 0, 0};
        const int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValidCoordinate(newX, newY) && !visited[newY][newX] &&
                (grid[newY][newX].type == CellType::POWERLINE || grid[newY][newX].type == CellType::POWERLINE_ROAD)) {
                visited[newY][newX] = true;
                grid[newY][newX].hasPower = true;
                q.push({newX, newY});
            }
        }
    }

    // Update power status for all cells
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (grid[y][x].type != CellType::POWERLINE && grid[y][x].type != CellType::POWERLINE_ROAD) {
                grid[y][x].hasPower = visited[y][x];
            }
        }
    }
}

void Region::simulate() {
    std::vector<std::vector<Cell>> newGrid = grid;
    
    // Update power status
    updatePowerStatus();
    
    // Process growth rules in priority order
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Cell& cell = grid[y][x];
            auto adjacent = getAdjacentCells(x, y);
            
            switch (cell.type) {
                case CellType::RESIDENTIAL:
                    // Implement residential growth rules
                    break;
                    
                case CellType::INDUSTRIAL:
                    // Implement industrial growth rules
                    if (availableWorkers >= 2) {
                        // Process industrial growth rules
                    }
                    break;
                    
                case CellType::COMMERCIAL:
                    // Implement commercial growth rules
                    if (availableWorkers >= 1 && availableGoods >= 1) {
                        // Process commercial growth rules
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    grid = newGrid;
    updatePollution();
    calculateResources();
}

void Region::printState() const {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            const Cell& cell = grid[y][x];
            if (cell.population > 0) {
                std::cout << cell.population;
            } else {
                switch (cell.type) {
                    case CellType::RESIDENTIAL: std::cout << 'R'; break;
                    case CellType::INDUSTRIAL: std::cout << 'I'; break;
                    case CellType::COMMERCIAL: std::cout << 'C'; break;
                    case CellType::ROAD: std::cout << '-'; break;
                    case CellType::POWERLINE: std::cout << 'T'; break;
                    case CellType::POWERLINE_ROAD: std::cout << '#'; break;
                    case CellType::POWERPLANT: std::cout << 'P'; break;
                    default: std::cout << ' ';
                }
            }
            std::cout << ' ';
        }
        std::cout << '\n';
    }
}

int Region::getTotalPopulation() const {
    int total = 0;
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            total += cell.population;
        }
    }
    return total;
}

int Region::getZonePopulation(CellType type) const {
    int total = 0;
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if (cell.type == type) {
                total += cell.population;
            }
        }
    }
    return total;
}

double Region::getTotalPollution() const {
    double total = 0;
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            total += cell.pollution;
        }
    }
    return total;
}

void Region::getAreaStats(int x1, int y1, int x2, int y2, int& resPop, int& indPop, int& comPop, double& pollution) const {
    resPop = indPop = comPop = 0;
    pollution = 0;
    
    for (int y = y1; y <= y2; ++y) {
        for (int x = x1; x <= x2; ++x) {
            if (!isValidCoordinate(x, y)) continue;
            
            const Cell& cell = grid[y][x];
            switch (cell.type) {
                case CellType::RESIDENTIAL: resPop += cell.population; break;
                case CellType::INDUSTRIAL: indPop += cell.population; break;
                case CellType::COMMERCIAL: comPop += cell.population; break;
                default: break;
            }
            pollution += cell.pollution;
        }
    }
}

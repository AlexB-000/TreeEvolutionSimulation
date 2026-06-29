#pragma once
#include <SFML/Graphics.hpp>

#include "simulation.hpp"

struct Cell{
    sf::Vector2i position {0, 0}; // (x, y) corresponds to (column, line) in the grid
    unsigned short type = 0; // 0 - ded, 1 - sprout, 2 - leaf, 3 - seed
    unsigned int activeGene = 0;

    int32_t energy = 0;

    int treeId = 0;

    Cell(sf::Vector2i inPosition={0, 0}):
        position(inPosition) {}

    void show(){
        std::cout << "Cell at (i, j) (" << position.y << ", " << position.x << ") \n"
                  << "Type: " << type << ", "
                  << "Active Gene: " << activeGene << ", "
                  << "Energy: " << energy << ", "
                  << "Tree ID: " << treeId << std::endl;
    }
};
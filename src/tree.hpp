#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "cell.hpp"

struct Tree{
    std::vector< Cell* > cells;
    int32_t numberOfSprouts = 0;
    int age = 0;
    int32_t energy = 0;
    std::vector<std::vector<int>> ADN;
    sf::Color color;

    Tree(Cell* seed, sf::Color inColor,
        std::vector<std::vector<int>> inADN = std::vector<std::vector<int>>(conf::genomeLength, std::vector<int>(4, 0))):
        ADN(inADN), color(inColor) {
        cells.push_back(seed);
    }
};
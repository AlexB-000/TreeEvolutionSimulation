#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include "index_vector.hpp"
#include "configuration.hpp"
#include "tree.hpp"
#include "cell.hpp"

using cell_pos = sf::Vector2i;

void showADN(std::vector<std::vector<int>> ADN);
sf::Color wheelColor();
sf::Color randomColor();

class Simulation{
    int generation = 0;
    int stepCounter = 0;

    std::vector<std::vector< int >> updateSunLight(std::vector<std::vector< int >> & lightMatrix);
    void produceEnergy(Cell* cell, const std::vector<std::vector< int >> & light);
    bool useEnergy(Cell* cell);

    void fall(Cell* cell);
    void grow(Cell* cell);

    void spreadSeed(siv::ID treeId);
    void dieTree(siv::ID treeId);

    void birth(Cell* newCell, Cell* cell, int inType, int energy, int inActiveGene);

    void newTree(Cell* seed, sf::Color color,  const std::vector<std::vector< int >> inADN);
    std::vector<std::vector< int >> mutation(std::vector<std::vector< int >> ADN);
    std::vector<std::vector< int >> randomADN();

public:
    siv::Vector< Tree > trees;
    std::vector<std::vector< Cell >> world; // all cells

    std::vector<std::vector< int >> light;

    Simulation();

    void throwSeeds(int n);
    void step();
};
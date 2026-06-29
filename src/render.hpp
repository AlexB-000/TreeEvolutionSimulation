#pragma once
#include <SFML/Graphics.hpp>
#include "configuration.hpp"
#include "simulation.hpp"

struct Render{
    Simulation & simulation;

    const int modes = 4;
    int renderMode = 0; // 0 - Default | 1 - Genes | 2 - Light | 3 - Mix (default + light)
    bool borders = false;
    float scale = 1.0f;

    sf::Vector2i targetCell {conf::columns / 2, conf::lines / 2}; // Center of the grid
    sf::Vector2f targetPixel  = conf::window_size_f / 2.0f ; // Center of the window

    Render(Simulation & inSimulation): simulation(inSimulation){};

    void draw(sf::RenderWindow &window);
};
#pragma once
#include <SFML/Graphics.hpp>

namespace conf
{
//Window configuration
sf::Vector2u const window_size = {1800, 300};
sf::Vector2f const window_size_f = static_cast<sf::Vector2f>(window_size);

constexpr int max_allowed_framerate = 300;

// Grid configuration
constexpr int lines = 50;
constexpr int columns = 300;

sf::Vector2f const cell_size { window_size_f.x / columns, window_size_f.y / lines };
constexpr float grid_thickness = 1.0f;

const float sensitivity = 1.1f; // Mouse wheel sensitivity for zooming

// Simulation
constexpr int startSeedsNumber = 30;

constexpr float lightThroughLayer = 0.7;
constexpr float lightIncrease = 0.2f;
constexpr int light = 50;

constexpr int energy_cunsumption = 15;
constexpr int newCellCost = 30;
constexpr int startEnergy = 100;

constexpr int lifetime = lines;

constexpr int genomeLength = 20;
constexpr int mutationRate = 25;
};
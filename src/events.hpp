#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "configuration.hpp"
#include "render.hpp"

void processEvents(sf::Window & window, Render & render, uint32_t & max_framerate, bool & display, bool & running);
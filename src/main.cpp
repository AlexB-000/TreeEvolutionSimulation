#include <SFML/Graphics.hpp>
#include <iostream>
#include "configuration.hpp"
#include "events.hpp"
#include "simulation.hpp"
#include "render.hpp"

int main(){
    uint32_t max_framerate = 10;

    auto window = sf::RenderWindow{sf::VideoMode({conf::window_size.x, conf::window_size.y}), "Trees", sf::Style::Close};
    window.setFramerateLimit(max_framerate);

    Simulation simulation;
    Render render(simulation);

    simulation.throwSeeds(conf::startSeedsNumber);

    bool running = false;
    bool display = true;
    
    while (window.isOpen())
    {
        processEvents(window, render, max_framerate, display, running);
        
        if (running) simulation.step();

        // rendering
        if (display){
            window.clear();

            render.draw(window);

            window.display();
        }
    }
}
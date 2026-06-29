#include "events.hpp"

void processEvents(sf::Window & window, Render & render, uint32_t & max_framerate, bool & display, bool & running){
    while (const std::optional event = window.pollEvent()) {   
        if (event->is<sf::Event::Closed>()){
            window.close();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if (keyPressed->code == sf::Keyboard::Key::Escape)
                window.close();

            else if (keyPressed->code == sf::Keyboard::Key::Space)
                running = !running;

            else if (keyPressed->code == sf::Keyboard::Key::F) // freeze
                display = !display;

            else if (keyPressed->code == sf::Keyboard::Key::M)
                render.renderMode = (render.renderMode+1) % render.modes;

            else if (keyPressed->code == sf::Keyboard::Key::L)
                render.renderMode = (render.renderMode-1 + render.modes) % render.modes;
                    
            else if (keyPressed->code == sf::Keyboard::Key::G) // show grid
                render.borders = !render.borders;

        }
        // else if (const auto& mouseWheelScroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
        //     // get the local mouse position (relative to the window)
        //     sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        //     // convert to grid coordinates            
        //     int i = (localPosition.y - render.targetPixel.y) / (conf::cell_size.y * render.scale) + render.targetCell.y;
        //     int j = (localPosition.x - render.targetPixel.x) / (conf::cell_size.x * render.scale) + render.targetCell.x;

        //     // Adjust the camera shift based on the mouse position
        //     render.targetCell = {j, i};
        //     render.targetPixel  = sf::Vector2f{localPosition};

        //     // Zoom in or out
        //     render.scale *= pow(conf::sensitivity, mouseWheelScroll->delta);
        //     if (render.scale <= 0) render.scale = 1;
        // }
    }            

    // Move events
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {
            // if shift is pressed, increase the speed of the program
            max_framerate *= 1.1;
            max_framerate++;
            
            if (max_framerate > conf::max_allowed_framerate)
                max_framerate = conf::max_allowed_framerate;

            window.setFramerateLimit(max_framerate);
            std::cout << "max fps : " << max_framerate << std::endl;
        }
        // else render.targetCell.y -= 1 + static_cast<int>(1/render.scale); // move up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {
            // if shift is pressed, decrease the speed of the program
            max_framerate /= 1.1;
            if (!max_framerate) max_framerate = 1;
            window.setFramerateLimit(max_framerate);
            std::cout << "max fps : " << max_framerate << std::endl;
        }
        // else render.targetCell.y += 1 + static_cast<int>(1/render.scale); // move down
    }
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) render.targetCell.x -= 1 + static_cast<int>(1/render.scale); // move left
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) render.targetCell.x += 1 + static_cast<int>(1/render.scale); // move right
}
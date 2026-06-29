#include "render.hpp"

void Render::draw(sf::RenderWindow &window)
{
    sf::RectangleShape shape{conf::cell_size};

    shape.setScale({scale, scale});

    for (int32_t i = 0; i < conf::lines; i++)
    {
        for (int32_t j = 0; j < conf::columns; j++)
        {
            const short cell = simulation.world[i][j].type;

            sf::Vector2f const position{
                (j - targetCell.x) * conf::cell_size.x * scale + targetPixel.x,
                (i - targetCell.y) * conf::cell_size.y * scale + targetPixel.y};

            shape.setPosition(position);
            
            if (renderMode == 2){   // Light
                if (!cell) shape.setFillColor(sf::Color::Black);
                else{
                    float l = static_cast<float> (simulation.light[i][j]) / conf::light;
                    int r = l * 255;
                    int g = l * 255;
                    int b = (1-l) * 255;
                    shape.setFillColor(sf::Color(r, g, b));
                }
            } else if (renderMode == 3){    // Mix
                float l = static_cast<float> (simulation.light[i][j]) / conf::light;
                int r, g, b;
                // Set the color based on the cell type
                switch (cell) {
                    case 0: // dead
                        r = 0;
                        g = l * 125;
                        b = 255;
                        shape.setFillColor(sf::Color(r, g, b));
                        break;
                    case 1: // sprout
                        shape.setFillColor(sf::Color::White);
                        break;
                    case 2: // leaf
                        r = 0;
                        g = l * 100 + 155;
                        b = 0;
                        shape.setFillColor(sf::Color(r, g, b));
                        break;
                    case 3: // seed
                        shape.setFillColor(sf::Color( 200, 125, 0));
                        break;
                    default:
                        shape.setFillColor(sf::Color::Red); // default color for unknown types
                }
            } else {
                // Set the color based on the cell type
                switch (cell) {
                    case 0: // dead
                        shape.setFillColor(sf::Color::Black);
                        break;
                    case 1: // sprout
                        shape.setFillColor(sf::Color::White);
                        break;
                    case 2: // leaf
                        if (renderMode)
                        shape.setFillColor(simulation.trees[simulation.world[i][j].treeId].color);
                        else
                        shape.setFillColor(sf::Color::Green);
                        break;
                    case 3: // seed
                        shape.setFillColor(sf::Color::Yellow);
                        break;
                    default:
                        shape.setFillColor(sf::Color::Red); // default color for unknown types
                }
            }
            
            window.draw(shape);
        }
    }

    if (borders){
        auto line = sf::RectangleShape{ sf::Vector2f(conf::cell_size.x * conf::columns * scale, conf::grid_thickness*scale) };
        line.setFillColor(sf::Color(100, 100, 100));
        for (int i = 0; i < conf::lines; i++){
            line.setPosition({0, i * conf::cell_size.y * scale + 0});
            window.draw(line);
        }
        line = sf::RectangleShape{ sf::Vector2f(conf::grid_thickness*scale, conf::cell_size.y * conf::lines * scale) };
        line.setFillColor(sf::Color(100, 100, 100));
        for (int i = 0; i < conf::columns; i++){
            line.setPosition({0 + i * conf::cell_size.x * scale, 0});
            window.draw(line);
        }
    }
}
#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <mutex>
#include <utility>
#include "cv_state.h"
#include "screen.h"

// Templatize this so that it works with a keyboard or a controller
template <int height, int width>
struct GameOutput
{
    std::mutex draw_m;
    sf::RenderWindow window{sf::VideoMode(height, width), "Tetris"};
    sf::Image image;
    sf::Image temp_image;
    sf::Texture texture;
    sf::Sprite sprite;

    GameOutput() {
        texture.create(height, width);
        image.create(height, width, sf::Color::White);
        temp_image.create(height, width, sf::Color::White);
        texture.update(image);
        sprite.setTexture(texture);
    }

    // Run this asynchronously
    void update(TetrisScreen<height, width> const &screen)
    {
        std::cout << "Updating window" << std::endl;
        {
            std::lock_guard<std::mutex>{draw_m};
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++) {
                    if (screen.buffer[i][j]) {
                        temp_image.setPixel(j, i, sf::Color::Black);
                    }
                    else {
                        temp_image.setPixel(j, i, sf::Color::White);
                    }
                }
            }
            std::cout << "Window updated" << std::endl;
            std::swap(image, temp_image);

            texture.update(image);
            sprite.setTexture(texture);
        }
    }
    void loop()
    {
        std::cout << "Drawing window" << std::endl;
        while (window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            //std::cout << "Drawing sprite" << std::endl;
            {
                std::lock_guard<std::mutex>{draw_m};
                window.draw(sprite);
                window.display();
            }
        }
    }
};

#endif
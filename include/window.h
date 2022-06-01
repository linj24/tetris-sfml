#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "screen.h"

// Templatize this so that it works with a keyboard or a controller
template <int height, int width>
struct GameOutput
{
    sf::RenderWindow window{sf::VideoMode(height, width), "Tetris"};
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::CircleShape shape{100.f};

    GameOutput() {
        shape.setFillColor(sf::Color::Green);
        texture.create(height, width);
        image.create(height, width, sf::Color::White);
        texture.update(image);
        sprite.setTexture(texture);
    }

    // Run this asynchronously
    void update(TetrisScreen<height, width> const &screen)
    {
        std::cout << "Updating window" << std::endl;
        //for (int i = 0; i < height; i++)
        //{
        //    for (int j = 0; j < width; j++) {
        //        std::cout << "i, j: " << i << ", " << j << std::endl;
        //        if (screen.buffer[i][j]) {
        //            image.setPixel(j, i, sf::Color::Black);
        //        }
        //        else {
        //            image.setPixel(j, i, sf::Color::White);
        //        }
        //    }
        //}
        texture.update(image);
        sprite.setTexture(texture);
    }

    void loop()
    {
        //sf::CircleShape shape(100.f);
        //shape.setFillColor(sf::Color::Green);

        //while (window.isOpen())
        //{   
            //sf::Event event;
            //while (window.pollEvent(event))
            //{
                //if (event.type == sf::Event::Closed)
                    //window.close();
            //}

            //window.clear();
            //window.draw(shape);
            //window.display();
        //}   

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
            std::cout << "Drawing sprite" << std::endl;
            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
};

#endif
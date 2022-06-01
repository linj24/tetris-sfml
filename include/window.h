#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Window.hpp>
#include "screen.h"

// Templatize this so that it works with a keyboard or a controller
template <int height, int width>
struct GameOutput
{
    sf::Window window{sf::VideoMode(800, 600), "Tetris"};

    // Run this asynchronously
    void update(Screen<height, width> const &screen)
    {
        return;
    }

    void loop()
    {
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
        }
    }
};

#endif
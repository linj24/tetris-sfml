#ifndef TETRIS_KEYBOARD_H
#define TETRIS_KEYBOARD_H

#include <map>
#include "SFML/Window/Keyboard.hpp"

// Templatize this so that it works with a keyboard or a controller
struct GameInput
{
    int pressCounter = 0;
    sf::Keyboard::Key lastKey;
    std::map<sf::Keyboard::Key, int> keyMap = {{sf::Keyboard::Space, 32},
                                               {sf::Keyboard::A, 65},
                                               {sf::Keyboard::D, 68},
                                               {sf::Keyboard::N, 78},
                                               {sf::Keyboard::S, 83},
                                               {sf::Keyboard::W, 87},
                                               {sf::Keyboard::Y, 89},
                                               {sf::Keyboard::Left, 130},
                                               {sf::Keyboard::Up, 131},
                                               {sf::Keyboard::Right, 132},
                                               {sf::Keyboard::Down, 133}};

    int getInput()
    {
        int currentKey;
        if (sf::Keyboard::isKeyPressed(lastKey))
        {
            pressCounter++;
            currentKey = keyMap[lastKey];
        }
        else
        {
            pressCounter = 1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                currentKey = 32;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) | sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                currentKey = 131;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) | sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                currentKey = 133;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) | sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                currentKey = 130;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) | sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                currentKey = 132;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            {
                currentKey = 78;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
            {
                currentKey = 89;
            }
        }
        return currentKey;
    }
};

#endif
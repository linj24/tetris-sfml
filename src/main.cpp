#include "game.h"
#include <iostream>
#include <string>

int main()
{
    std::cout << "Please select your screen size (enter a number): " << std::endl;
    std::cout << "1: 400x300" << std::endl;
    std::cout << "2: 800x600" << std::endl;
    std::cout << "3: 1200x900" << std::endl;

    std::string choice_str;
    int choice = 0;
    while (!choice)
    {
        try
        {
            std::cin >> choice_str;
            choice = std::stoi(choice_str);
            if (choice > 3 || choice < 1)
            {
                std::cout << "Invalid choice. Please try again." << std::endl;
                choice = 0;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Not a number. Please try again." << std::endl;
        }
    }

    if (choice == 1)
    {
        Game<400, 300> game{};
        game.run();
    }
    else if (choice == 2)
    {
        Game<800, 600> game{};
        game.run();
    }
    else if (choice == 3)
    {
        Game<1200, 900> game{};
        game.run();
    }
}
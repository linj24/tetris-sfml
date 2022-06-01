#include "game.h"
#include <iostream>
#include <X11/Xlib.h>

int main()
{
    XInitThreads();
    // std::cout << "Please select your screen size: " << endl;
    Game<800, 600> game{};
    std::cout << "Game run start" << std::endl;
    game.run();
}
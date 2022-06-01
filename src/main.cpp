#include "tetris.h"

int main()
{
    Tetris<800, 600> game{1, 2, 3};
    game.run();
}
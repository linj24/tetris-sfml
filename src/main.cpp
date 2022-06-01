#include "tetris.h"

int main()
{
    Tetris game = Tetris<800, 600>(1, 2, 3);
    game.run();
}
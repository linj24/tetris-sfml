This is a simple implementation of Tetris using the Simple and Fast Multimedia
Library (SFML). You can build the
game by running the provided Makefile from the root directory.

You can download SFML at (www.sfml-dev.org). If running on a non-Linux platform,
you may need to comment out lines 4 and 8 (X11 dependencies) before building.

This version of Tetris, much like the original, does not support hold pieces or wall kicking.
Pieces rotate around a block specified as the center of rotation; since wall kicking is not supported, there must be sufficient space for a piece to be rotated FROM ITS CURRENT POSITION.
You level up with every 10 lines you clear; leveling up will increase the rate at which tetriminos fall.

The controls of the game are as follows:

LEFT/A:  move left
RIGHT/D: move right
DOWN/S:  move down
UP/W:    rotate
SPACE:   hard drop

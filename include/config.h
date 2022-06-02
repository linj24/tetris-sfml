#ifndef TETRIS_CONFIG_H
#define TETRIS_CONFIG_H

int constexpr FPS = 30;

int constexpr MOVE_TIME = 5;
int constexpr GRAVITY_TIME = 90;
int constexpr LOCK_TIME = 90;
int constexpr PLAYFIELD_WIDTH = 10;
int constexpr PLAYFIELD_HEIGHT = 22;
int constexpr SPAWN_X = 5;
int constexpr SPAWN_Y = 2;

// Percent width/height
int constexpr X_SHIFT = 25;
int constexpr Y_SHIFT = 0;

// Tile size scales with width
int constexpr TILE_SIZE = 5;

#endif
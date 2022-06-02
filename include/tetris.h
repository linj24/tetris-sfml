#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <random>
#include <vector>
#include "background.h"
#include "playfield.h"
#include "tetrimino.h"
#include "keyboard.h"
#include "window.h"

int constexpr PLAYFIELD_WIDTH = 10;
int constexpr PLAYFIELD_HEIGHT = 22;

std::random_device RNG_DEV;
std::mt19937 RNG(RNG_DEV());
std::uniform_int_distribution<std::mt19937::result_type> DIST_7(0, 6);

template <int height, int width>
struct Tetris
{
public:
    TetrisScreen<height, width> screen;
    Playfield<PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH> playfield;
    Tetrimino piece;
    int level;
    int score;
    int moveCounter;
    int gravityCounter;
    int lockCounter;
    int moveTime;
    int gravityTime;
    int lockTime;
    int seed;
    int lastKey;
    int tileSize;
    int xShift;
    int yShift;
    int spawnX;
    int spawnY;
    bool running;
    bool waitMenu;


    Tetris(int mTime, int gTime, int lTime) : moveTime{mTime}, gravityTime{gTime}, lockTime{lTime}
    {

        /* This is where all the constants are being defined, but since they
         * aren't really constants internally I'm keeping them with lowercase
         * variable names.
         */
        tileSize = 11;
        xShift = 172;
        yShift = 4;
        playfield = Playfield<PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH>();
        // each tetrimino will spawn with its center of rotation on row 2, col 5
        spawnX = 5;
        spawnY = 2;
        piece = Tetrimino(TETRIMINO_MAP[DIST_7(RNG)], 0, spawnX, spawnY);
        screen = TetrisScreen<height, width>(xShift, yShift, tileSize);
        level = 1;
        score = 0;
        moveCounter = 0;
        gravityCounter = 0;
        lockCounter = 0;
        lastKey = 0;
        running = true;
        waitMenu = false;
        std::cout << "Init Tetris" << std::endl;
    }

    void readKeystroke(int key)
    {
        /* Read in the most recent key input and perform the associated action.
         */
        bool keyReleased;
        bool keyPressed;

        if (key == 0)
        {
            if (!(lastKey == 0))
            {
                keyReleased = true;
            }
            else
            {
                keyReleased = false;
            }
            moveCounter = 0;
        }
        else
        {
            if (lastKey == 0)
            {
                keyPressed = true;
            }
            else
            {
                keyPressed = false;
            }
            moveCounter = moveCounter + 1;
        }

        if (keyPressed)
        {
            // spacebar
            if (key == 32)
            {
                hardDrop();
            }
            // up arrow, W
            if ((key == 131) | (key == 87))
            {
                tryRotation();
            }
            // the game over menu
            if (waitMenu)
            {
                // N key
                if (key == 78)
                {
                    handleQuit();
                }
                // Y key
                if (key == 89)
                {
                    waitMenu = false;
                }
            }
            handleMove(key);
        }

        if (moveCounter > moveTime)
        {
            handleMove(key);
            moveCounter = 0;
        }

        lastKey = key;

        return;
    }

    void handleQuit()
    {
        /* Quit the game by ending the program.
         */
        running = false;
        waitMenu = false;
    }

    void handleMove(int key)
    {
        /* Move the current tetrimino if it does not result in a collision.
         */
        // left arrow, A
        if ((key == 130) | (key == 65))
        {
            tryMove(-1, 0);
        }

        // right arrow, D
        if ((key == 132) | (key == 68))
        {
            tryMove(1, 0);
        }

        // down arrow, S
        if ((key == 133) | (key == 83))
        {
            tryMove(0, 1);
        }
    }
    void tryRotation()
    {
        /* Rotate a piece if it does not result in a collision.
         */

        int currRotation = piece.getRotation();
        int nextRotation = currRotation + 1;
        // crude form of modulo
        if (nextRotation > 3)
        {
            nextRotation = nextRotation - 4;
        }

        // use a new dummy tetrimino to test for the collision
        Tetrimino rotated = piece;
        rotated.rotate(1);

        if (playfield.noCollision(rotated))
        {
            screen.drawTetrimino(piece, false);
            piece.rotate(1);
            screen.drawTetrimino(piece, true);
        }

        return;
    }

    bool hardDrop()
    {
        /* Perform a hard drop on the current tetrimino.
         */
        bool dropping = true;
        while (dropping)
        {
            dropping = tryMove(0, 1);
        }
        // immediately lock a dropped tetrimino
        lockCounter = lockTime;
        return true;
    }

    // return false if no collisions are found and true if a collision is detected
    bool tryMove(int xOffset, int yOffset)
    {
        /* Check for collisions in a proposed tetrimino movement.
         */
        int pieceX = piece.getX();
        int pieceY = piece.getY();
        if (playfield.noCollision(piece, pieceX + xOffset, pieceY + yOffset))
        {
            screen.drawTetrimino(piece, false);
            piece.move(xOffset, yOffset);
            screen.drawTetrimino(piece, true);
            return true;
        }
        return false;
    }


    void lockTetrimino()
    {
        /* Lock a tetrimino to the playing field and spawn a new one.
         */
        bool lockSuccess = playfield.lock(piece);
        piece = Tetrimino(TETRIMINO_MAP[DIST_7(RNG)], 0, spawnX, spawnY);
        if ((!playfield.noCollision(piece, spawnX, spawnY)) | (!lockSuccess))
        {
            // end game if newly spawned piece has a collision
            waitMenu = true;
        }
        return;
    }

    void updateScore(int linesCleared)
    {
        /* Update the player's current score and level.
         */
        int lastLevel = level;
        // Score is equivalent to the number of lines clared
        score = score + linesCleared;
        // Level increases by 1 every 10 lines you clear
        level = (score / 10) + 1;
        // With each level increase, decrease the time for a tetrimino to fall
        if (~(lastLevel == level))
        {
            gravityTime = gravityTime * 9 / 10;
        }
    }

    void init()
    {
        std::cout << "Drawing grid lines" << std::endl;
        screen.drawGridLines(playfield);
        std::cout << "Drawing tetrimino" << std::endl;
        // draw the first piece on the first frame since we only redraw on move
        screen.drawTetrimino(piece, true);
        std::cout << "Done with drawing init" << std::endl;
    }

    void update(int key)
    {
        std::cout << "Updating " << key << std::endl;
        std::cout << "Waitmenu " << waitMenu << std::endl;

        if (waitMenu) {
            endGame(key);
        }
        else {
            gameStep(key);
        }
    }

    void gameStep(int key)
    {
        /* Run the main game loop.
         */

        std::cout << "Running!" << std::endl;
        screen.drawGridLines(playfield);
        int pieceX = piece.getX();
        int pieceY = piece.getY();

        if (!playfield.noCollision(piece, pieceX, pieceY + 1))
        {
            lockCounter = lockCounter + 1;
            if (lockCounter > lockTime)
            {
                lockTetrimino();
                int linesCleared = playfield.clearLines();
                updateScore(linesCleared);
                lockCounter = 0;
                gravityCounter = 0;
                moveCounter = 0;
                screen.drawPlayfield(playfield);
                screen.drawTetrimino(piece, true);
            }
        }
        else
        {
            lockCounter = 0;
        }

        readKeystroke(key);

        if (gravityCounter > gravityTime)
        {
            tryMove(0, 1);
            gravityCounter = 0;
        }

        moveCounter += 1;
        gravityCounter += 1;
        std::cout << "Gravity " << gravityCounter << std::endl;
    }

    void endGame(int key)
    {
        /* End the game and program.
         */
        readKeystroke(key);
        if (running)
        {
            //reset();
        }
    }

    void reset()
    {
        /* Reset the playing field for a new round.
         */
        std::cout << "Resetting" << std::endl;
        Playfield<PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH> oldPlayfield = std::move(playfield);

        playfield = Playfield<PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH>();
        level = 1;
        score = 0;
        moveCounter = 0;
        gravityCounter = 0;
        lockCounter = 0;
        lastKey = 0;
    }
};

#endif
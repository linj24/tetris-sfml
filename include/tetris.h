#ifndef TETRIS_H
#define TETRIS_H

#include <iostream>
#include <random>
#include <vector>
#include "background.h"
#include "config.h"
#include "cv_state.h"
#include "playfield.h"
#include "tetrimino.h"
#include "keyboard.h"
#include "window.h"


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
    int level = 1;
    int score = 0;
    int moveCounter = 0;
    int gravityCounter = 0;
    int lockCounter = 0;
    int gravityTime = GRAVITY_TIME;
    int lockTime = LOCK_TIME;
    int lastKey = 0;
    bool running = true;
    bool waitMenu = false;

    Tetris(std::shared_ptr<CVState> &cv_state)
    {

        playfield = Playfield<PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH>();
        piece = Tetrimino(TETRIMINO_MAP[DIST_7(RNG)], 0, SPAWN_X, SPAWN_Y);
        screen = TetrisScreen<height, width>(cv_state);
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

        if (moveCounter > MOVE_TIME)
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

        int pieceX = piece.getX();
        int pieceY = piece.getY();

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
        piece = Tetrimino(TETRIMINO_MAP[DIST_7(RNG)], 0, SPAWN_X, SPAWN_Y);
        if ((!playfield.noCollision(piece, SPAWN_X, SPAWN_Y)) || (!lockSuccess))
        {
            // end game if newly spawned piece has a collision
            waitMenu = true;
        }
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
                screen.drawGridLines(playfield);
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
            reset();
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
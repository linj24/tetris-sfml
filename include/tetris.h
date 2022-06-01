#ifndef TETRIS_H
#define TETRIS_H

#include <vector>
#include "background.h"
#include "playfield.h"
#include "tetrimino.h"
#include "keyboard.h"
#include "window.h"

int constexpr PLAYFIELD_WIDTH = 10;
int constexpr PLAYFIELD_HEIGHT = 22;

template <int height, int width>
struct Tetris
{
    Playfield<PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH> playfield;
    Tetrimino piece;
    Screen<height, width> screen;
    GameInput keyboard;
    GameOutput<height, width> window;
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
        seed = 2;
        piece = Tetrimino(seed, 0, spawnX, spawnY);
        screen = Screen<height, width>(xShift, yShift, tileSize);
        level = 1;
        score = 0;
        moveCounter = 0;
        gravityCounter = 0;
        lockCounter = 0;
        lastKey = 0;
        running = true;
        waitMenu = true;
    }

    void readKeystroke()
    {
        /* Read in the most recent key input and perform the associated action.
         */
        int key = keyboard.getInput();
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

        int pieceX = piece.getX();
        int pieceY = piece.getY();
        int pieceID = piece.getID();

        // use a new dummy tetrimino to test for the collision
        Tetrimino rotated = Tetrimino(pieceID, nextRotation, pieceX, pieceY);

        if (playfield.noCollision(pieceX, pieceY, rotated))
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
        if (playfield.noCollision(pieceX + xOffset, pieceY + yOffset, piece))
        {
            screen.drawTetrimino(piece, false);
            piece.move(xOffset, yOffset);
            screen.drawTetrimino(piece, true);
            return true;
        }
        return false;
    }

    int PRNG()
    {
        /* Use prime numbers and modulo to simulate pseudorandom number generation.
         */
        // nextID = X * 11 mod 7
        seed = seed * 11;
        int quotient = seed / 7;
        int nextID = seed - (quotient * 7);
        if (nextID < 0)
        {
            nextID = nextID + 7;
        }
        return nextID;
    }

    void lockTetrimino()
    {
        /* Lock a tetrimino to the playing field and spawn a new one.
         */
        bool lockSuccess = playfield.lock(piece);
        int nextID = PRNG();
        piece = Tetrimino(nextID, 0, spawnX, spawnY);
        if ((!playfield.noCollision(spawnX, spawnY, piece)) | (!lockSuccess))
        {
            // end game if newly spawned piece has a collision
            endGame();
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
        if (~(lastLevel = level))
        {
            gravityTime = gravityTime * 9 / 10;
        }
    }

    void run()
    {
        /* Run the main game loop.
         */

        screen.drawGridLines();
        // draw the first piece on the first frame since we only redraw on move
        screen.drawTetrimino(piece, true);
        while (running)
        {
            screen.drawGridLines();
            int pieceX = piece.getX();
            int pieceY = piece.getY();

            if (!playfield.noCollision(pieceX, pieceY + 1, piece))
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

            readKeystroke();

            if (gravityCounter > gravityTime)
            {
                tryMove(0, 1);
                gravityCounter = 0;
            }

            moveCounter = moveCounter + 1;
            gravityCounter = gravityCounter + 1;
            // TODO: wait
        }
    }

    void endGame()
    {
        /* End the game and program.
         */
        screen.drawGridLines();
        waitMenu = true;
        while (waitMenu)
        {
            readKeystroke();
        }
        if (running)
        {
            reset();
        }
    }

    void reset()
    {
        /* Reset the playing field for a new round.
         */
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
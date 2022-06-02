#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#include <array>
#include <iostream>
#include <memory>
#include "tetrimino.h"

template <int height, int width>
struct Playfield
{
    std::array<std::array<bool, width>, height> playfield{
        std::array<std::array<bool, width>, height>()};

    // return the number of lines cleared for scoring purposes
    int clearLines()
    {
        /* Check the playing field for completed rows and remove them.
         */
        int linesCleared = 0;

        // iterate over # rows
        for (int i = height - 1; i > -1;)
        {
            int colOccupied = 0;
            // count the number of occupied columns in the current row
            for (int j = 0; j < width; j++)
            {
                if (playfield[i][j])
                {
                    colOccupied++;
                }
            }
            if (colOccupied == width)
            {
                linesCleared += 1;
                shiftPlayfield(i);
            }
            else
            {
                // only decrement when no changes made to avoid skipping lines
                i = i - 1;
            }
        }
        return linesCleared;
    }

    void shiftPlayfield(int i)
    {
        /* Shift down the rows of the playing field starting at row i.
         */

        // move all rows above the current one down
        for (int k = i; k > 0; k--)
        {
            playfield[k] = std::move(playfield[k - 1]);
        }

        // replace row of filled blocks with an empty row
        playfield[0] = std::array<bool, width>{};
    }

    // This was going to use partial specialization to operate on an offset of
    // tuples, but I ran out of time. See the "tuple" branch in the GitHub repo for my attempt.
    bool noCollision(Tetrimino &piece) {
        return noCollision(piece, piece.x, piece.y);
    }

    bool noCollision(Tetrimino &piece, int x, int y)
    {
        /* Check if a tetrimino will collide with the playing field if placed
         * in a certain location.
         */
        std::vector<int> xOffsets = piece.getXOffsets();
        std::vector<int> yOffsets = piece.getYOffsets();

        for (int i = 0; i < 4; i++)
        {
            int xPos = x + xOffsets[i];
            int yPos = y + yOffsets[i];
            if ((yPos > (height - 1)) | (yPos < 0) | (xPos > (width - 1)) | (xPos < 0))
            {
                return false;
            }
            if (playfield[yPos][xPos] == 1)
            {
                return false;
            }
        }
        return true;
    }

    bool lock(Tetrimino piece)
    {
        /* Lock a tetrimino to the playing field.
         */
        // return true if piece locked and game has not ended
        bool success;

        int x = piece.getX();
        int y = piece.getY();
        std::vector<int> xOffsets = piece.getXOffsets();
        std::vector<int> yOffsets = piece.getYOffsets();
        success = true;
        for (int i = 0; i < 4; i++)
        {
            int xPos = x + xOffsets[i];
            int yPos = y + yOffsets[i];
            if (yPos < 2)
            {
                success = false;
            }
            playfield[yPos][xPos] = true;
        }
        auto temp = std::move(piece);
        return success;
    }
};

#endif
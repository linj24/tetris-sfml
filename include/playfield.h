#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#include <array>
#include <iostream>
#include <memory>
#include <tuple>
#include "tetrimino.h"

template <int height, int width>
struct Playfield
{
    std::array<std::array<bool, width>, height> playfield{
        std::array<std::array<bool, width>, height>()};

    Playfield() { std::cout << "Init Playfield" << std::endl; }

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

    template<int n>
    struct noCollisionBlock {
        bool operator()(Tetrimino piece, int x, int y) {
            int xPos = x + std::get<n-1>(piece.getXOffsets());
            int yPos = y + std::get<n-1>(piece.getYOffsets());
            if ((yPos > (height - 1)) | (yPos < 0) | (xPos > (width - 1)) | (xPos < 0))
            {
                return false;
            }
            if (playfield[yPos][xPos] == 1)
            {
                return false;
            }
            return noCollisionBlock<n - 1>(piece, x, y);
        }
    };

    template<int n>
    struct noCollisionBlock<0> {
        bool operator()(Tetrimino piece, int x, int y) {
            return true;
        }
    };


//    template<int n>
//    bool noCollisionBlock<0>(Tetrimino piece, int x, int y)
//    {
//        return true;
//    }
//
//    template<int n>
//    bool noCollisionBlock<n>(Tetrimino piece, int x, int y)
//    {
//        int xPos = x + std::get<n-1>(piece.getXOffsets());
//        int yPos = y + std::get<n-1>(piece.getYOffsets());
//        if ((yPos > (height - 1)) | (yPos < 0) | (xPos > (width - 1)) | (xPos < 0))
//        {
//            return false;
//        }
//        if (playfield[yPos][xPos] == 1)
//        {
//            return false;
//        }
//        return noCollisionBlock<n - 1>(piece, x, y);
//    }

    // TODO: Convert this to template code
    bool noCollision(Tetrimino piece)
    {
        return noCollision(piece, piece.getX(), piece.getY());
    }

    bool noCollision(Tetrimino piece, int x, int y)
    {
        /* Check if a tetrimino will collide with the playing field if placed
         * in a certain location.
         */
        noCollisionBlock<4> block{};
        return block(piece, x, y);
    }

    template<int n>
    bool lockBlock<0>(Tetrimino piece)
    {
        return true;
    }

    template<int n>
    bool lockBlock<n>(Tetrimino piece)
    {
        int xPos = piece.getX() + std::get<n-1>(piece.getXOffsets());
        int yPos = piece.getY() + std::get<n-1>(piece.getYOffsets());
        playfield[yPos][xPos] = true;
        return yPos < 2 && lockBlock<n-1>(piece);
    }

    bool lock(Tetrimino piece)
    {
        /* Lock a tetrimino to the playing field.
         */
        // return true if piece locked and game has not ended
        bool success;

        success = lockBlock<4>(piece);
        auto temp = std::move(piece);
        return success;
    }
};

#endif
#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <variant>
#include <vector>

struct OTetrimino
{
    std::vector<int> xOffsets() {
        return {0, 0, -1, -1};
    }

    std::vector<int> yOffsets() {
        return {0, 1, 0, 1};
    }
};

struct ITetrimino
{
    std::vector<int> xOffsets() {
        return {0, -1, -2, 1};
    }

    std::vector<int> yOffsets() {
        return {0, 0, 0, 0};
    }
};

struct TTetrimino
{
    std::vector<int> xOffsets() {
        return {0, 0, -1, 1};
    }

    std::vector<int> yOffsets() {
        return {0, 1, 0, 0};
    }
};

struct STetrimino
{
    std::vector<int> xOffsets() {
        return {0, 1, 0, 1};
    }

    std::vector<int> yOffsets() {
        return {0, 0, 1, -1};
    }
};

struct ZTetrimino
{
    std::vector<int> xOffsets() {
        return {0, -1, 0, 1};
    }

    std::vector<int> yOffsets() {
        return {0, 0, 1, 1};
    }
};

struct JTetrimino
{
    std::vector<int> xOffsets() {
        return {0, -1, 1, 1};
    }

    std::vector<int> yOffsets() {
        return {0, 0, 0, 1};
    }
};

struct LTetrimino
{
    std::vector<int> xOffsets() {
        return {0, 1, -1, -1};
    }

    std::vector<int> yOffsets() {
        return {0, 0, 0, 1};
    }
};

using Shape = std::variant<OTetrimino, ITetrimino, TTetrimino, STetrimino, ZTetrimino, JTetrimino, LTetrimino>;


std::map<int, Shape> TETRIMINO_MAP = {{0, OTetrimino()},
                                      {1, ITetrimino()},
                                      {2, TTetrimino()},
                                      {3, STetrimino()},
                                      {4, ZTetrimino()},
                                      {5, JTetrimino()},
                                      {6, LTetrimino()}};

struct Tetrimino
{
    int x, y;
    int rotation;

    std::vector<int> xOffsets;
    std::vector<int> yOffsets;

    Tetrimino()
    {
    }


    Tetrimino(Shape shape, int init_rotation, int xPos, int yPos) : x{xPos}, y{yPos}, rotation{init_rotation}
    {
        xOffsets = std::visit([](auto &x)
                              { return x.xOffsets(); },
                              shape);
        yOffsets = std::visit([](auto &x)
                              { return x.yOffsets(); },
                              shape);
        rotate(rotation);
    }

    void move(int xOffset, int yOffset)
    {
        /* Move a tetrimino in a given direction.
         */
        x = x + xOffset;
        y = y + yOffset;
    }

    void setPos(int xPos, int yPos)
    {
        /* Move a tetrimino to a specific location.
         */
        x = xPos;
        y = yPos;
    }

    void rotate(int nRotations)
    {
        /* Rotate a tetrimino by 90 degrees counterclockwise n times.
         */

        for (int i = 0; i < nRotations; i++)
        {
            rotation += 1;
            if (rotation > 3)
            {
                // modulo 4
                rotation -= 4;
            }
            std::vector<int> temp = std::move(xOffsets);
            xOffsets = std::move(yOffsets);
            yOffsets = std::move(temp);

            for (int j = 0; j < 4; j++)
            {
                yOffsets[j] = -yOffsets[j];
            }
        }
    }

    int getX()
    {
        /* Get the x-position of the current tetrimino.
         */
        return x;
    }

    int getY()
    {
        /* Get the y-position of the current tetrimino.
         */
        return y;
    }

    std::vector<int> getXOffsets()
    {
        /* Get the x-positions of all the blocks in the tetrimino relative to
         * the center of rotation.
         */
        return xOffsets;
    }

    std::vector<int> getYOffsets()
    {
        /* Get the y-positions of all the blocks in the tetrimino relative to
         * the center of rotation.
         */
        return yOffsets;
    }

    int getRotation()
    {
        /* Get the current rotation of the tetrimino.
         */
        return rotation;
    }
};

// Implement a "tetrimino factory"
#endif
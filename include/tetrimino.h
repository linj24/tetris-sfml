#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <variant>
#include <vector>

struct OTetrimino
{
    std::tuple<int, int, int, int> constexpr xOffsets() {
        return {0, 0, -1, -1};
    }

    std::tuple<int, int, int, int> constexpr yOffsets() {
        return {0, 1, 0, 1};
    }
};

struct ITetrimino
{
    std::tuple<int, int, int, int> constexpr xOffsets() {
        return {0, -1, -2, 1};
    }

    std::tuple<int, int, int, int> constexpr yOffsets() {
        return {0, 0, 0, 0};
    }
};

struct TTetrimino
{
    std::tuple<int, int, int, int> constexpr xOffsets() {
        return {0, 0, -1, 1};
    }

    std::tuple<int, int, int, int> constexpr yOffsets() {
        return {0, 1, 0, 0};
    }
};

struct STetrimino
{
    std::tuple<int, int, int, int> constexpr xOffsets() {
        return {0, 1, 0, 1};
    }

    std::tuple<int, int, int, int> constexpr yOffsets() {
        return {0, 0, 1, -1};
    }
};

struct ZTetrimino
{
    std::tuple<int, int, int, int> constexpr xOffsets() {
        return {0, -1, 0, 1};
    }

    std::tuple<int, int, int, int> constexpr yOffsets() {
        return {0, 0, 1, 1};
    }
};

struct JTetrimino
{
    std::tuple<int, int, int, int> constexpr xOffsets() {
        return {0, -1, 1, 1};
    }

    std::tuple<int, int, int, int> constexpr yOffsets() {
        return {0, 0, 0, 1};
    }
};

struct LTetrimino
{
    std::tuple<int, int, int, int> constexpr xOffsets() {
        return {0, 1, -1, -1};
    }

    std::tuple<int, int, int, int> constexpr yOffsets() {
        return {0, 0, 0, 1};
    }
};

using Shape = std::variant<OTetrimino, ITetrimino, TTetrimino, STetrimino, ZTetrimino, JTetrimino, LTetrimino>;

struct North
{
    std::tuple<int, int, int, int> constexpr xOffsets(Shape s) {
        return std::visit([](auto &x)
                              { return x.xOffsets(); },
                              s);
    }

    std::tuple<int, int, int, int> constexpr yOffsets(Shape s) {
        return std::visit([](auto &x)
                              { return x.yOffsets(); },
                              s);
    }
};

std::map<int, Shape> TETRIMINO_MAP = {{0, OTetrimino()},
                                      {1, ITetrimino()},
                                      {2, TTetrimino()},
                                      {3, STetrimino()},
                                      {4, ZTetrimino()},
                                      {5, JTetrimino()},
                                      {6, LTetrimino()}};

struct Tetrimino
{

    /*
     * 0 = O
     * 1 = I
     * 2 = T
     * 3 = S
     * 4 = Z
     * 5 = J
     * 6 = L
     */

    int x, y;
    int rotation;

    std::tuple<int, int, int, int> xOffsets;
    std::tuple<int, int, int, int> yOffsets;

    Tetrimino()
    {
    }

    // Shape_type type_s;
    // Rotation_type type_r;
    //using Rotation = std::variant<North, South, East, West>;
    //  L_shape, J_shape, define xOffsets that return vectors
    //  Variant
    //  Piece type
    //  ID, xOffsets, yOffsets
    //  std::vector<int> const &xOffsets()
    //  {
    //      // Call this when using xOffsets later
    //      std::visit([](auto &x)
    //                 { return x.xOffsets(); },
    //                 type);
    //  }
    //  Tetrimino(Piece_type type, int init_rotation, int xPos, int yPos) : x{xPos},
    // y{yPos}, id{piece_id}, rotation{init_rotation}
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
            std::tuple<int, int, int, int> temp = std::move(xOffsets);
            xOffsets = std::move(yOffsets);
            yOffsets = std::make_tuple(-std::get<0>(yOffsets), -std::get<1>(yOffsets), -std::get<2>(yOffsets), -std::get<3>(yOffsets));
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

    std::tuple<int, int, int, int> getXOffsets()
    {
        /* Get the x-positions of all the blocks in the tetrimino relative to
         * the center of rotation.
         */
        return xOffsets;
    }

    std::tuple<int, int, int, int> getYOffsets()
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
#ifndef TETRIMINO_H
#define TETRIMINO_H

#include <vector>

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

    int id;
    int x, y;
    int rotation;

    std::vector<int> xOffsets;
    std::vector<int> yOffsets;

    Tetrimino()
    {
    }

    // Shape_type type_s;
    // Rotation_type type_r;
    // using Shape_type = variant<L_shape, J_shape, O_shape...>;
    // using Rotation_type = variant<North, South, East, West>;
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
    Tetrimino(int piece_id, int init_rotation, int xPos, int yPos) : x{xPos}, y{yPos}, id{piece_id}, rotation{init_rotation}
    {
        // O tetrimino
        if (id == 0)
        {
            xOffsets = {0, 0, -1, -1};
            yOffsets = {0, 1, 0, 1};
        }
        // I tetrimino
        else if (id == 1)
        {
            xOffsets = {0, -1, -2, 1};
            yOffsets = {0, 0, 0, 0};
        }
        // T tetrimino
        else if (id == 2)
        {
            xOffsets = {0, 0, -1, 1};
            yOffsets = {0, 1, 0, 0};
        }
        // S tetrimino
        else if (id == 3)
        {
            xOffsets = {0, 1, 0, 1};
            yOffsets = {0, 0, 1, -1};
        }
        // Z tetrimino
        else if (id == 4)
        {
            xOffsets = {0, -1, 0, 1};
            yOffsets = {0, 0, 1, 1};
        }
        // J tetrimino
        else if (id == 5)
        {
            xOffsets = {0, -1, 1, 1};
            yOffsets = {0, 0, 0, 1};
        }
        // L tetrimino
        else if (id == 6)
        {
            xOffsets = {0, 1, -1, -1};
            yOffsets = {0, 0, 0, 1};
        }
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

    int getID()
    {
        /* Get the ID of the current tetrimino.
         */
        return id;
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
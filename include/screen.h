#ifndef TETRIS_SCREEN_H
#define TETRIS_SCREEN_H
#include <array>
#include <iostream>
#include <memory>
#include "playfield.h"

int constexpr INSTRUCTION_START_ROW = 5;
int constexpr INSTRUCTION_START_COL = 1;
int constexpr GAME_OVER_X1 = 364;
int constexpr GAME_OVER_X2 = 504;
int constexpr GAME_OVER_Y1 = 88;
int constexpr GAME_OVER_Y2 = 160;
int constexpr GAME_OVER_START_COL = 47;
int constexpr GAME_OVER_START_ROW = 9;
int constexpr SCORE_COL = 25;

// Templatize to scale xShift, yShift, tileSize with screen size
template <int height, int width>
struct TetrisScreen
{
    // Use condition variables to update by sending messages to the rendering thread
    std::array<std::array<bool, width>, height> buffer{
        std::array<std::array<bool, width>, height>()};
    int tileSize;
    int xShift;
    int yShift;
    bool mode = true;

    TetrisScreen() {}
    TetrisScreen(int xShft, int yShft, int tSize) : xShift{xShft}, yShift{yShft}, tileSize{tSize} {
        std::cout << "Init TetrisScreen" << std::endl;
    }

    void setColor(bool color)
    {
        mode = color;
    }

    void clear()
    {
        auto temp = std::move(buffer);
        buffer = std::array<std::array<bool, width>, height>();
    }

    // TODO: use compile-time check to make sure drawn screen fits within dimensions
    void drawLine(int x0, int y0, int x1, int y1)
    {
        // Bresenham's algorithm
        int dx = x1 - x0;
        int dy = y1 - y0;
        int D = 2 * dy - dx;
        int y = y0;
        std::cout << "Buffer size: " << buffer[0].size() << std::endl;

        for (int x = x0; x <= x1; x++)
        {
            buffer[y][x] = mode;
            if (D > 0)
            {
                y = y + 1;
                D = D - 2 * dx;
            }
            D = D + 2 * dy;
            std::cout << "x0, x1, y0, y1: " << x0 << ", " << x1 << ", " << y0 << ", " << y1 << std::endl;
            std::cout << "x, y: " << x << ", " << y << std::endl;
        }
        std::cout << "Buffer drawn: " << buffer[0].size() << std::endl;
    }

    void drawRectangle(int x1, int y1, int x2, int y2)
    {
        for (int i = y1; i <= y2; i++)
        {
            for (int j = x1; j <= x2; j++)
            {
                buffer[i][j] = mode;
            }
        }
    }

    template <int p_height, int p_width>
    void drawPlayfield(Playfield<p_height, p_width> const &playfield)
    {
        /* Draw the playing field.
         */

        // skip the first two lines
        for (int i = 2; i < p_height; i++)
        {
            for (int j = 0; j < p_width; j++)
            {
                int x1 = xShift + (j * tileSize);
                int x2 = xShift + ((j + 1) * tileSize);
                int y1 = yShift + (i * tileSize);
                int y2 = yShift + ((i + 1) * tileSize);
                if (playfield.playfield[i][j] == 1)
                {
                    setColor(true);
                    drawRectangle(x1, y1, x2, y2);
                }
                else
                {
                    setColor(false);
                    drawRectangle(x1, y1, x2, y2);
                }
            }
        }

        return;
    }

    void drawTetrimino(Tetrimino const &tetrimino, bool color)
    {
        /* Draw the current tetrimino.
         */

        // true to draw; false to erase
        setColor(color);

        for (int i = 0; i < 4; i++)
        {
            // hide the first two rows (buffer to allow rotation on spawn row)
            if ((tetrimino.y + tetrimino.yOffsets[i]) > 1)
            {
                int x1 = xShift + ((tetrimino.x + tetrimino.xOffsets[i]) * tileSize);
                int x2 = xShift + ((tetrimino.x + tetrimino.xOffsets[i] + 1) * tileSize);
                int y1 = yShift + ((tetrimino.y + tetrimino.yOffsets[i]) * tileSize);
                int y2 = yShift + ((tetrimino.y + tetrimino.yOffsets[i] + 1) * tileSize);
                drawRectangle(x1, y1, x2, y2);
            }
        }
    }

    template <int p_height, int p_width>
    void drawGridLines(Playfield<p_height, p_width> const &playfield)
    {
        /* Draw the grid lines for the playing field.
         */

        // hide the top two rows
        int i = 2;
        // draw horizontal lines
        std::cout << "Drawing horizontal lines" << std::endl;
        for (int i = 2; i < (p_height + 1); i++)
        {
            int x1 = xShift;
            int x2 = xShift + (p_width * tileSize);
            int y1 = yShift + (i * tileSize);
            int y2 = yShift + (i * tileSize);
            drawLine(x1, y1, x2, y2);
        }
        std::cout << "Drawing vertical lines" << std::endl;
        // draw vertical lines
        for (int i = 0; i < (p_width + 1); i++)
        {
            int x1 = xShift + (i * tileSize);
            int x2 = xShift + (i * tileSize);
            // hide the first two rows, so start drawing at row 2
            int y1 = yShift + (2 * tileSize);
            int y2 = yShift + (p_height * tileSize);
            drawLine(x1, y1, x2, y2);
        }
    }
};
#endif
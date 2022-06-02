#ifndef TETRIS_SCREEN_H
#define TETRIS_SCREEN_H
#include <array>
#include <iostream>
#include <memory>
#include "config.h"
#include "cv_state.h"
#include "playfield.h"


template <int height, int width>
struct TetrisScreen
{
    // Use condition variables to update by sending messages to the rendering thread
    std::array<std::array<bool, width>, height> buffer{
        std::array<std::array<bool, width>, height>()};
    std::shared_ptr<CVState> cv_state;
    int xShift = X_SHIFT * width / 100;
    int yShift = Y_SHIFT * height / 100;
    int tileSize = TILE_SIZE * width / 100;

    TetrisScreen() {}
    TetrisScreen(std::shared_ptr<CVState> &cv_s) : cv_state{cv_s} {
    }

    void render()
    {
        {
            std::lock_guard lck(cv_state->m);
            cv_state->flag = true;
        }
        cv_state->cv.notify_all();
    }


    void clear()
    {
        auto temp = std::move(buffer);
        buffer = std::array<std::array<bool, width>, height>();
    }

    // TODO: use compile-time check to make sure drawn screen fits within
    // dimensions
    // TODO: Templatize this for vertical/horizontal lines
    void drawLine(int x0, int y0, int x1, int y1, bool color)
    {
        if (x0 == x1) {
            for (int y = y0; y <= y1; y++)
            {
                buffer[y][x0] = color;

            }
            return;
        }
        else if (y0 == y1) {
            for (int x = x0; x <= x1; x++)
            {
                buffer[y0][x] = color;
            }
            return;
        }
        // Bresenham's algorithm
        int dx = x1 - x0;
        int dy = y1 - y0;
        int D = 2 * dy - dx;
        int y = y0;

        for (int x = x0; x <= x1; x++)
        {
            buffer[y][x] = color;
            if (D > 0)
            {
                y = y + 1;
                D = D - 2 * dx;
            }
            D = D + 2 * dy;
        }
    }

    void drawRectangle(int x1, int y1, int x2, int y2, bool color)
    {
        for (int i = y1; i <= y2; i++)
        {
            for (int j = x1; j <= x2; j++)
            {
                buffer[i][j] = color;
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
                    drawRectangle(x1, y1, x2, y2, true);
                }
                else
                {
                    drawRectangle(x1, y1, x2, y2, false);
                }
            }
        }
        // Wake the rendering thread
        render();
    }

    void drawTetrimino(Tetrimino const &tetrimino, bool color)
    {
        /* Draw the current tetrimino.
         */

        // true to draw; false to erase

        for (int i = 0; i < 4; i++)
        {
            // hide the first two rows (buffer to allow rotation on spawn row)
            if ((tetrimino.y + tetrimino.yOffsets[i]) > 1)
            {
                int x1 = xShift + ((tetrimino.x + tetrimino.xOffsets[i]) * tileSize);
                int x2 = xShift + ((tetrimino.x + tetrimino.xOffsets[i] + 1) * tileSize);
                int y1 = yShift + ((tetrimino.y + tetrimino.yOffsets[i]) * tileSize);
                int y2 = yShift + ((tetrimino.y + tetrimino.yOffsets[i] + 1) * tileSize);
                if (!color) {
                    x1 += 1;
                    x2 -= 1;
                    y1 += 1;
                    y2 -= 1;
                }
                drawRectangle(x1, y1, x2, y2, color);
            }
        }
        render();
    }

    template <int p_height, int p_width>
    void drawGridLines(Playfield<p_height, p_width> const &playfield)
    {
        /* Draw the grid lines for the playing field.
         */

        // hide the top two rows
        int i = 2;
        // draw horizontal lines
        for (int i = 2; i < (p_height + 1); i++)
        {
            int x1 = xShift;
            int x2 = xShift + (p_width * tileSize);
            int y1 = yShift + (i * tileSize);
            int y2 = yShift + (i * tileSize);
            drawLine(x1, y1, x2, y2, true);
        }
        // draw vertical lines
        for (int i = 0; i < (p_width + 1); i++)
        {
            int x1 = xShift + (i * tileSize);
            int x2 = xShift + (i * tileSize);
            // hide the first two rows, so start drawing at row 2
            int y1 = yShift + (2 * tileSize);
            int y2 = yShift + (p_height * tileSize);
            drawLine(x1, y1, x2, y2, true);
        }
        render();
    }
};
#endif
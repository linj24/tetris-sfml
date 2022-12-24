#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <initializer_list>
#include <thread>
#include "config.h"
#include "cv_state.h"
#include "keyboard.h"
#include "window.h"
#include "tetris.h"

auto SLEEP_TIME = std::chrono::duration<double>(1.0 / FPS);

template <int height, int width>
struct Game
{

    std::shared_ptr<CVState> cv_state{std::make_shared<CVState>()};
    GameInput keyboard{};
    GameOutput<height, width> window{};
    Tetris<height, width> tetris{cv_state};

    Game()
    {
        tetris.init();
    }

    void game_loop()
    {
        while (window.window.isOpen())
        {
            int key = keyboard.getInput();
            tetris.update(key);
            std::this_thread::sleep_for(SLEEP_TIME);
        }
    }

    void render_loop()
    {
        while (window.window.isOpen())
        {
            std::shared_lock lck{cv_state->m};
            cv_state->cv.wait(lck, [&]
                              { return cv_state->flag; });
            window.update(tetris.screen);
            cv_state->flag = false;
        }
    }

    void run()
    {
        // Spawn threads for rendering/game logic
        std::thread logicThread(&Game::game_loop, this);
        std::thread renderThread(&Game::render_loop, this);
        window.loop();
        logicThread.join();
        renderThread.join();
    }
};

#endif
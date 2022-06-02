#include <chrono>
#include <initializer_list>
#include <thread>
#include "cv_state.h"
#include "keyboard.h"
#include "window.h"
#include "tetris.h"

using namespace std::chrono_literals;
// This is where we decide on the screen size


template <int height, int width>
struct Game {

    std::shared_ptr<CVState> cv_state{std::make_shared<CVState>()};
    GameInput keyboard{};
    GameOutput<height, width> window{};
    Tetris<height, width> tetris{cv_state};

    Game() {
        std::cout << "Init Game" << std::endl;
        tetris.init();
        std::cout << "Init Game Done" << std::endl;
    }

    void game_loop() {
        while (window.window.isOpen()) {
            int key = keyboard.getInput();
            tetris.update(key);
            std::this_thread::sleep_for(1ms);
        }
    }

    void render_loop() {
        while (window.window.isOpen()) {
            std::shared_lock lck{cv_state->m};
            cv_state->cv.wait(lck, [&]
                              { return cv_state->flag; });
            window.update(tetris.screen);
            std::this_thread::sleep_for(1ms);
            cv_state->flag = false;
        }
    }

    void run() {
        // Spawn threads for rendering/game logic
        std::cout << "Threads started" << std::endl;
        std::thread logicThread(&Game::game_loop, this);
        std::thread renderThread(&Game::render_loop, this);
        window.loop();
        logicThread.join();
        renderThread.join();
    }
};
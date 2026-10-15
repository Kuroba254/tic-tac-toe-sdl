/**
 * Tic-tac-toe Game in C++
 * Using Procedural Programming paradigm.
 * Approaching the game with four functional components (Logic, Interaction, UI and Helper)
 *
 * by baluong.87
 *
 * level 2.0 (OOP paradigm + SDL graphic - module)
 *
 * base from v0.5 23/02/2026
 */

/* ---------- Importing ---------- */

#include <iostream>

#include "game/engine.h"
#include "utils/config.h"
#include "utils/logger.h"

// interface
#include "game/interface/i_interaction.h"
#include "game/interface/i_renderer.h"

// terminal
#include "terminal/interaction.h"
#include "terminal/renderer.h"

// sdl
#include "sdl/interaction.h"
#include "sdl/renderer.h"

/**
 * Mô tả:
 * Hàm main là entry point của chương trình.
 * Thực hiện các bước:
 * - Parse cấu hình từ command line
 * - Khởi tạo logger
 * - Khởi tạo hạ tầng (renderer + interaction) theo mode (terminal/SDL)
 * - Khởi tạo và chạy game engine
 * - Dọn dẹp tài nguyên trước khi thoát
 *
 * Đầu vào:
 * - argc: số lượng tham số dòng lệnh
 * - argv: mảng chứa các tham số
 *
 * Đầu ra:
 * - int: mã trạng thái kết thúc (0 là thành công)
 *
 * Tác dụng phụ:
 * - Khởi tạo toàn bộ hệ thống game
 */
int main(int argc, char* argv[]) {
    // 1. Parse argument và khởi tạo logger hệ thống
    RunConfig config = parseArgs(argc, argv);
    Logger::init(config.judge_mode, config.to_file, config.log_file, config.verbose_flag);

    Logger::log("Game starting . . .");
    Logger::log(config.toString(), Logger::Level::DEBUG);

    // 2. Khởi tạo hạ tầng giao diện (Renderer và Interaction)
    I_Renderer* renderer = nullptr;
    I_Interaction* interaction = nullptr;

    if (config.gui_flag) {
        // chế độ đồ họa SDL
        renderer = new SDLRenderer();
        interaction = new SDLInteraction();
    } else {
        // chế độ terminal văn bản
        renderer = new TerminalRenderer();
        interaction = new TerminalInteraction();
    }

    // 3. Khởi tạo và chạy Engine điều phối game
    Engine engine(&config, renderer, interaction);

    engine.init();

    // Vòng lặp quản lý chơi lại, tránh bị văng game khi kết thúc ván
    bool is_playing = true;
    while (is_playing) {
        engine.startGame();
        
        GameResult result = engine.playGame();
        
        // Nhận lựa chọn từ người chơi: bấm SPACE để chơi lại (trả về 1), ESC để thoát (trả về 0)
        int action = engine.endGame(result); 
        
        if (action == 0) {
            is_playing = false; // Thoát game hoàn toàn
        }
    }

    engine.close();

    // 4. Dọn dẹp hạ tầng giao diện sau khi kết thúc vòng lặp
    if (renderer) {
        delete renderer;
    }
    if (interaction) {
        delete interaction;
    }
    Logger::log("Infra destroyed!");

    // đóng logger (flush và cleanup)
    Logger::log("Logger closing . . .");
    Logger::close();

    return 0;
}
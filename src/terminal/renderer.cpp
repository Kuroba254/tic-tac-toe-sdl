/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {
    // TODO: init state
}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer() {
    // TODO: cleanup nếu cần
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig& config) {
    // TODO: init renderer theo config
    if (config.interactive) {
        clearScreen();
    }
}

/**
 * Mô tả:
 *   Xóa màn hình terminal.//
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen() {
    // TODO: clear screen
    std::cout << "\n"
              << "\x1B[2J\x1B[H"
              << "\n";
    return;
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: render menu theo loại
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;

        case SelectType::SIZE_UI:
            // TODO: display board size selection
            // Example: "Size Input (NxN, 3 <= N <= BOARD_N_MAX)"
            std::cout<<"NHAP KICH THUOC N\n";
            std::cout<<"VOI 3<=N<="<<BOARD_N_MAX<<": ";
            break;
        case SelectType::GOAL_UI:
            // TODO: display goal selection
            // Example: "Goal Input (3 - 5, goal <= size)"
            std::cout<<"NHAP MUC TIEU GOAL\n";
            std::cout<<"VOI 3<=goal<= KICH THUOC BANG: ";
            break;

        case SelectType::GAME_MODE_UI:
            // TODO: display game mode selection
            // Example: "(1) PvP | (2) PvE | (3) EvE"
            std::cout<<"LUA CHON CHE DO CUA BAN\n";
            std::cout<<"(1) PvP\n";
            std::cout<<"(2) PvE\n";
            std::cout<<"(3) EvE\n";
            std::cout << "NHAP LUA CHON (1-3): ";
            break;

        case SelectType::BOT_LEVEL_UI:
            // TODO: display bot level selection
            // Example: "(1) EASY | (2) MEDIUM | (3) HARD"
            std::cout<<"LUA CHON MUC DO BOT CUA BAN\n";
            std::cout<<"(1) EASY\n";
            std::cout<<"(2) MEDIUM\n";
            std::cout<<"(3) HARD\n";
            std::cout << "NHAP LUA CHON (1-3): ";
            break;

        case SelectType::PLAYER_UI:
            // TODO: display player move prompt
            // Example input format: "(row, col)"
            std::cout<<"NEU BAN MUON NHAP O HANG 1 COT 1 THI SE NHAP LA:\n 1 1\n";
            std::cout<<"NHAP NUOC DI CUA BAN: ";
            break;

        case SelectType::MUL_BOT_LEVEL_UI:
            // TODO: display multiple bot level selection
            // Example input format: "(bot1_level, bot2_level)"
            std::cout<<"NEU BAN MUON NHAP MUC DO DE CHO BOT1 VA KHO CHO BOT 2 THI NHAP NHU SAU:\n";
            std::cout<<"1 3\n";
            std::cout<<"NHAP MUC DO KHO CUA BOT 1 VA 2: ";
            break;

        default:
            break;
    }

    return;
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: render invalid message
    std::cout << "LUA CHON KHONG HOP LE, VUI LONG NHAP LAI!\n";
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: render success message
    std::cout << "DA CHON!\n";
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    // TODO:
    int gachngang = 3 * size + 3; 
    std::cout << "R/C|";
    for(int i = 0; i < size; i++) {
        if(i < 10) { std::cout << " " << i; } else { std::cout << i; }
        if(i < size - 1) { std::cout << " "; } else { std::cout << "\n"; }
    }
    for(int i = 0; i < gachngang; i++) {
        std::cout << "-";
        if(i == gachngang - 1) { std::cout << "\n"; }
    }
    for(int i = 0; i < size; i++) {
        if(i < 10) { std::cout << " " << i << " |"; } else { std::cout << i << " |"; } 
        for(int j = 0; j < size; j++) {
            std::cout << " " << board[i][j];
            if(j < size - 1) { std::cout << " "; } 
            else { std::cout << "\n"; }
        }
    }
}

/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot) {
    // TODO: render player info
    if(player==0 && is_bot==true){
        std::cout<<"BOT(NGUOI CHOI 1) DANG SUY NGHI...\n";
    }
    if(player==0 && is_bot==false){
        std::cout<<"LUOT CUA NGUOI CHOI 1(X)\n";
    }
    if(player==1 && is_bot==true){
        std::cout<<"BOT(NGUOI CHOI 2) DANG SUY NGHI...\n";
    }
    if(player==1 && is_bot==false){
        std::cout<<"LUOT CUA NGUOI CHOI 2(O)\n";
    }
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col) {
    // TODO: render move
    std::cout<<"DA DANH VAO VI TRI ("<<row<<", "<<col<<")\n";
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove() {
    // TODO: render invalid move message
    std::cout<<"KHONG HOP LE\n";
    std::cout<<"VUI LONG KIEM TRA LAI\n";
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: render result
    if(winner==-1){std::cout<<"TRAN DAU HOA!\n";}
    else{
        if (winner == 0) {
            std::cout << "NGUOI CHOI 1 (X) THANG!\n";
        } else if (winner == 1) {
            if (is_bot) {
                std::cout << "BOT THANG!\n";
            } else {
                std::cout << "NGUOI CHOI 2 (O) THANG!\n";
            }
        }
    }
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult& gameResult) {
    // TODO: print result đơn giản
    std::cout<<gameResult.winner<<" "<<gameResult.turns<<"\n";
}

/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close() {
    // TODO: cleanup renderer
}
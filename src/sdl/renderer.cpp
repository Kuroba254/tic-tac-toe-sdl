/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer() {
}

SDLRenderer::~SDLRenderer() {
}

void SDLRenderer::init(const RunConfig& config) {
    int screenWidth = 1000;
    int screenHeight = 700;

    SDL_Init(SDL_INIT_VIDEO);
    
    // Khởi tạo thư viện Font chữ
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF Error: " << TTF_GetError() << "\n";
    }

    // Khởi tạo thư viện hình ảnh PNG
    int imgFlags = IMG_INIT_PNG; 
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
    }

    window = SDL_CreateWindow(
        "TicTacToe SDL - UET vs HUST",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Tải tài nguyên hình ảnh từ thư mục assets
    backgroundTexture = loadTexture("assets/LOGO_MANUTD.png"); 
    hustTexture = loadTexture("assets/LOGO_HUST.png");             
    uetTexture = loadTexture("assets/LOGO_UET.png");               

    // Tải font chữ size 26 chuyên dụng cho menu
    font = TTF_OpenFont("assets/font.ttf", 26);
    if (!font) {
        std::cout << "Khong tim thay file assets/font.ttf! Vui long kiem tra lai vị tri file.\n";
    }
}

void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  
    SDL_RenderClear(renderer);
}

void SDLRenderer::renderPresent() {
    SDL_RenderPresent(renderer);
}

void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

// Hàm bổ trợ nội bộ giúp render chữ mượt mà chống răng cưa lên cửa sổ SDL
void SDLRenderer::renderText(const std::string& text, int x, int y, SDL_Color color, bool centerH) {
    if (!font || text.empty()) return;

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    if (centerH) {
        dstRect.x = (1000 - surface->w) / 2; // Tự động tính toán để căn giữa hàng ngang
    }

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void SDLRenderer::showSelectMenu(SelectType selectType, int context) {
    if (selectType != SelectType::TITLE_UI &&
        selectType != SelectType::SIZE_UI &&
        selectType != SelectType::GOAL_UI &&
        selectType != SelectType::GAME_MODE_UI &&
        selectType != SelectType::BOT_LEVEL_UI &&
        selectType != SelectType::MUL_BOT_LEVEL_UI) { 
        return; 
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // --- ĐÃ SỬA: Kéo giãn hộp ra 850px chiều ngang, 480px chiều cao và căn lại tâm ---
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 210);
    SDL_Rect menuBox = { 75, 60, 850, 480 }; 
    SDL_RenderFillRect(renderer, &menuBox);
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); 
    SDL_RenderDrawRect(renderer, &menuBox);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color gray = {160, 160, 160, 255};

    // Đẩy tiêu đề lên cao một chút để không bị cấn vào viền hộp
    renderText("=== TIC TAC TOE INTERMEDIATE ===", 0, 70, gold, true);

    switch (selectType) {
        case SelectType::TITLE_UI:
            // Tách khoảng cách các dòng cho thoáng
            renderText("CHAO MUNG BAN DEN VOI TRAN DAU SIU KINH DIEN!", 0, 220, white, true);
            renderText("UET (O)  VS  HUST (X)", 0, 300, gold, true);
            renderText("Nhan phim ENTER de bat dau ngay...", 0, 440, white, true);
            break;

        case SelectType::SIZE_UI:
            renderText("[ CAU HINH BAN CO ]", 0, 190, gold, true);
            renderText("Vui long nhap KICH THUOC ban co ban muon choi:", 0, 260, white, true);
            renderText("Goi y: Nhap tu phim so (3 den 12)", 0, 320, gray, true);
            renderText("Sau do nhan ENTER de xac nhan", 0, 440, gold, true);
            break;

        case SelectType::GOAL_UI:
            renderText("[ CAU HINH DIEU KIEN THANG ]", 0, 190, gold, true);
            renderText("Nhap SO QUAN LIEN TIEP xep hang de chien thang:", 0, 260, white, true);
            renderText("Luu y: So quan phai >= 3 va <= Kich thuoc ban co", 0, 320, gray, true);
            renderText("Sau do nhan ENTER de xac nhan", 0, 440, gold, true);
            break;

        case SelectType::GAME_MODE_UI:
            renderText("[ CHON CHE DO CHOI KINH DIEN ]", 0, 180, gold, true);
            // Dịch tọa độ X để các menu chọn nằm giữa hộp mới
            renderText("Phim 1 : PVP (Nguoi choi Dau Nguoi choi)", 220, 250, white, false);
            renderText("Phim 2 : PVE (Thach dau voi BOT thong minh)", 220, 310, white, false);
            renderText("Phim 3 : EVE (Xem hai BOT tu thach dau nhau)", 220, 370, white, false);
            renderText("Bam phim (1-3) va nhan ENTER de xac nhan", 0, 410, gold, true);
            break;

        case SelectType::BOT_LEVEL_UI: {
            renderText("[ CHON DO KHO CHO MAY THONG MINH ]", 0, 180, gold, true);
            renderText("Phim 1 : CHE DO DE (Easy Bot)", 270, 250, white, false);
            renderText("Phim 2 : CHE DO TRUNG BINH (Medium Bot)", 270, 310, white, false);
            renderText("Phim 3 : CHE DO KHO (Hardcore Bot)", 270, 370, white, false);
            renderText("Bam phim (1-3) va nhan ENTER de xac nhan", 0, 410, gold, true);
            break;
        }

        case SelectType::MUL_BOT_LEVEL_UI: {
            std::string botTitle = "";
            if (context == 0) botTitle = "CHON DO KHO CHO BOT 1 (QUAN X)";
            else if (context == 1) botTitle = "CHON DO KHO CHO BOT 2 (QUAN O)";
            else botTitle = "CHON DO KHO CHO BOT";

            renderText("[ " + botTitle + " ]", 0, 180, gold, true);
            renderText("Phim 1 : CHE DO DE (Easy Bot)", 270, 250, white, false);
            renderText("Phim 2 : CHE DO TRUNG BINH (Medium Bot)", 270, 310, white, false);
            renderText("Phim 3 : CHE DO KHO (Hardcore Bot)", 270, 370, white, false);
            renderText("Bam phim (1-3) va nhan ENTER de xac nhan", 0, 410, gold, true);
            break;
        }
        
        default:
            break;
    }
    renderPresent();
}

void SDLRenderer::showInvalidSelect(SelectType selectType, int context) {
    // Vẽ một dải đỏ thông báo lỗi nhập liệu ở đáy menu UI
    SDL_Color red = {255, 60, 60, 255};
    renderText(">>> LUA CHON KHONG HOP LE! VUI LONG NHAP LAI CHINH XAC.", 0, 475, red, true);
    renderPresent();
    SDL_Delay(500);
}

void SDLRenderer::showValidSelect(SelectType selectType, int context) {
    // 1. Vẽ dải xanh lá báo xác nhận thành công
    SDL_Color green = {100, 255, 100, 255};
    renderText(">>> XAC NHAN THANH CONG: DANG CHUYEN TIEP...", 0, 475, green, true);
    renderPresent();

    // 2. Dừng hình đúng 0.5 giây để người chơi kịp đọc
    SDL_Delay(500);

    // 3. Vượt qua Pause bằng Event chuẩn
    SDL_Event skipEvent;
    SDL_memset(&skipEvent, 0, sizeof(skipEvent)); // QUAN TRỌNG: Xóa sạch rác bộ nhớ
    
    skipEvent.type = SDL_KEYDOWN;
    // Dùng phím F15 (một phím rất hiếm trên bàn phím) 
    // để đảm bảo SDL2 nhận diện hợp lệ mà không bị trùng với các phím (0-9) của game
    skipEvent.key.keysym.sym = SDLK_F15; 
    
    SDL_PushEvent(&skipEvent);
}

void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    this->currentSize = size; 

    int BOARD_PIXELS = 480; 
    int CELL_SIZE = BOARD_PIXELS / size;
    int startX = (1000 - BOARD_PIXELS) / 2; 
    int startY = (700 - BOARD_PIXELS) / 2; 

    SDL_RenderClear(renderer);
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    // Phủ tấm kính đen mờ sau lưới cờ
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); 
    SDL_Rect boardBg = {startX, startY, CELL_SIZE * size, CELL_SIZE * size};
    SDL_RenderFillRect(renderer, &boardBg);

    // Vẽ lưới bàn cờ màu trắng sắc nét dày dặn
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200); 
    for (int i = 0; i <= size; i++) {
        for(int offset = -1; offset <= 1; offset++) {
            SDL_RenderDrawLine(renderer, startX, startY + i * CELL_SIZE + offset, 
                                         startX + size * CELL_SIZE, startY + i * CELL_SIZE + offset);
            SDL_RenderDrawLine(renderer, startX + i * CELL_SIZE + offset, startY, 
                                         startX + i * CELL_SIZE + offset, startY + size * CELL_SIZE);
        }
    }

    // Đổ hình ảnh LOGO UET (O) và HUST (X) vào từng ô cờ tương ứng
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            int cellX = startX + c * CELL_SIZE;
            int cellY = startY + r * CELL_SIZE;
            int pad = CELL_SIZE / 10; 
            
            SDL_Rect destRect = {cellX + pad, cellY + pad, CELL_SIZE - 2*pad, CELL_SIZE - 2*pad};

            if (board[r][c] == 'X' && hustTexture) {
                SDL_RenderCopy(renderer, hustTexture, NULL, &destRect);
            } 
            else if (board[r][c] == 'O' && uetTexture) {
                SDL_RenderCopy(renderer, uetTexture, NULL, &destRect);
            }
        }
    }
    renderPresent();
}

void SDLRenderer::showMove(const int row, const int col) {
    int BOARD_PIXELS = 480; 
    int CELL_SIZE = BOARD_PIXELS / currentSize;
    int startX = (1000 - BOARD_PIXELS) / 2; 
    int startY = (700 - BOARD_PIXELS) / 2; 
    
    int cellX = startX + col * CELL_SIZE;
    int cellY = startY + row * CELL_SIZE;

    SDL_Color highlight = {255, 215, 0, 255}; 
    for(int i = 0; i < 4; i++) {
        drawRect(cellX + i, cellY + i, CELL_SIZE - 2*i, CELL_SIZE - 2*i, highlight, false);
    }
    renderPresent();
}

void SDLRenderer::showInvalidMove() {
    SDL_Color errorColor = {255, 70, 70, 255};
    drawRect(0, 15, 1000, 42, {30, 0, 0, 230}, true);
    renderText("O NAY ĐA CO QUAN HOAC NAM NGOAI VIEN! VUI LONG CLICK O KHAC.", 0, 22, errorColor, true);
    renderPresent();
}

void SDLRenderer::showPlayer(const int player, const bool is_bot) {
    // Hiển thị Banner thông báo lượt đi thời gian thực ngay dưới bàn cờ cực đẹp
    SDL_Color textClr = {255, 255, 255, 255};
    SDL_Color cyan = {0, 255, 255, 255};
    
    drawRect(0, 645, 1000, 45, {10, 10, 10, 220}, true);

    std::string turnStr = "LUOT DI HIEN TAI: ";
    if (player == 0) turnStr += "UET - ĐAI HOC CONG NGHE (O)";
    else turnStr += "HUST - ĐAI HOC BACH KHOA (X)";
    
    if (is_bot) turnStr += " [ MAY DANG TINH... ]";
    else turnStr += " [ DEN LUOT BAN CLICK CHUOT ]";
    
    renderText(turnStr, 0, 653, cyan, true);
    renderPresent();
}

void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // 1. Vẽ khung nền ở phía dưới bàn cờ (từ y=600 đến y=700)
    drawRect(0, 600, 1000, 100, {0, 0, 0, 220}, true);
    SDL_Color gold = {255, 215, 0, 255};
    SDL_Color white = {255, 255, 255, 255};

    // 2. Căn lại tọa độ chữ:
    // Dòng 1: Kết quả thắng/thua (đặt tại y=615)
    std::string resultStr = "";
    if (winner == DRAW_RESULT) {
        resultStr = "KET QUA: HOA NHAU!";
    } else {
        resultStr = (winner == 1) ? "CHIEN THANG: TRUONG UET (O)!!!" : "CHIEN THANG: TRUONG HUST (X)!!!";
    }
    renderText(resultStr, 0, 615, gold, true);

    // Dòng 2: Hướng dẫn chơi lại (đặt tại y=655)
    renderText(">> BAM [SPACE] DE CHOI LAI --- BAM [ESC] DE VE MENU <<", 0, 655, white, true);

    // 3. Phần gạch đỏ (WinLine) giữ nguyên vì nó cần đè lên bàn cờ để đánh dấu
    if (winLine != nullptr && !winLine->cells.empty()) {
        int BOARD_PIXELS = 480; 
        int CELL_SIZE = BOARD_PIXELS / currentSize;
        int startX = (1000 - BOARD_PIXELS) / 2; 
        int startY = (700 - BOARD_PIXELS) / 2; 

        int r1 = winLine->cells.front().first;
        int c1 = winLine->cells.front().second;
        int r2 = winLine->cells.back().first;
        int c2 = winLine->cells.back().second;

        int px1 = startX + c1 * CELL_SIZE + CELL_SIZE/2;
        int py1 = startY + r1 * CELL_SIZE + CELL_SIZE/2;
        int px2 = startX + c2 * CELL_SIZE + CELL_SIZE/2;
        int py2 = startY + r2 * CELL_SIZE + CELL_SIZE/2;

        SDL_SetRenderDrawColor(renderer, 255, 30, 30, 255); 
        for(int offset = -3; offset <= 3; offset++) {
            SDL_RenderDrawLine(renderer, px1 + offset, py1 + offset, px2 + offset, py2 + offset);
        }
    }
    renderPresent();
}

void SDLRenderer::printResult(const GameResult& gameResult) {
    if (gameResult.winner == DRAW_RESULT) {
        std::cout << "DRAW " << gameResult.turns << "\n";
    } else {
        std::cout << "WINNER " << gameResult.winner << " BOT " << gameResult.isBot << " TURNS " << gameResult.turns << "\n";
    }
}

void SDLRenderer::close() {
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (hustTexture) SDL_DestroyTexture(hustTexture); 
    if (uetTexture) SDL_DestroyTexture(uetTexture);

    // Giải phóng bộ nhớ Font chữ hệ thống
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();
    IMG_Quit(); 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
SDL_Texture* SDLRenderer::loadTexture(const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        std::cout << "Could not load image: " << path << ", SDL_image Error: " << IMG_GetError() << "\n";
    }
    return texture;
}

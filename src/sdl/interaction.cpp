/**
 * SDL Interaction cpp implementation
 *
 */

#include "interaction.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

/* ---------- Definitions ---------- */

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 *   - e: SDL_Event hiện tại.
 * Đầu ra:
 *   - false nếu không phải quit event.
 * Tác dụng phụ:
 *   - Ném QuitException nếu nhận được sự kiện thoát.
 */
bool SDLInteraction::waitForQuit(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        throw QuitException();
    }
    return false;
}

/**
 * Mô tả: Khởi tạo đối tượng SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::SDLInteraction() {
}

/**
 * Mô tả: Destructor của SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::~SDLInteraction() {
}

/**
 * Mô tả: Khởi tạo hệ thống interaction cho SDL.
 * Đầu vào:
 *   - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Thiết lập trạng thái ban đầu cho input SDL.
 */
void SDLInteraction::init(const RunConfig& config) {
    // TODO:
    // - Khởi tạo các thành phần cần thiết cho input SDL
    // - Có thể reset event queue hoặc trạng thái input
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}// dọn từ đầu tới cuối những thứ nằm trong hàng đợi

/**
 * Mô tả: Tạm dừng chương trình trong SDL.
 *        - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 *        - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 *   - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Có thể block thread hiện tại.
 *   - Có thể ném QuitException nếu người dùng đóng cửa sổ.
 */
void SDLInteraction::pause(int timeout) {
    // Nếu có timeout cụ thể -> Vẫn delay nhưng phải liên tục xử lý Event Loop
    if (timeout > 0) {
        Uint32 start = SDL_GetTicks();
        SDL_Event e;
        
        // Vòng lặp chờ cho đến khi hết thời gian timeout
        while (SDL_GetTicks() - start < (Uint32)timeout) {
            // Xử lý các sự kiện để cửa sổ hệ điều hành không bị đơ
            while (SDL_PollEvent(&e)) {
                waitForQuit(e); // Vẫn cho phép bấm nút X để thoát lúc Bot đang đi
            }
            SDL_Delay(10); // Ngủ 10ms mỗi chu kỳ để tránh ngốn CPU
        }
        return;
    }
    
    bool waiting = true;
    SDL_Event event;
    // Vòng lặp chờ sự kiện từ người dùng
    while (waiting) {
        // SDL_WaitEvent sẽ block cho tới khi có event
        if (SDL_WaitEvent(&event)) {
            // Nếu người dùng đóng cửa sổ -> thoát game
            if (waitForQuit(event)) {
            }
            // Nếu có tương tác (nhấn phím hoặc click chuột) -> kết thúc pause
            if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                waiting = false;
            }
        }
    }
}

/**
 * Mô tả: Lấy input kích thước bàn cờ từ người dùng qua SDL.
 * Đầu vào:
 *   - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 *   - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật giá trị tại size nếu thành công.
 */
bool SDLInteraction::selectSize(int* size) {
    int inputVal = 0;
    SDL_Event e;
    
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e); // Kiểm tra xem người dùng có bấm nút X tắt game không
        
        if (e.type == SDL_KEYDOWN) {
            // Quan trọng: Chỉ xử lý nếu đây là lần nhấn phím đầu tiên (chống trôi phím)
            if (e.key.repeat == 0) {
                SDL_Keycode key = e.key.keysym.sym;
                int chu_so = -1;
                
                // 1. Bắt phím số hàng ngang
                if (key >= SDLK_0 && key <= SDLK_9) {
                    chu_so = key - SDLK_0;
                } 
                // 2. Bắt phím số Numpad bên phải
                else if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
                    chu_so = key - SDLK_KP_0;
                }
                
                // 3. Nếu người dùng có bấm phím số
                if (chu_so != -1) {
                    inputVal = inputVal * 10 + chu_so; 
                } 
                // 4. Cho phép dùng Backspace để xóa số gõ nhầm
                else if (key == SDLK_BACKSPACE) {
                    inputVal = inputVal / 10;
                }
                // 5. Bắt phím Enter (cả phím Enter chính và Enter bên Numpad) để xác nhận
                else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                    // Kiểm tra xem số vừa nhập có nằm trong khoảng 3 đến 12 không
                    if (inputVal >= 3 && inputVal <= 12) {
                        *size = inputVal; // Gán giá trị hợp lệ vào biến size
                        
                        // ---> DÒNG MỚI THÊM VÀO ĐỂ LƯU KÍCH THƯỚC BÀN CỜ <---
                        this->currentBoardSize = inputVal; 
                        
                        return true;      // Thành công, thoát hàm
                    }
                    
                    // Nếu nhập sai (ví dụ gõ 99 rồi bấm Enter) -> Reset về 0 gõ lại
                    return false; 
                }
            }
        }
    }
    return false;
}

/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 *   - goal: con trỏ lưu giá trị goal.
 *   - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật goal nếu hợp lệ.
 */
bool SDLInteraction::selectGoal(int* goal, const int size) {
    // TODO:
    // - Lấy input từ SDL
    // - Parse thành số nguyên
    // - Kiểm tra điều kiện (3 <= goal <= size)
    // - Trả về true nếu hợp lệ
    int inputVal = 0;
    SDL_Event e;
    
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e); // Kiểm tra xem người dùng có bấm nút X tắt game không
        
        if (e.type == SDL_KEYDOWN) {
            // Quan trọng: Chỉ xử lý nếu đây là lần nhấn phím đầu tiên (chống trôi phím)
            if (e.key.repeat == 0) {
                SDL_Keycode key = e.key.keysym.sym;
                int chu_so = -1;
                
                // 1. Bắt phím số hàng ngang
                if (key >= SDLK_0 && key <= SDLK_9) {
                    chu_so = key - SDLK_0;
                } 
                // 2. Bắt phím số Numpad bên phải
                else if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
                    chu_so = key - SDLK_KP_0;
                }
                
                // 3. Nếu người dùng có bấm phím số
                if (chu_so != -1) {
                    inputVal = inputVal * 10 + chu_so; 
                } 
                // 4. Cho phép dùng Backspace để xóa số gõ nhầm
                else if (key == SDLK_BACKSPACE) {
                    inputVal = inputVal / 10;
                }
                // 5. Bắt phím Enter (cả phím Enter chính và Enter bên Numpad) để xác nhận
                else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                    // Kiểm tra xem số vừa nhập có nằm trong khoảng 3 đến 12 không
                    if (inputVal >= 3 && inputVal <= size) {
                        *goal = inputVal; // Gán giá trị hợp lệ vào biến size
                        return true;      // Thành công, thoát hàm
                    }
                    
                    // Nếu nhập sai (ví dụ gõ 99 rồi bấm Enter) -> Reset về 0 gõ lại
                    return false; 
                }
            }
        }
    }
    return false;
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 *   - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    // TODO:
    // - Lắng nghe input từ SDL (phím số hoặc click)
    // - Map input sang GameMode tương ứng
    // - Validate giá trị (1-3)
    int inputVal = 0;
    SDL_Event e;
    
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e); // Kiểm tra xem người dùng có bấm nút X tắt game không
        
        if (e.type == SDL_KEYDOWN) {
            // Quan trọng: Chỉ xử lý nếu đây là lần nhấn phím đầu tiên (chống trôi phím)
            if (e.key.repeat == 0) {
                SDL_Keycode key = e.key.keysym.sym;
                int chu_so = -1;
                
                // 1. Bắt phím số hàng ngang
                if (key >= SDLK_0 && key <= SDLK_9) {
                    chu_so = key - SDLK_0;
                } 
                // 2. Bắt phím số Numpad bên phải
                else if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
                    chu_so = key - SDLK_KP_0;
                }
                
                // 3. Nếu người dùng có bấm phím số
                if (chu_so != -1) {
                    inputVal = inputVal * 10 + chu_so; 
                } 
                // 4. Cho phép dùng Backspace để xóa số gõ nhầm
                else if (key == SDLK_BACKSPACE) {
                    inputVal = inputVal / 10;
                }
                // 5. Bắt phím Enter (cả phím Enter chính và Enter bên Numpad) để xác nhận
                else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                    // Kiểm tra xem số vừa nhập có nằm trong khoảng 3 đến 12 không
                    if (inputVal >= 1 && inputVal <= 3) {
                              // Thành công, thoát hàm
                        if(inputVal==1){
                            *mode=GameMode::PVP;
                            return true;
                        }
                        if(inputVal==2){
                            *mode=GameMode::PVE;
                            return true;
                        }
                        if(inputVal==3){
                            *mode=GameMode::EVE;
                            return true;
                        }
                    }
                    // Nếu nhập sai (ví dụ gõ 99 rồi bấm Enter) -> Reset về 0 gõ lại
                    return false; 
                }
            }
        }
    }
    return false;
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 * - levels: mảng lưu cấp độ bot.
 * - index: vị trí bot cần gán.
 * Đầu ra:
 * - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 * - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
    // THÊM DÒNG NÀY: Xóa toàn bộ phím Enter/phím F15 còn sót lại từ menu trước đó
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    // Ánh xạ an toàn
    int inputVal = 0;
    SDL_Event e;
    
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e);
        
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            SDL_Keycode key = e.key.keysym.sym;
            int chu_so = -1;
            
            if (key >= SDLK_0 && key <= SDLK_9) chu_so = key - SDLK_0;
            else if (key >= SDLK_KP_0 && key <= SDLK_KP_9) chu_so = key - SDLK_KP_0;
            
            if (chu_so != -1) {
                if (inputVal < 10) {
                    inputVal = inputVal * 10 + chu_so; 
                }
            } 
            else if (key == SDLK_BACKSPACE) {
                inputVal = inputVal / 10;
            }
            else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                if (inputVal >= 1 && inputVal <= 3) {
                    switch(inputVal) {
                        case 1: levels[index] = BotLevel::EASY; break;
                        case 2: levels[index] = BotLevel::MEDIUM; break;
                        case 3: levels[index] = BotLevel::HARD; break;
                    }
                    return true;
                }
                return false; 
            }
        }
    }
    return false;
}

/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Đầu vào:
 *   - row: con trỏ lưu hàng.
 *   - col: con trỏ lưu cột.
 * Đầu ra:
 *   - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 *   - Cập nhật row, col nếu hợp lệ.
 */
bool SDLInteraction::getPlayerMove(int* row, int* col) {
    // THÊM DÒNG NÀY: Đảm bảo khi vào trận đấu, chuột và phím được làm sạch, không bị click đúp tự động
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e); 
        
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                
                int BOARD_PIXELS = 480; 
                int CELL_SIZE = BOARD_PIXELS / currentBoardSize; 
                
                int START_X = (1000 - BOARD_PIXELS) / 2;
                int START_Y = (700 - BOARD_PIXELS) / 2;

                if (mouseX >= START_X && mouseX < START_X + BOARD_PIXELS &&
                    mouseY >= START_Y && mouseY < START_Y + BOARD_PIXELS) {
                    
                    *col = (mouseX - START_X) / CELL_SIZE;
                    *row = (mouseY - START_Y) / CELL_SIZE;
                    
                    return true; 
                }
            }
        }
    }
    return false;
}
/**
 * Mô tả: Dọn dẹp tài nguyên liên quan đến SDL interaction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Giải phóng hoặc reset trạng thái input nếu cần.
 */
void SDLInteraction::close() {
    // TODO:
    // - Dọn dẹp trạng thái, nếu có resource thì giải phóng
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}
int SDLInteraction::getPostGameAction() {
    // Xóa sạch lịch sử phím bấm/chuột cũ để không bị dính nút
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    SDL_Event e;
    
    // Vòng lặp chờ người chơi bấm phím
    while (SDL_WaitEvent(&e)) {
        waitForQuit(e); // Vẫn cho phép tắt game bằng nút X
        
        if (e.type == SDL_KEYDOWN) {
            // Nếu bấm phím SPACE (Dấu cách) -> Trả về 1 (Chơi lại)
            if (e.key.keysym.sym == SDLK_SPACE) {
                return 1;
            }
            // Nếu bấm phím ESC -> Trả về 2 (Về Menu)
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                return 2;
            }
        }
    }
    return 0; // Trả về 0 nếu có lỗi
}
# Tic-tac-toe: UET vs HUST

Game Tic-tac-toe / Caro viết bằng **C++20 + SDL2** theo hướng lập trình hướng đối tượng, có bot chơi theo thuật toán **Minimax + cắt tỉa Alpha-Beta + hàm đánh giá Heuristic**. Chạy được trên Terminal, cửa sổ SDL và **ngay trên trình duyệt** (WebAssembly).

### ▶ Chơi ngay: https://kuroba254.github.io/tic-tac-toe-sdl/

![Màn hình kết quả](docs/screenshot-result.png)

## Tính năng

- Bàn cờ N×N tuỳ chọn (3 ≤ N ≤ 12), số quân liên tiếp để thắng tuỳ chọn.
- 3 chế độ: người – người, người – máy, máy – máy.
- 3 mức bot:
  - **Dễ:** đánh ngẫu nhiên.
  - **Trung bình:** ưu tiên thắng ngay / chặn đối thủ.
  - **Khó:** Minimax + Alpha-Beta, nhìn trước 6 nước (bàn 3×3 thì tính đến hết ván, không bao giờ thua), mỗi nước thường nghĩ dưới 0.2 giây (bản desktop).
- Luật chặn hai đầu: chuỗi đủ quân nhưng bị đối thủ chặn cả hai đầu thì không tính thắng.

## Cách chơi

| Màn hình | Thao tác |
|---|---|
| Tiêu đề | `Enter` |
| Kích thước bàn, số quân để thắng | gõ số → `Enter` (`Backspace` để xoá) |
| Chế độ chơi | `1` người–người · `2` người–máy · `3` máy–máy → `Enter` |
| Độ khó bot | `1` dễ · `2` trung bình · `3` khó → `Enter` |
| Đánh cờ | click chuột trái vào ô trống |
| Hết ván | `Space` chơi lại |

Trên trình duyệt: click vào khung game trước để bàn phím hoạt động. Game cần bàn phím nên nên chơi trên máy tính.

## Tự build

### Desktop (Windows MSYS2 UCRT64 / Linux / macOS)

Cần CMake, trình biên dịch hỗ trợ C++20 (GCC 13+ / Clang 17+), SDL2, SDL2_ttf, SDL2_image.

```bash
# Windows (MSYS2 UCRT64):
pacman -S mingw-w64-ucrt-x86_64-{gcc,cmake,make,SDL2,SDL2_ttf,SDL2_image}
# Ubuntu/Debian:
sudo apt install g++ cmake libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev

cmake -S . -B build
cmake --build build
cd build
./game -g        # giao diện đồ hoạ
./game           # giao diện Terminal
```

Các tuỳ chọn dòng lệnh: `-g` đồ hoạ, `-v` log chi tiết, `-l <file>` ghi log ra file (để trống thì in ra màn hình), `-j -i <file>` chế độ chấm điểm đọc input từ file.

### Web (Emscripten)

```bash
emcmake cmake -S . -B build-web
cmake --build build-web
python3 -m http.server -d build-web 8000   # mở http://localhost:8000
```

Bản web tự động được build và đăng lên GitHub Pages bằng GitHub Actions (`.github/workflows/deploy-web.yml`) mỗi khi push lên nhánh `main`.

## Cấu trúc mã nguồn

```
src/
├── main.cpp              chọn Terminal hoặc SDL, vòng lặp chơi lại
├── game/
│   ├── engine.*          điều phối: init → startGame → playGame → endGame → close
│   ├── logic.*           luật chơi, kiểm tra thắng/hoà
│   ├── interface/        I_Renderer, I_Interaction (lớp trừu tượng)
│   └── bot/              Bot → BotLevel1 → BotLevel2 → BotLevel3, BotFactory
├── terminal/             TerminalRenderer, TerminalInteraction (cout/cin)
├── sdl/                  SDLRenderer, SDLInteraction (SDL2)
└── utils/                logger, config, helper
web/shell.html            trang HTML cho bản web
```

Engine chỉ làm việc qua `I_Renderer` / `I_Interaction`, nên cùng một luồng game chạy cho cả Terminal, cửa sổ SDL và trình duyệt.

## Ghi chú

Dự án được làm trong khuôn khổ bài tập lớn môn Phương pháp luận lập trình, Viện Trí tuệ nhân tạo, Trường Đại học Công nghệ – ĐHQGHN. Logo trong `assets/` thuộc về các tổ chức tương ứng, chỉ dùng cho mục đích học tập, phi thương mại. Font: Playfair Display (SIL Open Font License).

Tác giả: Hoàng Trung Hiếu · [github.com/Kuroba254](https://github.com/Kuroba254)

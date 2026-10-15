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

<p align="center"><img src="docs/report/arch.png" width="760" alt="Kiến trúc chương trình"></p>

## Báo cáo

### 📄 [Đọc báo cáo đầy đủ →](docs/REPORT.md)

Báo cáo bài tập lớn trình bày thiết kế Engine, thuật toán bot HARD, giao diện SDL2, phân tích OOP/SOLID và kết quả thực nghiệm. Tóm tắt nhanh:

| Hạng mục | Kết quả |
|---|---|
| **Thuật toán bot HARD** | Minimax + cắt tỉa Alpha-Beta, heuristic đếm "cửa sổ" `goal` ô (điểm ±10^k), cập nhật điểm tăng dần, giữ 10 nước tốt nhất mỗi tầng |
| **Đánh hoàn hảo trên 3×3** | Kiểm thử vét cạn mọi chuỗi nước của đối thủ: 533 ván, 0 thua |
| **Đối đầu (840 ván)** | Thắng 684 · hoà 149 · thua 7; so với bot cũ 995 dòng: 200 thắng – 75 hoà – 5 thua với mã ngắn hơn một nửa |
| **Tốc độ** | Tối đa ≈ 0.2 s/nước trên bàn 12×12 |
| **Kiểm chứng luật thắng** | 168 060 thế cờ ngẫu nhiên, khớp 100% với `Logic::checkWin` |

<details>
<summary><b>Các mục trong báo cáo</b></summary>

1. [Tổng quan](docs/REPORT.md#1-tổng-quan-overview): luồng thực thi, giao diện đồ hoạ, nội dung cập nhật
2. [Chi tiết cài đặt](docs/REPORT.md#2-chi-tiết-cài-đặt-implementation-details): Engine, [bot Minimax + Alpha-Beta](docs/REPORT.md#22-bot-mức-hard-minimax--alpha-beta--heuristic), rendering & tương tác SDL, [OOP · SOLID · mẫu thiết kế](docs/REPORT.md#24-trả-lời-câu-hỏi)
3. [Kết quả thực nghiệm](docs/REPORT.md#3-kết-quả-thực-nghiệm-experiment-results): ảnh Terminal vs SDL, assets, log, [đánh giá bot](docs/REPORT.md#34-đánh-giá-bot-hard-mới)
4. [Nhận xét và tổng kết](docs/REPORT.md#4-nhận-xét-và-tổng-kết-bài-làm)
5. [Ghi chú sử dụng AI](docs/REPORT.md#5-ghi-chú-sử-dụng-ai-ai-disclosure)
6. [Nguồn tham khảo](docs/REPORT.md#6-nguồn-tham-khảo-references)

</details>

## Ghi chú

Dự án được làm trong khuôn khổ bài tập lớn môn Phương pháp luận lập trình, Viện Trí tuệ nhân tạo, Trường Đại học Công nghệ – ĐHQGHN. Logo trong `assets/` thuộc về các tổ chức tương ứng, chỉ dùng cho mục đích học tập, phi thương mại. Font: Playfair Display (SIL Open Font License).

Tác giả: Hoàng Trung Hiếu · [github.com/Kuroba254](https://github.com/Kuroba254)

/**
 * Bot LV3 header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <vector>

#include "../setup.h"
#include "bot_lv2.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả:
 *   BotLevel3 là bot mức độ HARD.
 *   Kế thừa từ BotLevel2 và mở rộng bằng thuật toán tìm kiếm trên cây trò chơi:
 *
 *   1. Minimax           : máy "tưởng tượng" trước vài lượt đi của cả hai bên,
 *                          giả sử đối thủ luôn chọn nước đi tốt nhất cho họ.
 *   2. Cắt tỉa Alpha-Beta: bỏ qua các nhánh chắc chắn không làm thay đổi kết quả
 *                          → tìm sâu hơn trong cùng thời gian.
 *   3. Hàm đánh giá Heuristic: khi không thể tìm đến cuối ván, chấm điểm bàn cờ
 *                          bằng cách đếm các "cửa sổ" (window) dài `goal` ô
 *                          mà mỗi bên còn có khả năng tạo thành đường thắng.
 *
 *   Thứ tự ưu tiên khi chọn nước đi:
 *   - Có nước thắng ngay           → đánh luôn.
 *   - Đối thủ sắp thắng ở một ô    → chặn ngay.
 *   - Còn lại                      → chạy Minimax + Alpha-Beta.
 *
 * Đầu vào:
 *   - board, size, goal
 *
 * Đầu ra:
 *   - Một nước đi tốt nhất có thể (row, col)
 *
 * Tác dụng phụ:
 *   - Không có (bot đặt thử quân lên board khi tìm kiếm nhưng luôn gỡ lại)
 *
 * NOTE:
 *   - Bàn 3x3: tìm hết cây trò chơi → bot đánh hoàn hảo (không bao giờ thua).
 *   - Bàn lớn: giới hạn độ sâu và số nước xét ở mỗi tầng để chạy nhanh.
 */
class BotLevel3 : public BotLevel2 {
   private:
    // Điểm số dùng kiểu long long vì 10^goal có thể rất lớn (goal tối đa = size = 12)
    using Score = long long;

    long long visitedNodes = 0;  // số trạng thái đã duyệt (chỉ để ghi log)

    /**
     * Mô tả:
     *   Một nước đi kèm theo mức thay đổi điểm heuristic nếu đánh nước đó.
     *   gain > 0 nghĩa là nước đi làm bàn cờ có lợi hơn cho bot.
     */
    struct ScoredMove {
        pII cell;    // ô (row, col)
        Score gain;  // điểm heuristic tăng/giảm (luôn tính theo góc nhìn của bot)
    };

    /**
     * Mô tả:
     *   Thuật toán Minimax có cắt tỉa Alpha-Beta.
     *   - Lượt của bot      → chọn nước có điểm LỚN nhất (MAX).
     *   - Lượt của đối thủ  → chọn nước có điểm NHỎ nhất (MIN).
     *
     * Đầu vào:
     *   - board, size, goal
     *   - depth: số lượt còn được phép nhìn trước
     *   - alpha: điểm tốt nhất mà bot (MAX) chắc chắn đạt được
     *   - beta : điểm tốt nhất mà đối thủ (MIN) chắc chắn đạt được
     *   - isBotTurn: true nếu đang là lượt của bot
     *   - currentScore: điểm heuristic của bàn cờ hiện tại
     *
     * Đầu ra:
     *   - Score: điểm của trạng thái (càng lớn càng có lợi cho bot)
     *
     * Tác dụng phụ:
     *   - Đặt thử rồi gỡ quân trên board (board được trả về như cũ)
     */
    Score minimax(char board[][BOARD_N_MAX], const int size, const int goal,
                  int depth, Score alpha, Score beta, bool isBotTurn, Score currentScore);

    /**
     * Mô tả:
     *   Hàm đánh giá Heuristic cho TOÀN BỘ bàn cờ.
     *   Duyệt mọi cửa sổ dài `goal` ô theo 4 hướng (ngang, dọc, 2 đường chéo)
     *   và cộng điểm của từng cửa sổ (xem scoreWindow).
     *
     * Đầu ra:
     *   - Score: > 0 là bot đang có lợi, < 0 là đối thủ đang có lợi
     */
    Score evaluateBoard(const char board[][BOARD_N_MAX], const int size, const int goal) const;

    /**
     * Mô tả:
     *   Giống evaluateBoard nhưng chỉ tính các cửa sổ CÓ CHỨA ô (row, col).
     *   Dùng để cập nhật điểm nhanh sau mỗi nước đi thay vì tính lại cả bàn:
     *       điểm mới = điểm cũ - điểm quanh ô (trước khi đặt) + điểm quanh ô (sau khi đặt)
     */
    Score evaluateAround(const char board[][BOARD_N_MAX], const int size, const int goal,
                         const int row, const int col) const;

    /**
     * Mô tả:
     *   Chấm điểm một cửa sổ dài `goal` ô, bắt đầu tại (row, col), đi theo hướng (dRow, dCol).
     *   - Có quân của cả hai bên  → 0 điểm (không ai thắng được ở cửa sổ này).
     *   - Chỉ có quân của bot     → +10^(số quân bot).
     *   - Chỉ có quân đối thủ     → -10^(số quân đối thủ).
     *
     * NOTE:
     *   - Người gọi phải đảm bảo cả cửa sổ nằm trong bàn cờ.
     */
    Score scoreWindow(const char board[][BOARD_N_MAX], const int goal,
                      const int row, const int col, const int dRow, const int dCol) const;

    /**
     * Mô tả:
     *   Sinh danh sách nước đi đáng xét.
     *   - Bàn nhỏ (size <= 5): xét mọi ô trống.
     *   - Bàn lớn: chỉ xét ô trống nằm sát (khoảng cách 1) một quân đã đánh,
     *     vì nước đi ở xa thường vô nghĩa và làm cây tìm kiếm phình to.
     *   - Bàn trống: chỉ xét ô chính giữa.
     */
    std::vector<pII> getCandidateMoves(const char board[][BOARD_N_MAX], const int size) const;

    /**
     * Mô tả:
     *   Chấm điểm từng nước đi cho người đang đi (`who`), sắp xếp từ tốt đến kém
     *   và chỉ giữ lại tối đa `maxMoves` nước. Nước tốt được xét trước
     *   giúp Alpha-Beta cắt tỉa được nhiều nhánh hơn.
     *
     * Tác dụng phụ:
     *   - Đặt thử rồi gỡ quân trên board (board được trả về như cũ)
     */
    std::vector<ScoredMove> orderMoves(char board[][BOARD_N_MAX], const int size, const int goal,
                                       const std::vector<pII>& moves, const char who,
                                       const size_t maxMoves) const;

    /**
     * Mô tả:
     *   Kiểm tra quân `who` vừa đặt tại (row, col) có tạo thành đường thắng không.
     *   Luật giống Logic::checkWin (EndRule::OPEN_TWO): có `goal` quân liên tiếp
     *   và hai ô kẹp hai đầu KHÔNG phải quân đối phương (ô trống / ngoài bàn đều được).
     *
     * NOTE:
     *   - Chỉ kiểm tra 4 đường đi qua (row, col) → nhanh hơn quét cả bàn.
     */
    bool isWinningMove(const char board[][BOARD_N_MAX], const int size, const int goal,
                       const int row, const int col, const char who) const;

    /**
     * Mô tả:
     *   Tìm một ô mà nếu `who` đánh vào thì thắng ngay.
     *
     * Đầu ra:
     *   - pII: ô thắng, hoặc (-1, -1) nếu không có
     */
    pII findWinningMove(char board[][BOARD_N_MAX], const int size, const int goal, const char who) const;

    /**
     * Mô tả:
     *   Chọn độ sâu tìm kiếm theo số ô trống còn lại.
     *   Càng ít ô trống thì cây càng nhỏ → được phép nhìn sâu hơn.
     */
    int chooseDepth(const char board[][BOARD_N_MAX], const int size) const;

   protected:
    /**
     * Mô tả:
     *   Hàm xử lý logic nâng cao cho bot HARD:
     *   thắng ngay → chặn ngay → Minimax + Alpha-Beta.
     *
     * Đầu vào:
     *   - board: trạng thái bàn cờ
     *   - size: kích thước
     *   - goal: điều kiện thắng
     *
     * Đầu ra:
     *   - pII: nước đi tối ưu, (-1, -1) nếu bàn đã đầy
     *
     * Tác dụng phụ:
     *   - Ghi log DEBUG (độ sâu, số trạng thái đã duyệt, điểm của nước đi)
     */
    pII hard_level(char board[][BOARD_N_MAX], const int size, const int goal);

   public:
    /**
     * Mô tả:
     *   Constructor của BotLevel3.
     */
    BotLevel3(const BotLevel& _level, const char& _symbol);

    /**
     * Mô tả:
     *   Destructor của BotLevel3.
     */
    ~BotLevel3() override;

    /**
     * Mô tả:
     *   Lấy nước đi của bot HARD.
     *   1. Dùng thuật toán nâng cao (hard_level)
     *   2. Nếu kết quả không hợp lệ → fallback về logic của BotLevel2
     *
     * Đầu vào:
     *   - board, size, goal
     *
     * Đầu ra:
     *   - pII: nước đi
     *
     * Tác dụng phụ:
     *   - Không có
     */
    pII getMove(char board[][BOARD_N_MAX], const int size, const int goal) override;
};

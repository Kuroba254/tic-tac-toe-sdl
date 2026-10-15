/**
 * Bot LV3 cpp implementation
 *
 * Ý tưởng chính: Minimax + cắt tỉa Alpha-Beta + hàm đánh giá Heuristic.
 *
 *   Mỗi lượt, bot xây dựng (ngầm) một cây trò chơi:
 *     - Gốc   : bàn cờ hiện tại, đến lượt bot.
 *     - Tầng 1: các nước bot có thể đi.
 *     - Tầng 2: các nước đối thủ có thể đáp lại.
 *     - ...
 *   Ở lá (hết độ sâu cho phép), bàn cờ được chấm điểm bằng heuristic.
 *   Bot chọn nhánh có điểm cao nhất, giả sử đối thủ luôn chọn nhánh có điểm thấp nhất.
 */

#include "bot_lv3.h"

/* ---------- Importing ---------- */

#include <algorithm>
#include <format>

#include "../../utils/logger.h"
#include "../logic.h"
using namespace Logic;

/* ---------- Constants ---------- */

namespace {

const char EMPTY_CELL = '-';  // ký hiệu ô trống trên bàn cờ

// 4 hướng cần xét: ngang, dọc, chéo chính, chéo phụ
const int DIRECTIONS[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

// Điểm của một nước thắng. Luôn lớn hơn mọi điểm heuristic:
// bàn có tối đa 4*12*12 = 576 cửa sổ, mỗi cửa sổ tối đa 10^11 điểm → tổng < 10^15.
const long long WIN_SCORE = 1'000'000'000'000'000LL;
const long long INF = WIN_SCORE * 10;  // "vô cùng" cho alpha-beta

const int DEFAULT_DEPTH = 6;      // độ sâu tìm kiếm mặc định (3 lượt bot + 3 lượt đối thủ)
const int FULL_SEARCH_CELLS = 9;  // còn <= 9 ô trống → tìm đến hết ván
const size_t MAX_BRANCH = 10;     // mỗi tầng chỉ xét tối đa 10 nước tốt nhất
const int SMALL_BOARD = 5;        // bàn <= 5x5 thì xét mọi ô trống

/**
 * Mô tả:
 *   Kiểm tra ô (row, col) có nằm trong bàn cờ và chứa ký hiệu `ch` không.
 */
bool cellIs(const char board[][BOARD_N_MAX], const int size, const int row, const int col, const char ch) {
    return row >= 0 && row < size && col >= 0 && col < size && board[row][col] == ch;
}

/**
 * Mô tả:
 *   Kiểm tra cửa sổ dài `goal` ô bắt đầu tại (row, col) theo hướng (dRow, dCol)
 *   có nằm trọn trong bàn cờ không (chỉ cần kiểm tra ô đầu và ô cuối).
 */
bool windowFits(const int size, const int goal, const int row, const int col, const int dRow, const int dCol) {
    const int endRow = row + (goal - 1) * dRow;
    const int endCol = col + (goal - 1) * dCol;
    return row >= 0 && row < size && col >= 0 && col < size &&
           endRow >= 0 && endRow < size && endCol >= 0 && endCol < size;
}

/**
 * Mô tả:
 *   Tính 10^k (k nhỏ nên dùng vòng lặp cho dễ hiểu).
 */
long long powerOf10(const int k) {
    long long result = 1;
    for (int i = 0; i < k; i++) result *= 10;
    return result;
}

}  // namespace

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel3.
 */
BotLevel3::BotLevel3(const BotLevel& _level, const char& _symbol)
    : BotLevel2(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel3.
 */
BotLevel3::~BotLevel3() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot HARD.
 *   - Bước 1: Gọi hard_level để tìm nước đi tối ưu
 *   - Bước 2: Nếu không tìm được nước hợp lệ → fallback về logic của BotLevel2
 */
pII BotLevel3::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    pII move = hard_level(board, size, goal);

    if (!isValidMove(board, size, move.first, move.second)) {
        Logger::log("[BotLv3] no valid move from minimax, fallback to BotLevel2", Logger::Level::WARNING);
        move = BotLevel2::getMove(board, size, goal);
    }
    return move;
}

/**
 * Mô tả:
 *   Logic nâng cao cho bot HARD.
 *   - Bước 1: Nếu bot thắng được ngay → đánh luôn.
 *   - Bước 2: Nếu đối thủ thắng được ngay ở ô nào → chặn ô đó.
 *   - Bước 3: Chạy Minimax + Alpha-Beta trên các nước đi đáng xét.
 *   - Trường hợp biên: bàn đã đầy → trả về (-1, -1).
 */
pII BotLevel3::hard_level(char board[][BOARD_N_MAX], const int size, const int goal) {
    // Bước 1: thắng ngay
    pII winMove = findWinningMove(board, size, goal, symbol);
    if (winMove.first != -1) {
        Logger::log(std::format("[BotLv3] winning move at ({}, {})", winMove.first, winMove.second),
                    Logger::Level::DEBUG);
        return winMove;
    }

    // Bước 2: chặn nước thắng của đối thủ
    pII blockMove = findWinningMove(board, size, goal, op_symbol);
    if (blockMove.first != -1) {
        Logger::log(std::format("[BotLv3] block opponent at ({}, {})", blockMove.first, blockMove.second),
                    Logger::Level::DEBUG);
        return blockMove;
    }

    // Bước 3: Minimax + Alpha-Beta
    std::vector<pII> candidates = getCandidateMoves(board, size);
    if (candidates.empty()) return {-1, -1};          // bàn đầy
    if (candidates.size() == 1) return candidates[0];  // chỉ có 1 lựa chọn (vd: nước đầu tiên)

    const int depth = chooseDepth(board, size);
    const Score rootScore = evaluateBoard(board, size, goal);
    std::vector<ScoredMove> moves = orderMoves(board, size, goal, candidates, symbol, MAX_BRANCH);

    visitedNodes = 0;
    pII bestMove = moves[0].cell;
    Score bestScore = -INF;
    Score alpha = -INF;
    const Score beta = INF;

    for (const ScoredMove& move : moves) {
        const auto [row, col] = move.cell;

        makeMove(board, row, col, symbol);  // đặt thử quân của bot
        // (không cần kiểm tra thắng ở đây vì Bước 1 đã xử lý nước thắng ngay)
        Score value = minimax(board, size, goal, depth - 1, alpha, beta, false, rootScore + move.gain);
        board[row][col] = EMPTY_CELL;  // gỡ quân

        if (value > bestScore) {
            bestScore = value;
            bestMove = move.cell;
        }
        alpha = std::max(alpha, bestScore);
    }

    Logger::log(std::format("[BotLv3] minimax depth={} candidates={} nodes={} -> move ({}, {}) score={}",
                            depth, moves.size(), visitedNodes, bestMove.first, bestMove.second, bestScore),
                Logger::Level::DEBUG);
    return bestMove;
}

/**
 * Mô tả:
 *   Minimax có cắt tỉa Alpha-Beta.
 *
 *   Cắt tỉa: nếu alpha >= beta thì người chơi ở tầng trên sẽ không bao giờ
 *   đi vào nhánh này nữa → dừng xét các nước còn lại.
 */
BotLevel3::Score BotLevel3::minimax(char board[][BOARD_N_MAX], const int size, const int goal,
                                    int depth, Score alpha, Score beta, bool isBotTurn, Score currentScore) {
    visitedNodes++;

    // 1. Hết độ sâu → trả về điểm heuristic
    if (depth == 0) return currentScore;

    // 2. Không còn nước đi → hòa
    std::vector<pII> candidates = getCandidateMoves(board, size);
    if (candidates.empty()) return 0;

    const char who = isBotTurn ? symbol : op_symbol;
    std::vector<ScoredMove> moves = orderMoves(board, size, goal, candidates, who, MAX_BRANCH);

    Score best = isBotTurn ? -INF : INF;

    for (const ScoredMove& move : moves) {
        const auto [row, col] = move.cell;
        makeMove(board, row, col, who);  // đặt thử

        Score value;
        if (isWinningMove(board, size, goal, row, col, who)) {
            // Thắng càng sớm (depth còn lại càng lớn) thì điểm càng cao
            value = isBotTurn ? (WIN_SCORE + depth) : -(WIN_SCORE + depth);
        } else {
            value = minimax(board, size, goal, depth - 1, alpha, beta, !isBotTurn, currentScore + move.gain);
        }

        board[row][col] = EMPTY_CELL;  // gỡ quân

        if (isBotTurn) {
            best = std::max(best, value);  // MAX: bot chọn điểm lớn nhất
            alpha = std::max(alpha, best);
        } else {
            best = std::min(best, value);  // MIN: đối thủ chọn điểm nhỏ nhất
            beta = std::min(beta, best);
        }

        if (alpha >= beta) break;  // cắt tỉa Alpha-Beta
    }
    return best;
}

/**
 * Mô tả:
 *   Heuristic cho toàn bộ bàn cờ: cộng điểm mọi cửa sổ dài `goal` ô theo 4 hướng.
 */
BotLevel3::Score BotLevel3::evaluateBoard(const char board[][BOARD_N_MAX], const int size, const int goal) const {
    Score total = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            for (const auto& dir : DIRECTIONS) {
                if (windowFits(size, goal, row, col, dir[0], dir[1])) {
                    total += scoreWindow(board, goal, row, col, dir[0], dir[1]);
                }
            }
        }
    }
    return total;
}

/**
 * Mô tả:
 *   Heuristic chỉ cho các cửa sổ có chứa ô (row, col).
 *   Với mỗi hướng, ô (row, col) có thể là ô thứ 0, 1, ..., goal-1 của cửa sổ
 *   → cửa sổ bắt đầu tại (row - k*dRow, col - k*dCol) với k = 0..goal-1.
 */
BotLevel3::Score BotLevel3::evaluateAround(const char board[][BOARD_N_MAX], const int size, const int goal,
                                           const int row, const int col) const {
    Score total = 0;
    for (const auto& dir : DIRECTIONS) {
        for (int k = 0; k < goal; k++) {
            const int startRow = row - k * dir[0];
            const int startCol = col - k * dir[1];
            if (windowFits(size, goal, startRow, startCol, dir[0], dir[1])) {
                total += scoreWindow(board, goal, startRow, startCol, dir[0], dir[1]);
            }
        }
    }
    return total;
}

/**
 * Mô tả:
 *   Đếm số quân mỗi bên trong cửa sổ rồi quy ra điểm.
 *   Ví dụ goal = 5:
 *     [X X X - -] (bot là X)  → +10^3 = +1000
 *     [O O - - -]             → -10^2 = -100
 *     [X O - - -]             →  0 (bị chặn, không ai thắng được ở đây)
 */
BotLevel3::Score BotLevel3::scoreWindow(const char board[][BOARD_N_MAX], const int goal,
                                        const int row, const int col, const int dRow, const int dCol) const {
    int botCount = 0;
    int opCount = 0;
    for (int i = 0; i < goal; i++) {
        const char cell = board[row + i * dRow][col + i * dCol];
        if (cell == symbol) botCount++;
        else if (cell == op_symbol) opCount++;
    }

    if (botCount > 0 && opCount > 0) return 0;
    if (botCount > 0) return powerOf10(botCount);
    if (opCount > 0) return -powerOf10(opCount);
    return 0;
}

/**
 * Mô tả:
 *   Sinh các nước đi đáng xét (xem mô tả ở header).
 */
std::vector<pII> BotLevel3::getCandidateMoves(const char board[][BOARD_N_MAX], const int size) const {
    const int radius = (size <= SMALL_BOARD) ? size : 1;  // bàn nhỏ: radius phủ cả bàn

    std::vector<pII> moves;
    bool boardIsEmpty = true;

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] != EMPTY_CELL) {
                boardIsEmpty = false;
                continue;
            }

            // ô trống này có quân nào trong phạm vi `radius` không?
            bool nearStone = false;
            for (int dr = -radius; dr <= radius && !nearStone; dr++) {
                for (int dc = -radius; dc <= radius && !nearStone; dc++) {
                    if (cellIs(board, size, row + dr, col + dc, symbol) ||
                        cellIs(board, size, row + dr, col + dc, op_symbol)) {
                        nearStone = true;
                    }
                }
            }
            if (nearStone) moves.push_back({row, col});
        }
    }

    if (boardIsEmpty) moves.push_back({size / 2, size / 2});  // nước đầu tiên: đánh giữa bàn
    return moves;
}

/**
 * Mô tả:
 *   Chấm điểm và sắp xếp nước đi cho người đang đi.
 *   gain = (điểm quanh ô sau khi đặt) - (điểm quanh ô trước khi đặt).
 *   Một nước tốt vừa tăng điểm cửa sổ của mình (tấn công),
 *   vừa xóa điểm cửa sổ của đối thủ (phòng thủ).
 */
std::vector<BotLevel3::ScoredMove> BotLevel3::orderMoves(char board[][BOARD_N_MAX], const int size, const int goal,
                                                         const std::vector<pII>& moves, const char who,
                                                         const size_t maxMoves) const {
    std::vector<ScoredMove> scored;
    for (const auto& [row, col] : moves) {
        const Score before = evaluateAround(board, size, goal, row, col);
        board[row][col] = who;
        const Score after = evaluateAround(board, size, goal, row, col);
        board[row][col] = EMPTY_CELL;

        scored.push_back({{row, col}, after - before});
    }

    // bot muốn gain lớn nhất, đối thủ muốn gain nhỏ nhất
    const bool isBot = (who == symbol);
    std::stable_sort(scored.begin(), scored.end(), [isBot](const ScoredMove& a, const ScoredMove& b) {
        return isBot ? (a.gain > b.gain) : (a.gain < b.gain);
    });

    if (scored.size() > maxMoves) scored.resize(maxMoves);
    return scored;
}

/**
 * Mô tả:
 *   Kiểm tra (row, col) vừa được `who` đánh có tạo đường thắng không.
 *   Với mỗi hướng:
 *     1. Tìm dãy quân `who` liên tiếp dài nhất đi qua (row, col): từ offset `first` đến `last`.
 *     2. Thử mọi đoạn dài `goal` nằm trong dãy đó; đoạn thắng nếu hai ô kẹp hai đầu
 *        không phải quân đối phương (giống EndRule::OPEN_TWO trong Logic::checkWin).
 */
bool BotLevel3::isWinningMove(const char board[][BOARD_N_MAX], const int size, const int goal,
                              const int row, const int col, const char who) const {
    const char enemy = (who == symbol) ? op_symbol : symbol;

    for (const auto& dir : DIRECTIONS) {
        const int dr = dir[0];
        const int dc = dir[1];

        // 1. Mở rộng về hai phía để tìm dãy quân liên tiếp
        int first = 0;
        while (cellIs(board, size, row + (first - 1) * dr, col + (first - 1) * dc, who)) first--;
        int last = 0;
        while (cellIs(board, size, row + (last + 1) * dr, col + (last + 1) * dc, who)) last++;

        // 2. Thử từng đoạn dài `goal` trong dãy [first, last]
        for (int start = first; start + goal - 1 <= last; start++) {
            const int end = start + goal - 1;
            const bool headBlocked = cellIs(board, size, row + (start - 1) * dr, col + (start - 1) * dc, enemy);
            const bool tailBlocked = cellIs(board, size, row + (end + 1) * dr, col + (end + 1) * dc, enemy);
            if (!headBlocked && !tailBlocked) return true;
        }
    }
    return false;
}

/**
 * Mô tả:
 *   Thử đặt quân `who` vào từng ô trống, ô nào tạo đường thắng thì trả về ô đó.
 */
pII BotLevel3::findWinningMove(char board[][BOARD_N_MAX], const int size, const int goal, const char who) const {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] != EMPTY_CELL) continue;

            board[row][col] = who;  // đặt thử
            const bool wins = isWinningMove(board, size, goal, row, col, who);
            board[row][col] = EMPTY_CELL;  // gỡ quân

            if (wins) return {row, col};
        }
    }
    return {-1, -1};
}

/**
 * Mô tả:
 *   Còn ít ô trống (bàn 3x3 hoặc cuối ván) → tìm đến hết ván để đánh hoàn hảo.
 *   Ngược lại dùng độ sâu mặc định.
 */
int BotLevel3::chooseDepth(const char board[][BOARD_N_MAX], const int size) const {
    int emptyCells = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board[row][col] == EMPTY_CELL) emptyCells++;
        }
    }
    return (emptyCells <= FULL_SEARCH_CELLS) ? emptyCells : DEFAULT_DEPTH;
}

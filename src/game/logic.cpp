/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>
#include <vector>
#include "../utils/logger.h"
using namespace std;
/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){board[i][j]='-';}
    }
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    // TODO: implement
    if(row>=0 && row<size && col>=0 &&col<size && board[row][col]=='-'){return true;}
    return false;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    // TODO: implement
    board[row][col]=symbol;
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
    // TODO: implement
    if((x>=0 && x<size && y>=0 && y<size && (board[x][y]=='-'||board[x][y]==symbol))||(x<0 || x>=size || y<0 || y>=size)){return true;}
    return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    // TODO: implement
    int count_cheo_chinh=1;
    int count_cheo_phu=1;
    int count_ngang=1;
    int count_doc=1;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            count_cheo_chinh=0;
            count_cheo_phu=0;
            count_ngang=0;
            count_doc=0;
            int checkdau=0;
            int y_1;int x_1;int y_2;int x_2;
            for(int u=0;u<=goal;u++){///// CHÉO CHÍNH TIẾN
                if(i+u>=0 && i+u<size && j+u>=0 && j+u<size && board[i+u][j+u]==symbol){    
                    count_cheo_chinh++;
                    if(count_cheo_chinh==goal && rule==EndRule::NONE){return true;}
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i+u;x_2=j+u;
                        if(isEmptyHead(board,size,y_2+1,x_2+1,symbol)||isEmptyHead(board,size,y_1-1,x_1-1,symbol)){return true;}
                    }
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i+u;int x_2=j+u;
                        if(isEmptyHead(board,size,y_2+1,x_2+1,symbol) && isEmptyHead(board,size,y_1-1,x_1-1,symbol)){return true;}
                    }
                }
                else{
                    y_1=i+u-1;x_1=j+u-1;
                    break;
                }
            }
            for(int u=1;u<goal;u++){///// CHÉO CHÍNH LUI
                if(i-u>=0 && i-u<size && j-u>=0 && j-u<size && board[i-u][j-u]==symbol){
                    count_cheo_chinh++;
                    if(count_cheo_chinh==goal && rule==EndRule::NONE){return true;}
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_ONE){
                        y_2=i-u;x_2=j-u;
                        if(isEmptyHead(board,size,y_1+1,x_1+1,symbol)||isEmptyHead(board,size,y_2-1,x_2-1,symbol)){return true;}
                    }
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_TWO){
                        int y_2=i-u;int x_2=j-u;
                        if(isEmptyHead(board,size,y_1+1,x_1+1,symbol) && isEmptyHead(board,size,y_2-1,x_2-1,symbol)){return true;}
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal;u++){///// CHÉO PHỤ TIẾN
                if(i+u>=0 && i+u<size && j-u>=0 && j-u<size && board[i+u][j-u]==symbol){
                    count_cheo_phu++;
                    if(count_cheo_phu==goal && rule==EndRule::NONE){return true;}
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i+u;x_2=j-u;
                        if(isEmptyHead(board,size,y_2+1,x_2-1,symbol)||isEmptyHead(board,size,y_1-1,x_1+1,symbol)){return true;}
                    }
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i+u;int x_2=j-u;
                        if(isEmptyHead(board,size,y_2+1,x_2-1,symbol) && isEmptyHead(board,size,y_1-1,x_1+1,symbol)){return true;}
                    }
                }
                else{
                    y_1=i+u-1;x_1=j-u+1;
                    break;}
            }
            for(int u=1;u<goal;u++){///// CHÉO PHỤ LUI
                if(i-u>=0 && i-u<size && j+u>=0 && j+u<size && board[i-u][j+u]==symbol){
                    count_cheo_phu++;
                    if(count_cheo_phu==goal && rule==EndRule::NONE){return true;}
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_ONE){
                        y_2=i-u;x_2=j+u;
                        if(isEmptyHead(board,size,y_1+1,x_1-1,symbol)||isEmptyHead(board,size,y_2-1,x_2+1,symbol)){return true;}
                    }
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_TWO){
                        int y_2=i-u;int x_2=j+u;
                        if(isEmptyHead(board,size,y_1+1,x_1-1,symbol) && isEmptyHead(board,size,y_2-1,x_2+1,symbol)){return true;}
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal;u++){//// NGANG TIẾN
                if(i>=0 && i<size && j+u>=0 && j+u<size && board[i][j+u]==symbol){
                    count_ngang++;
                    if(count_ngang==goal && rule==EndRule::NONE){return true;}
                    else if(count_ngang==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i;x_2=j+u;
                        if(isEmptyHead(board,size,y_1,x_1-1,symbol)||isEmptyHead(board,size,y_2,x_2+1,symbol)){return true;}
                    }
                    else if(count_ngang==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i;int x_2=j+u;
                        if(isEmptyHead(board,size,y_1,x_1-1,symbol) && isEmptyHead(board,size,y_2,x_2+1,symbol)){return true;}
                    }
                }
                else{
                    y_1=i;x_1=j+u-1;
                    break;}
            }
            for(int u=1;u<goal;u++){/// NGANG LUI
                if(i>=0 && i<size && j-u>=0 && j-u<size && board[i][j-u]==symbol){
                    count_ngang++;
                    if(count_ngang==goal && rule==EndRule::NONE){return true;}
                    else if(count_ngang==goal && rule==EndRule::OPEN_ONE){
                        y_2=i;x_2=j-u;
                        if(isEmptyHead(board,size,y_1,x_1+1,symbol)||isEmptyHead(board,size,y_2,x_2-1,symbol)){return true;}
                    }
                    else if(count_ngang==goal && rule==EndRule::OPEN_TWO){
                        int y_2=i;int x_2=j-u;
                        if(isEmptyHead(board,size,y_1,x_1+1,symbol) && isEmptyHead(board,size,y_2,x_2-1,symbol)){return true;}
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal;u++){//// DỌC TIẾN
                if(i+u>=0 && i+u<size && j>=0 && j<size && board[i+u][j]==symbol){
                    count_doc++;
                    if(count_doc==goal && rule==EndRule::NONE){return true;}
                    else if(count_doc==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i+u;x_2=j;
                        if(isEmptyHead(board,size,y_1-1,x_1,symbol)||isEmptyHead(board,size,y_2+1,x_2,symbol)){return true;}
                    }
                    else if(count_doc==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i+u;int x_2=j;
                        if(isEmptyHead(board,size,y_1-1,x_1,symbol) && isEmptyHead(board,size,y_2+1,x_2,symbol)){return true;}
                    }
                }
                else{
                    y_1=i+u-1;x_1=j;
                    break;
                }
            }
            for(int u=1 ;u<goal;u++){//DỌC LUI
                if(i-u>=0 && i-u<size && j>=0 && j<size && board[i-u][j]==symbol){
                    count_doc++;
                    if(count_doc==goal && rule==EndRule::NONE){return true;}
                    else if(count_doc==goal && rule==EndRule::OPEN_ONE){
                         y_2=i-u;x_2=j;
                        if(isEmptyHead(board,size,y_1+1,x_1,symbol)||isEmptyHead(board,size,y_2-1,x_2,symbol)){return true;}
                    }
                    else if(count_doc==goal && rule==EndRule::OPEN_TWO){
                        y_2=i-u;x_2=j;
                        if(isEmptyHead(board,size,y_1+1,x_1,symbol) && isEmptyHead(board,size,y_2-1,x_2,symbol)){return true;}
                    }
                }
                else{break;}
            }
        }
    }

    return false;
}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(board[i][j]=='-'){return false;}
        }
    }
    return true;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {
    int count_cheo_chinh=1;
    int count_cheo_phu=1;
    int count_ngang=1;
    int count_doc=1;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            count_cheo_chinh=0;
            count_cheo_phu=0;
            count_ngang=0;
            count_doc=0;
            int checkdau=0;
            int y_1;int x_1;int y_2;int x_2;
            std::vector<std::pair<int, int>> cheochinh;
            for(int u=0;u<=goal;u++){///// CHÉO CHÍNH TIẾN
                if(i+u>=0 && i+u<size && j+u>=0 && j+u<size && board[i+u][j+u]==symbol){    
                    count_cheo_chinh++;
                    cheochinh.push_back({i+u,j+u});
                    if(count_cheo_chinh==goal && rule==EndRule::NONE){return WinLine{cheochinh};}
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i+u;x_2=j+u;
                        if(isEmptyHead(board,size,y_2+1,x_2+1,symbol)||isEmptyHead(board,size,y_1-1,x_1-1,symbol)){return WinLine{cheochinh};}
                    }
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i+u;int x_2=j+u;
                        if(isEmptyHead(board,size,y_2+1,x_2+1,symbol) && isEmptyHead(board,size,y_1-1,x_1-1,symbol)){return WinLine{cheochinh};}
                    }
                }
                else{
                    y_1=i+u-1;x_1=j+u-1;
                    break;
                }
            }
            for(int u=1;u<goal;u++){///// CHÉO CHÍNH LUI
                if(i-u>=0 && i-u<size && j-u>=0 && j-u<size && board[i-u][j-u]==symbol){
                    count_cheo_chinh++;
                    cheochinh.push_back({i-u,j-u});
                    if(count_cheo_chinh==goal && rule==EndRule::NONE){return WinLine{cheochinh};}
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_ONE){
                        y_2=i-u;x_2=j-u;
                        if(isEmptyHead(board,size,y_1+1,x_1+1,symbol)||isEmptyHead(board,size,y_2-1,x_2-1,symbol)){return WinLine{cheochinh};}
                    }
                    else if(count_cheo_chinh==goal && rule==EndRule::OPEN_TWO){
                        int y_2=i-u;int x_2=j-u;
                        if(isEmptyHead(board,size,y_1+1,x_1+1,symbol) && isEmptyHead(board,size,y_2-1,x_2-1,symbol)){return WinLine{cheochinh};}
                    }
                }
                else{break;}
            }
            std::vector<std::pair<int, int>> cheophu;
            for(int u=0;u<=goal;u++){///// CHÉO PHỤ TIẾN
                if(i+u>=0 && i+u<size && j-u>=0 && j-u<size && board[i+u][j-u]==symbol){
                    count_cheo_phu++;
                    cheophu.push_back({i+u,j-u});
                    if(count_cheo_phu==goal && rule==EndRule::NONE){return WinLine{cheophu};}
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i+u;x_2=j-u;
                        if(isEmptyHead(board,size,y_2+1,x_2-1,symbol)||isEmptyHead(board,size,y_1-1,x_1+1,symbol)){return WinLine{cheophu};}
                    }
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i+u;int x_2=j-u;
                        if(isEmptyHead(board,size,y_2+1,x_2-1,symbol) && isEmptyHead(board,size,y_1-1,x_1+1,symbol)){return WinLine{cheophu};}
                    }
                }
                else{
                    y_1=i+u-1;x_1=j-u+1;
                    break;}
            }
            for(int u=1;u<goal;u++){///// CHÉO PHỤ LUI
                if(i-u>=0 && i-u<size && j+u>=0 && j+u<size && board[i-u][j+u]==symbol){
                    count_cheo_phu++;
                    cheophu.push_back({i-u,j+u});
                    if(count_cheo_phu==goal && rule==EndRule::NONE){return WinLine{cheophu};}
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_ONE){
                        y_2=i-u;x_2=j+u;
                        if(isEmptyHead(board,size,y_1+1,x_1-1,symbol)||isEmptyHead(board,size,y_2-1,x_2+1,symbol)){return WinLine{cheophu};}
                    }
                    else if(count_cheo_phu==goal && rule==EndRule::OPEN_TWO){
                        int y_2=i-u;int x_2=j+u;
                        if(isEmptyHead(board,size,y_1+1,x_1-1,symbol) && isEmptyHead(board,size,y_2-1,x_2+1,symbol)){return WinLine{cheophu};}
                    }
                }
                else{break;}
            }
            std::vector<std::pair<int, int>> ngang;
            for(int u=0;u<=goal;u++){//// NGANG TIẾN
                if(i>=0 && i<size && j+u>=0 && j+u<size && board[i][j+u]==symbol){
                    count_ngang++;
                    ngang.push_back({i,j+u});
                    if(count_ngang==goal && rule==EndRule::NONE){return WinLine{ngang};}
                    else if(count_ngang==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i;x_2=j+u;
                        if(isEmptyHead(board,size,y_1,x_1-1,symbol)||isEmptyHead(board,size,y_2,x_2+1,symbol)){return WinLine{ngang};}
                    }
                    else if(count_ngang==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i;int x_2=j+u;
                        if(isEmptyHead(board,size,y_1,x_1-1,symbol) && isEmptyHead(board,size,y_2,x_2+1,symbol)){return WinLine{ngang};}
                    }
                }
                else{
                    y_1=i;x_1=j+u-1;
                    break;}
            }
            for(int u=1;u<goal;u++){/// NGANG LUI
                if(i>=0 && i<size && j-u>=0 && j-u<size && board[i][j-u]==symbol){
                    count_ngang++;
                    ngang.push_back({i,j-u});
                    if(count_ngang==goal && rule==EndRule::NONE){return WinLine{ngang};}
                    else if(count_ngang==goal && rule==EndRule::OPEN_ONE){
                        y_2=i;x_2=j-u;
                        if(isEmptyHead(board,size,y_1,x_1+1,symbol)||isEmptyHead(board,size,y_2,x_2-1,symbol)){return WinLine{ngang};}
                    }
                    else if(count_ngang==goal && rule==EndRule::OPEN_TWO){
                        int y_2=i;int x_2=j-u;
                        if(isEmptyHead(board,size,y_1,x_1+1,symbol) && isEmptyHead(board,size,y_2,x_2-1,symbol)){return WinLine{ngang};}
                    }
                }
                else{break;}
            }
            std::vector<std::pair<int, int>> doc;
            for(int u=0;u<=goal;u++){//// DỌC TIẾN
                if(i+u>=0 && i+u<size && j>=0 && j<size && board[i+u][j]==symbol){
                    count_doc++;
                    doc.push_back({i+u,j});
                    if(count_doc==goal && rule==EndRule::NONE){return WinLine{doc};}
                    else if(count_doc==goal && rule==EndRule::OPEN_ONE){
                        y_1=i;x_1=j;y_2=i+u;x_2=j;
                        if(isEmptyHead(board,size,y_1-1,x_1,symbol)||isEmptyHead(board,size,y_2+1,x_2,symbol)){return WinLine{doc};}
                    }
                    else if(count_doc==goal && rule==EndRule::OPEN_TWO){
                        int y_1=i;int x_1=j;int y_2=i+u;int x_2=j;
                        if(isEmptyHead(board,size,y_1-1,x_1,symbol) && isEmptyHead(board,size,y_2+1,x_2,symbol)){return WinLine{doc};}
                    }
                }
                else{
                    y_1=i+u-1;x_1=j;
                    break;
                }
            }
            for(int u=1 ;u<goal;u++){//DỌC LUI
                if(i-u>=0 && i-u<size && j>=0 && j<size && board[i-u][j]==symbol){
                    count_doc++;
                    doc.push_back({i-u,j});
                    if(count_doc==goal && rule==EndRule::NONE){return WinLine{doc};}
                    else if(count_doc==goal && rule==EndRule::OPEN_ONE){
                         y_2=i-u;x_2=j;
                        if(isEmptyHead(board,size,y_1+1,x_1,symbol)||isEmptyHead(board,size,y_2-1,x_2,symbol)){return WinLine{doc};}
                    }
                    else if(count_doc==goal && rule==EndRule::OPEN_TWO){
                        y_2=i-u;x_2=j;
                        if(isEmptyHead(board,size,y_1+1,x_1,symbol) && isEmptyHead(board,size,y_2-1,x_2,symbol)){return WinLine{doc};}
                    }
                }
                else{break;}
            }
        }
    }
    return std::nullopt;
}


bool Logic::isvalid(int row,int col,const int size){
    if(row>=0 && row<size && col>=0 && col<size){return true;}
    return false;
}
bool Logic::empty(char board[][BOARD_N_MAX],int row,int col,const int size){
    if(row>=0 && row<size && col>=0 && col<size &&board[row][col]=='-'){return true;}
    return false;
}
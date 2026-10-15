/**
 * Bot LV2 cpp implementation
 *
 */

#include "bot_lv2.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"
using namespace Logic;
/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel2.
 */
BotLevel2::BotLevel2(const BotLevel& _level, const char& _symbol)
    : BotLevel1(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel2.
 */
BotLevel2::~BotLevel2() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot MEDIUM.
 *
 * TODO:
 *   - Gọi heuristic để chọn nước đi
 */
pII BotLevel2::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    // TODO: gọi simple_heuristic với symbol và op_symbol
    return simple_heuristic(board,size, goal, symbol, op_symbol);
}

/**
 * Mô tả:
 *   Heuristic đơn giản:
 *   - Ưu tiên thắng
 *   - Chặn đối thủ
 *   - Fallback random
 *
 * TODO:
 *   - Bước 1: kiểm tra nước thắng
 *   - Bước 2: kiểm tra nước chặn
 *   - Bước 3: chọn random nếu không có
 */
pII BotLevel2::simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol) {
    // TODO: implement heuristic
    int count_cheo_chinh;
    int count_cheo_phu;
    int count_ngang;
    int count_doc;
    int arr[2];
        for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            count_cheo_chinh=0;
            count_cheo_phu=0;
            count_ngang=0;
            count_doc=0;
            int checkdau=0;
            int y_1;int x_1;int y_2;int x_2;
            for(int u=0;u<=goal-1;u++){///// CHÉO CHÍNH TIẾN
                if(i+u>=0 && i+u<size && j+u>=0 && j+u<size && board[i+u][j+u]==botSymbol){    
                    count_cheo_chinh++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j+u;
                    if(count_cheo_chinh==goal-1&&(isEmptyHead(board,size,y_2+1,x_2+1,botSymbol)||isEmptyHead(board,size,y_1-1,x_1-1,botSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2+1,botSymbol)&&isEmptyHead(board,size,y_1-1,x_1-1,botSymbol)&&isValidMove(board,size,y_2+1,x_2+1) && isValidMove(board,size,y_1-1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2+1,botSymbol)&&isValidMove(board,size,y_2+1,x_2+1)){
                            arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1-1,botSymbol)&&isValidMove(board,size,y_1-1,x_1-1)){
                            arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j+u+1;
                    break;
                }
            }
            for(int u=1;u<goal-1;u++){///// CHÉO CHÍNH LUI
                if(i-u>=0 && i-u<size && j-u>=0 && j-u<size && board[i-u][j-u]==botSymbol){
                    count_cheo_chinh++;
                    y_2=i-u;x_2=j-u;
                    if(count_cheo_chinh==goal-1&&(isEmptyHead(board,size,y_2-1,x_2-1,botSymbol)||isEmptyHead(board,size,y_1+1,x_1+1,botSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2-1,botSymbol)&&isEmptyHead(board,size,y_1+1,x_1+1,botSymbol)&&isValidMove(board,size,y_2-1,x_2-1) && isValidMove(board,size,y_1+1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2-1,botSymbol)&&isValidMove(board,size,y_2-1,x_2-1)){
                            arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1+1,botSymbol)&&isValidMove(board,size,y_1+1,x_1+1)){
                            arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}//////xong cho con bot đoạn chéo chính rồi
            }
            for(int u=0;u<=goal-1;u++){///// CHÉO PHỤ TIẾN
                if(i+u>=0 && i+u<size && j-u>=0 && j-u<size && board[i+u][j-u]==botSymbol){
                    count_cheo_phu++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j-u;
                    if(count_cheo_phu==goal-1&&(isEmptyHead(board,size,y_2+1,x_2-1,botSymbol)||isEmptyHead(board,size,y_1-1,x_1+1,botSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2-1,botSymbol)&&isEmptyHead(board,size,y_1-1,x_1+1,botSymbol)&&isValidMove(board,size,y_2+1,x_2-1) && isValidMove(board,size,y_1-1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2-1,botSymbol)&&isValidMove(board,size,y_2+1,x_2-1)){
                            arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1+1,botSymbol)&&isValidMove(board,size,y_1-1,x_1+1)){
                            arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j-u+1;
                    break;}
            }
            for(int u=1;u<goal-1;u++){///// CHÉO PHỤ LUI
                if(i-u>=0 && i-u<size && j+u>=0 && j+u<size && board[i-u][j+u]==botSymbol){
                        y_2=i-u;x_2=j+u;
                    if(count_cheo_phu==goal-1&&(isEmptyHead(board,size,y_2-1,x_2+1,botSymbol)||isEmptyHead(board,size,y_1+1,x_1-1,botSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2+1,botSymbol)&&isEmptyHead(board,size,y_1+1,x_1-1,botSymbol)&&isValidMove(board,size,y_2-1,x_2+1) && isValidMove(board,size,y_1+1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2+1,botSymbol)&&isValidMove(board,size,y_2-1,x_2+1)){
                            arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);;
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1-1,botSymbol)&&isValidMove(board,size,y_1+1,x_1-1)){
                            arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);;
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-1;u++){//// NGANG TIẾN
                if(i>=0 && i<size && j+u>=0 && j+u<size && board[i][j+u]==botSymbol){
                    count_ngang++;
                    y_1=i;x_1=j;y_2=i;x_2=j+u;
                    if(count_ngang==goal-1&&(isEmptyHead(board,size,y_2,x_2+1,botSymbol)||isEmptyHead(board,size,y_1,x_1-1,botSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2+1,botSymbol)&&isEmptyHead(board,size,y_1,x_1-1,botSymbol)&&isValidMove(board,size,y_2,x_2+1) && isValidMove(board,size,y_1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2+1,botSymbol)&&isValidMove(board,size,y_2,x_2+1)){
                            arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1-1,botSymbol)&&isValidMove(board,size,y_1,x_1-1)){
                            arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i;x_1=j+u-1;
                    break;}
            }
            for(int u=1;u<goal-1;u++){/// NGANG LUI
                if(i>=0 && i<size && j-u>=0 && j-u<size && board[i][j-u]==botSymbol){
                    count_ngang++;
                    y_2=i;x_2=j-u;
                    if(count_ngang==goal-1&&(isEmptyHead(board,size,y_2,x_2-1,botSymbol)||isEmptyHead(board,size,y_1,x_1+1,botSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2-1,botSymbol)&&isEmptyHead(board,size,y_1,x_1+1,botSymbol)&&isValidMove(board,size,y_2,x_2-1) && isValidMove(board,size,y_1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2-1,botSymbol)&&isValidMove(board,size,y_2,x_2-1)){
                            arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1+1,botSymbol)&&isValidMove(board,size,y_1,x_1+1)){
                            arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-1;u++){//// DỌC TIẾN
                if(i+u>=0 && i+u<size && j>=0 && j<size && board[i+u][j]==botSymbol){
                    count_doc++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j;
                    if(count_doc==goal-1&&(isEmptyHead(board,size,y_2+1,x_2,botSymbol)||isEmptyHead(board,size,y_1-1,x_1,botSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2,botSymbol)&&isEmptyHead(board,size,y_1-1,x_1,botSymbol)&&isValidMove(board,size,y_2+1,x_2) && isValidMove(board,size,y_1-1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2,botSymbol)&&isValidMove(board,size,y_2+1,x_2)){
                            arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1,botSymbol)&&isValidMove(board,size,y_1-1,x_1)){
                            arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j;
                    break;
                }
            }
            for(int u=1 ;u<=goal-1;u++){//DỌC LUI
                if(i-u>=0 && i-u<size && j>=0 && j<size && board[i-u][j]==botSymbol){
                    count_doc++;
                    y_2=i-u;x_2=j;
                    if(count_doc==goal-1&&(isEmptyHead(board,size,y_2-1,x_2,botSymbol)||isEmptyHead(board,size,y_1+1,x_1,botSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2,botSymbol)&&isEmptyHead(board,size,y_1+1,x_1,botSymbol)&&isValidMove(board,size,y_2-1,x_2) && isValidMove(board,size,y_1+1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2,botSymbol)&&isValidMove(board,size,y_2-1,x_2)){
                            arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1,botSymbol)&&isValidMove(board,size,y_1+1,x_1)){
                            arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
        }
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            count_cheo_chinh=0;
            count_cheo_phu=0;
            count_ngang=0;
            count_doc=0;
            int checkdau=0;
            int y_1;int x_1;int y_2;int x_2;
            for(int u=0;u<=goal-1;u++){///// CHÉO CHÍNH TIẾN
                if(i+u>=0 && i+u<size && j+u>=0 && j+u<size && board[i+u][j+u]==playerSymbol){    
                    count_cheo_chinh++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j+u;
                    if(count_cheo_chinh==goal-1&&(isEmptyHead(board,size,y_2+1,x_2+1,playerSymbol)||isEmptyHead(board,size,y_1-1,x_1-1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2+1,playerSymbol)&&isEmptyHead(board,size,y_1-1,x_1-1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2+1) && isValidMove(board,size,y_1-1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2+1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2+1)){
                            arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1-1,playerSymbol)&&isValidMove(board,size,y_1-1,x_1-1)){
                            arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j+u+1;
                    break;
                }
            }
            for(int u=1;u<goal-1;u++){///// CHÉO CHÍNH LUI
                if(i-u>=0 && i-u<size && j-u>=0 && j-u<size && board[i-u][j-u]==playerSymbol){
                    count_cheo_chinh++;
                    y_2=i-u;x_2=j-u;
                    if(count_cheo_chinh==goal-1&&(isEmptyHead(board,size,y_2-1,x_2-1,playerSymbol)||isEmptyHead(board,size,y_1+1,x_1+1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2-1,playerSymbol)&&isEmptyHead(board,size,y_1+1,x_1+1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2-1) && isValidMove(board,size,y_1+1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2-1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2-1)){
                            arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1+1,playerSymbol)&&isValidMove(board,size,y_1+1,x_1+1)){
                            arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}//////xong cho con bot đoạn chéo chính rồi
            }
            for(int u=0;u<=goal-1;u++){///// CHÉO PHỤ TIẾN
                if(i+u>=0 && i+u<size && j-u>=0 && j-u<size && board[i+u][j-u]==playerSymbol){
                    count_cheo_phu++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j-u;
                    if(count_cheo_phu==goal-1&&(isEmptyHead(board,size,y_2+1,x_2-1,playerSymbol)||isEmptyHead(board,size,y_1-1,x_1+1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2-1,playerSymbol)&&isEmptyHead(board,size,y_1-1,x_1+1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2-1) && isValidMove(board,size,y_1-1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2-1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2-1)){
                            arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1+1,playerSymbol)&&isValidMove(board,size,y_1-1,x_1+1)){
                            arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j-u+1;
                    break;}
            }
            for(int u=1;u<goal-1;u++){///// CHÉO PHỤ LUI
                if(i-u>=0 && i-u<size && j+u>=0 && j+u<size && board[i-u][j+u]==playerSymbol){
                    count_cheo_phu++;
                    y_2=i-u;x_2=j+u;
                    if(count_cheo_phu==goal-1&&(isEmptyHead(board,size,y_2-1,x_2+1,playerSymbol)||isEmptyHead(board,size,y_1+1,x_1-1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2+1,playerSymbol)&&isEmptyHead(board,size,y_1+1,x_1-1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2+1) && isValidMove(board,size,y_1+1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2+1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2+1)){
                            arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);;
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1-1,playerSymbol)&&isValidMove(board,size,y_1+1,x_1-1)){
                            arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);;
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-1;u++){//// NGANG TIẾN
                if(i>=0 && i<size && j+u>=0 && j+u<size && board[i][j+u]==playerSymbol){
                    count_ngang++;
                    y_1=i;x_1=j;y_2=i;x_2=j+u;
                    if(count_ngang==goal-1&&(isEmptyHead(board,size,y_2,x_2+1,playerSymbol)||isEmptyHead(board,size,y_1,x_1-1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2+1,playerSymbol)&&isEmptyHead(board,size,y_1,x_1-1,playerSymbol)&&isValidMove(board,size,y_2,x_2+1) && isValidMove(board,size,y_1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2+1,playerSymbol)&&isValidMove(board,size,y_2,x_2+1)){
                            arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1-1,playerSymbol)&&isValidMove(board,size,y_1,x_1-1)){
                            arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i;x_1=j+u-1;
                    break;}
            }
            for(int u=1;u<goal-1;u++){/// NGANG LUI
                if(i>=0 && i<size && j-u>=0 && j-u<size && board[i][j-u]==playerSymbol){
                    count_ngang++;
                    y_2=i;x_2=j-u;
                    if(count_ngang==goal-1&&(isEmptyHead(board,size,y_2,x_2-1,playerSymbol)||isEmptyHead(board,size,y_1,x_1+1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2-1,playerSymbol)&&isEmptyHead(board,size,y_1,x_1+1,playerSymbol)&&isValidMove(board,size,y_2,x_2-1) && isValidMove(board,size,y_1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2-1,playerSymbol)&&isValidMove(board,size,y_2,x_2-1)){
                            arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1+1,playerSymbol)&&isValidMove(board,size,y_1,x_1+1)){
                            arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-1;u++){//// DỌC TIẾN
                if(i+u>=0 && i+u<size && j>=0 && j<size && board[i+u][j]==playerSymbol){
                    count_doc++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j;
                    if(count_doc==goal-1&&(isEmptyHead(board,size,y_2+1,x_2,playerSymbol)||isEmptyHead(board,size,y_1-1,x_1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2,playerSymbol)&&isEmptyHead(board,size,y_1-1,x_1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2) && isValidMove(board,size,y_1-1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2,playerSymbol)&&isValidMove(board,size,y_2+1,x_2)){
                            arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1,playerSymbol)&&isValidMove(board,size,y_1-1,x_1)){
                            arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j;
                    break;
                }
            }
            for(int u=1 ;u<=goal-1;u++){//DỌC LUI
                if(i-u>=0 && i-u<size && j>=0 && j<size && board[i-u][j]==playerSymbol){
                    count_doc++;
                    y_2=i-u;x_2=j;
                    if(count_doc==goal-1&&(isEmptyHead(board,size,y_2-1,x_2,playerSymbol)||isEmptyHead(board,size,y_1+1,x_1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2,playerSymbol)&&isEmptyHead(board,size,y_1+1,x_1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2) && isValidMove(board,size,y_1+1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2,playerSymbol)&&isValidMove(board,size,y_2-1,x_2)){
                            arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1,playerSymbol)&&isValidMove(board,size,y_1+1,x_1)){
                            arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
        }
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            count_cheo_chinh=0;
            count_cheo_phu=0;
            count_ngang=0;
            count_doc=0;
            int checkdau=0;
            int y_1;int x_1;int y_2;int x_2;
            for(int u=0;u<=goal-2;u++){///// CHÉO CHÍNH TIẾN
                if(i+u>=0 && i+u<size && j+u>=0 && j+u<size && board[i+u][j+u]==playerSymbol){    
                    count_cheo_chinh++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j+u;
                    if(count_cheo_chinh==goal-2&&(isEmptyHead(board,size,y_2+1,x_2+1,playerSymbol)||isEmptyHead(board,size,y_1-1,x_1-1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2+1,playerSymbol)&&isEmptyHead(board,size,y_1-1,x_1-1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2+1) && isValidMove(board,size,y_1-1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2+1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2+1)){
                            arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1-1,playerSymbol)&&isValidMove(board,size,y_1-1,x_1-1)){
                            arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j+u+1;
                    break;
                }
            }
            for(int u=1;u<goal-2;u++){///// CHÉO CHÍNH LUI
                if(i-u>=0 && i-u<size && j-u>=0 && j-u<size && board[i-u][j-u]==playerSymbol){
                    count_cheo_chinh++;
                    y_2=i-u;x_2=j-u;
                    if(count_cheo_chinh==goal-2&&(isEmptyHead(board,size,y_2-1,x_2-1,playerSymbol)||isEmptyHead(board,size,y_1+1,x_1+1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2-1,playerSymbol)&&isEmptyHead(board,size,y_1+1,x_1+1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2-1) && isValidMove(board,size,y_1+1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2-1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2-1)){
                            arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1+1,playerSymbol)&&isValidMove(board,size,y_1+1,x_1+1)){
                            arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}//////xong cho con bot đoạn chéo chính rồi
            }
            for(int u=0;u<=goal-2;u++){///// CHÉO PHỤ TIẾN
                if(i+u>=0 && i+u<size && j-u>=0 && j-u<size && board[i+u][j-u]==playerSymbol){
                    count_cheo_phu++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j-u;
                    if(count_cheo_phu==goal-2&&(isEmptyHead(board,size,y_2+1,x_2-1,playerSymbol)||isEmptyHead(board,size,y_1-1,x_1+1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2-1,playerSymbol)&&isEmptyHead(board,size,y_1-1,x_1+1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2-1) && isValidMove(board,size,y_1-1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2-1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2-1)){
                            arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1+1,playerSymbol)&&isValidMove(board,size,y_1-1,x_1+1)){
                            arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j-u+1;
                    break;}
            }
            for(int u=1;u<goal-2;u++){///// CHÉO PHỤ LUI
                if(i-u>=0 && i-u<size && j+u>=0 && j+u<size && board[i-u][j+u]==playerSymbol){
                    count_cheo_phu++;
                    y_2=i-u;x_2=j+u;
                    if(count_cheo_phu==goal-2&&(isEmptyHead(board,size,y_2-1,x_2+1,playerSymbol)||isEmptyHead(board,size,y_1+1,x_1-1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2+1,playerSymbol)&&isEmptyHead(board,size,y_1+1,x_1-1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2+1) && isValidMove(board,size,y_1+1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2+1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2+1)){
                            arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);;
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1-1,playerSymbol)&&isValidMove(board,size,y_1+1,x_1-1)){
                            arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);;
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-2;u++){//// NGANG TIẾN
                if(i>=0 && i<size && j+u>=0 && j+u<size && board[i][j+u]==playerSymbol){
                    count_ngang++;
                    y_1=i;x_1=j;y_2=i;x_2=j+u;
                    if(count_ngang==goal-2&&(isEmptyHead(board,size,y_2,x_2+1,playerSymbol)||isEmptyHead(board,size,y_1,x_1-1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2+1,playerSymbol)&&isEmptyHead(board,size,y_1,x_1-1,playerSymbol)&&isValidMove(board,size,y_2,x_2+1) && isValidMove(board,size,y_1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2+1,playerSymbol)&&isValidMove(board,size,y_2,x_2+1)){
                            arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1-1,playerSymbol)&&isValidMove(board,size,y_1,x_1-1)){
                            arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i;x_1=j+u-1;
                    break;}
            }
            for(int u=1;u<goal-2;u++){/// NGANG LUI
                if(i>=0 && i<size && j-u>=0 && j-u<size && board[i][j-u]==playerSymbol){
                    count_ngang++;
                    y_2=i;x_2=j-u;
                    if(count_ngang==goal-2&&(isEmptyHead(board,size,y_2,x_2-1,playerSymbol)||isEmptyHead(board,size,y_1,x_1+1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2-1,playerSymbol)&&isEmptyHead(board,size,y_1,x_1+1,playerSymbol)&&isValidMove(board,size,y_2,x_2-1) && isValidMove(board,size,y_1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2-1,playerSymbol)&&isValidMove(board,size,y_2,x_2-1)){
                            arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1+1,playerSymbol)&&isValidMove(board,size,y_1,x_1+1)){
                            arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-2;u++){//// DỌC TIẾN
                if(i+u>=0 && i+u<size && j>=0 && j<size && board[i+u][j]==playerSymbol){
                    count_doc++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j;
                    if(count_doc==goal-2&&(isEmptyHead(board,size,y_2+1,x_2,playerSymbol)||isEmptyHead(board,size,y_1-1,x_1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2,playerSymbol)&&isEmptyHead(board,size,y_1-1,x_1,playerSymbol)&&isValidMove(board,size,y_2+1,x_2) && isValidMove(board,size,y_1-1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2,playerSymbol)&&isValidMove(board,size,y_2+1,x_2)){
                            arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1,playerSymbol)&&isValidMove(board,size,y_1-1,x_1)){
                            arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j;
                    break;
                }
            }
            for(int u=1 ;u<=goal-2;u++){//DỌC LUI
                if(i-u>=0 && i-u<size && j>=0 && j<size && board[i-u][j]==playerSymbol){
                    count_doc++;
                    y_2=i-u;x_2=j;
                    if(count_doc==goal-2&&(isEmptyHead(board,size,y_2-1,x_2,playerSymbol)||isEmptyHead(board,size,y_1+1,x_1,playerSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2,playerSymbol)&&isEmptyHead(board,size,y_1+1,x_1,playerSymbol)&&isValidMove(board,size,y_2-1,x_2) && isValidMove(board,size,y_1+1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2,playerSymbol)&&isValidMove(board,size,y_2-1,x_2)){
                            arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1,playerSymbol)&&isValidMove(board,size,y_1+1,x_1)){
                            arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
        }
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            count_cheo_chinh=0;
            count_cheo_phu=0;
            count_ngang=0;
            count_doc=0;
            int checkdau=0;
            int y_1;int x_1;int y_2;int x_2;
            for(int u=0;u<=goal-2;u++){///// CHÉO CHÍNH TIẾN
                if(i+u>=0 && i+u<size && j+u>=0 && j+u<size && board[i+u][j+u]==botSymbol){    
                    count_cheo_chinh++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j+u;
                    if(count_cheo_chinh==goal-2&&(isEmptyHead(board,size,y_2+1,x_2+1,botSymbol)||isEmptyHead(board,size,y_1-1,x_1-1,botSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2+1,botSymbol)&&isEmptyHead(board,size,y_1-1,x_1-1,botSymbol)&&isValidMove(board,size,y_2+1,x_2+1) && isValidMove(board,size,y_1-1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2+1,botSymbol)&&isValidMove(board,size,y_2+1,x_2+1)){
                            arr[0]=y_2+1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1-1,botSymbol)&&isValidMove(board,size,y_1-1,x_1-1)){
                            arr[0]=y_1-1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j+u+1;
                    break;
                }
            }
            for(int u=1;u<goal-2;u++){///// CHÉO CHÍNH LUI
                if(i-u>=0 && i-u<size && j-u>=0 && j-u<size && board[i-u][j-u]==botSymbol){
                    count_cheo_chinh++;
                    y_2=i-u;x_2=j-u;
                    if(count_cheo_chinh==goal-2&&(isEmptyHead(board,size,y_2-1,x_2-1,botSymbol)||isEmptyHead(board,size,y_1+1,x_1+1,botSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2-1,botSymbol)&&isEmptyHead(board,size,y_1+1,x_1+1,botSymbol)&&isValidMove(board,size,y_2-1,x_2-1) && isValidMove(board,size,y_1+1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2-1,botSymbol)&&isValidMove(board,size,y_2-1,x_2-1)){
                            arr[0]=y_2-1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1+1,botSymbol)&&isValidMove(board,size,y_1+1,x_1+1)){
                            arr[0]=y_1+1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}//////xong cho con bot đoạn chéo chính rồi
            }
            for(int u=0;u<=goal-2;u++){///// CHÉO PHỤ TIẾN
                if(i+u>=0 && i+u<size && j-u>=0 && j-u<size && board[i+u][j-u]==botSymbol){
                    count_cheo_phu++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j-u;
                    if(count_cheo_phu==goal-2&&(isEmptyHead(board,size,y_2+1,x_2-1,botSymbol)||isEmptyHead(board,size,y_1-1,x_1+1,botSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2-1,botSymbol)&&isEmptyHead(board,size,y_1-1,x_1+1,botSymbol)&&isValidMove(board,size,y_2+1,x_2-1) && isValidMove(board,size,y_1-1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2-1,botSymbol)&&isValidMove(board,size,y_2+1,x_2-1)){
                            arr[0]=y_2+1;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1+1,botSymbol)&&isValidMove(board,size,y_1-1,x_1+1)){
                            arr[0]=y_1-1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j-u+1;
                    break;}
            }
            for(int u=1;u<goal-2;u++){///// CHÉO PHỤ LUI
                if(i-u>=0 && i-u<size && j+u>=0 && j+u<size && board[i-u][j+u]==botSymbol){
                    count_cheo_phu++;
                    y_2=i-u;x_2=j+u;
                    if(count_cheo_phu==goal-2&&(isEmptyHead(board,size,y_2-1,x_2+1,botSymbol)||isEmptyHead(board,size,y_1+1,x_1-1,botSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2+1,botSymbol)&&isEmptyHead(board,size,y_1+1,x_1-1,botSymbol)&&isValidMove(board,size,y_2-1,x_2+1) && isValidMove(board,size,y_1+1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2+1,botSymbol)&&isValidMove(board,size,y_2-1,x_2+1)){
                            arr[0]=y_2-1;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);;
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1-1,botSymbol)&&isValidMove(board,size,y_1+1,x_1-1)){
                            arr[0]=y_1+1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);;
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-2;u++){//// NGANG TIẾN
                if(i>=0 && i<size && j+u>=0 && j+u<size && board[i][j+u]==botSymbol){
                    count_ngang++;
                    y_1=i;x_1=j;y_2=i;x_2=j+u;
                    if(count_ngang==goal-2&&(isEmptyHead(board,size,y_2,x_2+1,botSymbol)||isEmptyHead(board,size,y_1,x_1-1,botSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2+1,botSymbol)&&isEmptyHead(board,size,y_1,x_1-1,botSymbol)&&isValidMove(board,size,y_2,x_2+1) && isValidMove(board,size,y_1,x_1-1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2+1,botSymbol)&&isValidMove(board,size,y_2,x_2+1)){
                            arr[0]=y_2;arr[1]=x_2+1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1-1,botSymbol)&&isValidMove(board,size,y_1,x_1-1)){
                            arr[0]=y_1;arr[1]=x_1-1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i;x_1=j+u-1;
                    break;}
            }
            for(int u=1;u<goal-2;u++){/// NGANG LUI
                if(i>=0 && i<size && j-u>=0 && j-u<size && board[i][j-u]==botSymbol){
                    count_ngang++;
                    y_2=i;x_2=j-u;
                    if(count_ngang==goal-2&&(isEmptyHead(board,size,y_2,x_2-1,botSymbol)||isEmptyHead(board,size,y_1,x_1+1,botSymbol))){
                        if(isEmptyHead(board,size,y_2,x_2-1,botSymbol)&&isEmptyHead(board,size,y_1,x_1+1,botSymbol)&&isValidMove(board,size,y_2,x_2-1) && isValidMove(board,size,y_1,x_1+1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2,x_2-1,botSymbol)&&isValidMove(board,size,y_2,x_2-1)){
                            arr[0]=y_2;arr[1]=x_2-1;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1,x_1+1,botSymbol)&&isValidMove(board,size,y_1,x_1+1)){
                            arr[0]=y_1;arr[1]=x_1+1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
            for(int u=0;u<=goal-2;u++){//// DỌC TIẾN
                if(i+u>=0 && i+u<size && j>=0 && j<size && board[i+u][j]==botSymbol){
                    count_doc++;
                    y_1=i;x_1=j;y_2=i+u;x_2=j;
                    if(count_doc==goal-2&&(isEmptyHead(board,size,y_2+1,x_2,botSymbol)||isEmptyHead(board,size,y_1-1,x_1,botSymbol))){
                        if(isEmptyHead(board,size,y_2+1,x_2,botSymbol)&&isEmptyHead(board,size,y_1-1,x_1,botSymbol)&&isValidMove(board,size,y_2+1,x_2) && isValidMove(board,size,y_1-1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2+1,x_2,botSymbol)&&isValidMove(board,size,y_2+1,x_2)){
                            arr[0]=y_2+1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1-1,x_1,botSymbol)&&isValidMove(board,size,y_1-1,x_1)){
                            arr[0]=y_1-1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{
                    y_1=i+u-1;x_1=j;
                    break;
                }
            }
            for(int u=1 ;u<=goal-2;u++){//DỌC LUI
                if(i-u>=0 && i-u<size && j>=0 && j<size && board[i-u][j]==botSymbol){
                    count_doc++;
                    y_2=i-u;x_2=j;
                    if(count_doc==goal-2&&(isEmptyHead(board,size,y_2-1,x_2,botSymbol)||isEmptyHead(board,size,y_1+1,x_1,botSymbol))){
                        if(isEmptyHead(board,size,y_2-1,x_2,botSymbol)&&isEmptyHead(board,size,y_1+1,x_1,botSymbol)&&isValidMove(board,size,y_2-1,x_2) && isValidMove(board,size,y_1+1,x_1) ){
                            int random=std::uniform_int_distribution <int> (0,1) (generator);
                            if(random==0){arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);}
                            else{arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);}
                        }
                        else if(isEmptyHead(board,size,y_2-1,x_2,botSymbol)&&isValidMove(board,size,y_2-1,x_2)){
                            arr[0]=y_2-1;arr[1]=x_2;return std::make_pair(arr[0],arr[1]);
                        }
                        else if(isEmptyHead(board,size,y_1+1,x_1,botSymbol)&&isValidMove(board,size,y_1+1,x_1)){
                            arr[0]=y_1+1;arr[1]=x_1;return std::make_pair(arr[0],arr[1]);
                        }
                    }
                }
                else{break;}
            }
        }
    }
    while(true){
        arr[0]=std::uniform_int_distribution <int> (0,size-1) (generator);
        arr[1]=std::uniform_int_distribution <int> (0,size-1) (generator);
        if(isValidMove(board,size,arr[0],arr[1])){break;}
    }
    return std::make_pair(arr[0],arr[1]);
}
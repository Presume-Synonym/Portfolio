#include <iostream>
#include <chrono>
#include <cstring>
using namespace std;

const int debug = 0; // 1: debug mode, 0: normal mode
int init_count = 0; // count of init() function

const int EASY = 0; // open all neighbor if neighbor is 0 and open neighbor if neighbor is not 0
const int NORMAL = 1; // open all neighbor if neighbor is 0 (normal minesweeper)
const int HARD = 2; // open only selected
const int mode = NORMAL; // 0: easy, 1: normal, 2: hard

#ifdef _WIN32
    const char *sys_call = "cls";
#elif __linux__
    const char *sys_call = "clear";
#elif __APPLE__
    const char *sys_call = "clear";
#else
    const char *sys_call = "clear";
#endif

#define BEGINNER 0
#define INTERMEDIATE 1
#define EXPERT 2
#define EXPERT_PLUS 3
#define EXTREME 4
#define level INTERMEDIATE

#if level == BEGINNER
    const int ROW_VALUE = 9;
    const int COL_VALUE = 9;
    const int mine = 10;
#elif level == INTERMEDIATE
    const int ROW_VALUE = 16;
    const int COL_VALUE = 16;
    const int mine = 40;
#elif level == EXPERT
    const int ROW_VALUE = 16;
    const int COL_VALUE = 30;
    const int mine = 150;
#elif level == EXPERT_PLUS
    const int ROW_VALUE = 25;
    const int COL_VALUE = 60;
    const int mine = 750;
#elif level == EXTREME
    const int ROW_VALUE = 50;
    const int COL_VALUE = 100;
    const int mine = 3000;
#endif

const int HAS_MINE = 0;
const int IS_OPENED = 1;
const int NEIGHBOR_MINES = 2;
int board[ROW_VALUE][COL_VALUE][3] = {}; // [][][0]: has_mine, [][][1]: is_opened, [][][2]: number_of_neighbor_mines
chrono::high_resolution_clock::time_point start_time, end_time;

/*
* render:
* ボードを描画する関数
* ansがtrueの場合、答えを表示する(初期値はfalse)
*/
void render(bool ans = false);
/*
* init:
* ボードを初期化する関数
* rowとcolで指定されたマス以外のマスに地雷を配置する
*/
void init(int, int);
/*
* init_eval:
* init関数で呼び出される関数
* 周囲のマスの地雷の数を返す
*/
int init_eval(int, int);
/*
* eval:
* ボードの状態を評価する関数
* 開いていないマスの数と地雷の数が一致した場合、trueを返す
*/
int eval();
/*
* eval_neighbor:
* rowとcolで指定されたマスの周囲-1~1マスのうち、boardの範囲外でない範囲を返す関数
* init_eval()とrecursive_open()のforループ用
*/
void eval_neighbor(int, int, int*, int*, int*, int*);
/*
* recursive_open:
* マスを開く関数
* マスを開いた際に、周囲のマスが0の場合、再帰的に開く
*/
void recursive_open(int, int, int, int);
/*
* is_digit:
* 文字列が数字かどうかを判定する関数
*/
bool is_digit(string);

int main()
{
    if (mine >= ROW_VALUE * COL_VALUE) {
        cerr << "err: too many mines" << endl;
        return 1;
    } else if (mine < 1) {
        cerr << "err: too few mines" << endl;
        return 2;
    } else if (mode < EASY || mode > HARD) {
        cerr << "err: mode is invalid" << endl;
        return 3;
    } else if (level < BEGINNER || level > EXTREME) {
        cerr << "err: level is invalid" << endl;
        return 4;
    } else if (ROW_VALUE < 1 || COL_VALUE < 1) {
        cerr << "err: row or col value is invalid" << endl;
        return 5;
    }

    srand((unsigned)time(NULL));
    render();
    int banned = 0, flag = 0, attempt = 0;

    while(!banned) {
        flag = 0;
        int x, y;

        while (true) {
            if (!flag)
                cout << "enter you want to open(row, col): ";
            else
                cout << "enter you want to stand flag(row, col): ";
            string input1, input2;
            cin >> input1;
            if ((input1 == "ans" || input1 == "a") && debug) {
                if (!attempt)
                    cout << "cannot show answer before first attempt" << endl;
                else
                    render(true);
                continue;
            } else if (input1 == "flag" || input1 == "f") {
                if (!attempt)
                    cout << "cannot stand flag before first attempt" << endl;
                else
                    flag = flag ? 0 : 1;
                continue;
            } else {
                cin >> input2;
                if (!is_digit(input1) || !is_digit(input2)) {
                    cout << "invalid input" << endl;
                    continue;
                }
            }
            x = stoi(input1) - 1;
            y = stoi(input2) - 1;
            if (x < 0 || x > ROW_VALUE-1 || y < 0 || y > COL_VALUE-1)
                cout << "invalid number" << endl;
            else if (board[x][y][IS_OPENED] == 1)
                cout << "that is opened" << endl;
            else if (board[x][y][IS_OPENED] == -1 && flag == 0)
                cout << "that is marked" << endl;
            else
                break;
        }

        if (!attempt)
            init(x, y);
        end_time = chrono::high_resolution_clock::now();

        if (flag) {
            board[x][y][IS_OPENED] = board[x][y][IS_OPENED] == -1 ? 0 : -1; // toggle flag
        } else if (board[x][y][HAS_MINE]) {
            banned = 1;
            attempt++;
            break;
        } else {
            if (mode != HARD) {
                recursive_open(x, y, board[x][y][NEIGHBOR_MINES], board[x][y][NEIGHBOR_MINES]);
            } else if (mode == HARD) {
                board[x][y][IS_OPENED] = 1;
            }
            attempt++;
        }

        render();
        if (eval())
            break;

        cout << "time elapsed: " << (double)chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000 << "s" << endl;
    }

    end_time = chrono::high_resolution_clock::now();

    if (banned)
        cout << "game over!!" << endl;
    else
        cout << "you win!!" << endl;

    cout << "attempts: " << attempt << endl << "time elapsed: " << (double)chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() / 1000 << "s" << endl;
    render(true);
    return 0;
}

void render(bool ans) {
    if (ans) {
        for (int i = -1; i < ROW_VALUE; i++) {
            for (int j = -1; j < COL_VALUE; j++) {
                if (i == -1 && j == -1) {
                    printf("    ");
                    continue;
                } else if (i == -1) {
                    printf(" %03d ", j+1);
                    continue;
                } else if (j == -1) {
                    printf("%3d ", i+1);
                    continue;
                } else if (board[i][j][HAS_MINE])
                    printf("[ 💣]");
                else
                    printf("[ %d ]", board[i][j][NEIGHBOR_MINES]);
            }
            printf("\n");
        }
    } else {
        system(sys_call);

        for (int i = -1; i < ROW_VALUE; i++) {
            for (int j = -1; j < COL_VALUE; j++) {
                if (i == -1 && j == -1)
                    printf("    ");
                else if (i == -1)
                    printf(" %03d ", j+1);
                else if (j == -1)
                    printf("%3d ", i+1);
                else {
                    if (board[i][j][IS_OPENED] == 1)
                        printf("[ %d ]", board[i][j][NEIGHBOR_MINES]);
                    else if (board[i][j][IS_OPENED] == -1)
                        printf("[ 🚩]");
                    else
                        printf("[   ]");
                }
            }
            printf("\n");
        }
        if (debug)
            cout << "init_count : " << init_count << endl;
    }
}

void init(int row, int col) {
    while(true) {
        for (int i = 0; i < mine;) {
            int rnd1 = rand() % ROW_VALUE;
            int rnd2 = rand() % COL_VALUE;
            if (board[rnd1][rnd2][HAS_MINE] != 1 && !(rnd1 == row && rnd2 == col)) {
                board[rnd1][rnd2][HAS_MINE] = 1;
                i++;
            }
        }

        for (int i = 0; i < ROW_VALUE; i++) {
            for (int j = 0; j < COL_VALUE; j++) {
                board[i][j][NEIGHBOR_MINES] = init_eval(i, j);
            }
        }
        if (board[row][col][NEIGHBOR_MINES] == 0)
            break;
        else
            memset(board, 0, sizeof(board));
        init_count++;
    }
    start_time = chrono::high_resolution_clock::now();
}

int init_eval(int row, int col) {
    int count = 0;
    int row_from, col_from, row_to, col_to;
    eval_neighbor(row, col, &row_from, &row_to, &col_from, &col_to);

    for (int i = row_from; i <= row_to; i++) {
        for (int j = col_from; j <= col_to; j++) {
            if (board[row + i][col + j][HAS_MINE])
                count++;
        }
    }
    return count;
}

int eval() {
    int count = 0, flags = 0;
    for (int i = 0; i < ROW_VALUE; i++) {
        for (int j = 0; j < COL_VALUE; j++) {
            if (board[i][j][IS_OPENED] != 1)
                count++;
            if (board[i][j][IS_OPENED] == -1)
                flags++;
        }
    }
    if (count != mine)
        cout << "tips: enter 'flag' or 'f' to stand flag" << endl;
    if (debug)
        cout << "count = " << count << endl;
    cout << "remaining: " << mine - flags << endl;
    if (count < mine) {
        cerr << "err: incontistent of mines" << endl;
        exit(5);
    }
    return count == mine;
}

void eval_neighbor(int row, int col, int *row_from, int *row_to, int *col_from, int *col_to) {
    if (row == 0) {
        *row_from = 0;
        *row_to = 1;
    } else if (row == ROW_VALUE - 1) {
        *row_from = -1;
        *row_to = 0;
    } else {
        *row_from = -1;
        *row_to = 1;
    }

    if (col == 0) {
        *col_from = 0;
        *col_to = 1;
    } else if (col == COL_VALUE - 1) {
        *col_from = -1;
        *col_to = 0;
    } else {
        *col_from = -1;
        *col_to = 1;
    }
}

void recursive_open(int row, int col, int neighbor, int first) {
    int row_from, col_from, row_to, col_to;
    eval_neighbor(row, col, &row_from, &row_to, &col_from, &col_to);
    for (int i = row_from; i <= row_to; i++) {
        for (int j = col_from; j <= col_to; j++) {
            int neighbor_of_neighbor = board[row + i][col + j][NEIGHBOR_MINES];
            int opened = board[row + i][col + j][IS_OPENED];
            int mine = board[row + i][col + j][HAS_MINE];

            if (i == 0 && j == 0) {
                board[row + i][col + j][IS_OPENED] = 1;
            } else if (first != 0 && !mine && opened == 0 && mode == EASY) {
                board[row + i][col + j][IS_OPENED] = 1;
            } else if (first == 0 && !mine && opened == 0) {
                if (neighbor_of_neighbor == 0) {
                    board[row + i][col + j][IS_OPENED] = 1;
                    recursive_open(row + i, col + j, neighbor_of_neighbor, first);
                } else {
                    board[row + i][col + j][IS_OPENED] = 1;
                }
            }
        }
    }
}

bool is_digit(string str) {
    for (unsigned long i = 0; i < str.length(); i++) {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}
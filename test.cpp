
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "winmm.lib")

using namespace std;

#define H 20
#define W 15

char board[H][W] = {};
int x, y, b;

const int TOC_DO_BAN_DAU = 500;
const int TOC_DO_NHANH_NHAT = 100;
const int BUOC_GIAM_THOI_GIAN = 40;
int tocDoRoi = TOC_DO_BAN_DAU;
int tongSoDongDaXoa = 0;


void setColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

int getCellColor(char cell) {
    switch (cell) {
    case 'I': return 11;
    case 'O': return 14;
    case 'T': return 13;
    case 'S': return 10;
    case 'Z': return 12;
    case 'J': return 9;
    case 'L': return 6;
    case '#': return 8;
    default:  return 7;
    }
}

void sound_BatDauNhacNen() {
    Beep(196, 90);
    Beep(220, 90);
    Beep(262, 110);
}

void sound_DiChuyenNgang() {
    PlaySoundW((LPCWSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_ASYNC | SND_NODEFAULT);
}

void sound_XoayKhoi() {
    cout << "\a";
}

void sound_XoaKhoi() {
    Beep(262, 110);
    Beep(330, 110);
    Beep(392, 110);
    Beep(523, 280);
}

void sound_KetThuc() {
    Beep(392, 120);
    Beep(293, 180);
    Beep(193, 240);
}

char blocks[][4][4] = {
    {{' ',' ',' ',' '},
     {'I','I','I','I'},
     {' ',' ',' ',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {' ','O','O',' '},
     {' ','O','O',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {' ','T',' ',' '},
     {'T','T','T',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {' ','S','S',' '},
     {'S','S',' ',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {'Z','Z',' ',' '},
     {' ','Z','Z',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {'J',' ',' ',' '},
     {'J','J','J',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},
     {' ',' ','L',' '},
     {'L','L','L',' '},
     {' ',' ',' ',' '}}
};

char activeBlock[4][4];

void loadActiveBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            activeBlock[i][j] = blocks[b][i][j];
}

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (activeBlock[i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W - 1 || yt >= H - 1) return false;
                if (board[yt][xt] != ' ') return false;
            }
    return true;
}

void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (activeBlock[i][j] != ' ')
                board[y + i][x + j] = activeBlock[i][j];
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (activeBlock[i][j] != ' ')
                board[y + i][x + j] = ' ';
}

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (i == 0 || i == H - 1 || j == 0 || j == W - 1) board[i][j] = '#';
            else board[i][j] = ' ';
}

bool rotateBlock(char piece[4][4], const char brd[H][W], int& pieceX, int pieceY) {
    char rotated[4][4];
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            rotated[col][3 - row] = piece[row][col];

    const int kicks[] = {0, -1, 1, -2, 2};
    for (int kick : kicks) {
        int newX = pieceX + kick;
        bool valid = true;

        for (int row = 0; row < 4 && valid; row++) {
            for (int col = 0; col < 4; col++) {
                if (rotated[row][col] == ' ') continue;
                int bx = newX + col;
                int by = pieceY + row;
                if (bx < 1 || bx >= W - 1 || by < 1 || by >= H - 1 || brd[by][bx] != ' ') {
                    valid = false;
                    break;
                }
            }
        }

        if (valid) {
            pieceX = newX;
            for (int row = 0; row < 4; row++)
                for (int col = 0; col < 4; col++)
                    piece[row][col] = rotated[row][col];
            return true;
        }
    }
    return false;
}

bool laHangDay(int hang) {
    for (int cot = 1; cot < W - 1; cot++)
        if (board[hang][cot] == ' ') return false;
    return true;
}

int removeLine() {
    int soDongDaXoa = 0;
    int hangGhi = H - 2;

    for (int hangDoc = H - 2; hangDoc >= 1; hangDoc--) {
        if (laHangDay(hangDoc)) {
            soDongDaXoa++;
        } else {
            if (hangGhi != hangDoc)
                for (int cot = 1; cot < W - 1; cot++)
                    board[hangGhi][cot] = board[hangDoc][cot];
            hangGhi--;
        }
    }

    for (int hang = hangGhi; hang >= 1; hang--)
        for (int cot = 1; cot < W - 1; cot++)
            board[hang][cot] = ' ';

    return soDongDaXoa;
}

void CapNhatTocDo(int SoDongXoa) {
    if (SoDongXoa <= 0) return;
    tongSoDongDaXoa += SoDongXoa;
    tocDoRoi -= SoDongXoa * BUOC_GIAM_THOI_GIAN;
    if (tocDoRoi < TOC_DO_NHANH_NHAT) tocDoRoi = TOC_DO_NHANH_NHAT;
}

void draw() {
    COORD cursorPos = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (i == 0 && j == 0)              { setColor(getCellColor('#')); cout << " ╔"; }
            else if (i == 0 && j == W - 1)     { setColor(getCellColor('#')); cout << "╗ "; }
            else if (i == H - 1 && j == 0)     { setColor(getCellColor('#')); cout << " ╚"; }
            else if (i == H - 1 && j == W - 1) { setColor(getCellColor('#')); cout << "╝ "; }
            else if (i == 0 || i == H - 1)     { setColor(getCellColor('#')); cout << "══"; }
            else if (j == 0)                   { setColor(getCellColor('#')); cout << " ║"; }
            else if (j == W - 1)               { setColor(getCellColor('#')); cout << "║ "; }
            else if (board[i][j] == ' ') cout << "  ";
            else { setColor(getCellColor(board[i][j])); cout << "██"; }
        }
        cout << '\n';
    }

    setColor(7);
    cout << " So dong da xoa: " << tongSoDongDaXoa
         << " | Toc do (ms): " << tocDoRoi << "     \n";
}

int main() {
    SetConsoleOutputCP(65001);
    system("cls");
    srand(static_cast<unsigned int>(time(0)));

    x = 5; y = 0; b = rand() % 7;
    initBoard();
    loadActiveBlock();
    sound_BatDauNhacNen();

    while (1) {
        boardDelBlock();

        if (_kbhit()) {
            int c = _getch();

            if (c == 224 || c == 0) {
                c = _getch();
                if (c == 75 && canMove(-1, 0)) { x--; sound_DiChuyenNgang(); }
                if (c == 77 && canMove(1, 0))  { x++; sound_DiChuyenNgang(); }
                if (c == 80 && canMove(0, 1))  y++;
                if (c == 72 && rotateBlock(activeBlock, board, x, y)) sound_XoayKhoi();
            }
            else {
                if ((c == 'a' || c == 'A') && canMove(-1, 0)) { x--; sound_DiChuyenNgang(); }
                if ((c == 'd' || c == 'D') && canMove(1, 0))  { x++; sound_DiChuyenNgang(); }
                if ((c == 'x' || c == 'X') && canMove(0, 1))  y++;
                if ((c == 'w' || c == 'W') && rotateBlock(activeBlock, board, x, y)) sound_XoayKhoi();
                if (c == 'q' || c == 'Q') break;
            }
        }

        if (canMove(0, 1)) {
            y++;
        }
        else {
            block2Board();

            int SoDongXoa = removeLine();
            if (SoDongXoa > 0) {
                CapNhatTocDo(SoDongXoa);
                sound_XoaKhoi();
            }

            bool isOver = false;
            for (int j = 1; j < W - 1; j++)
                if (board[2][j] != ' ') { isOver = true; break; }

            if (isOver) {
                draw();
                cout << "\n   GAME OVER! Tro choi ket thuc." << endl;
                sound_KetThuc();
                break;
            }

            x = 5; y = 0; b = rand() % 7;
            loadActiveBlock();
        }

        block2Board();
        draw();
        Sleep(tocDoRoi);
    }
    return 0;
}

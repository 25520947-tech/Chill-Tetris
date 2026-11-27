#include <iostream>
#include <conio.h>
#include <windows.h> 
#include <cstdlib>
#include <ctime>
#include "removeLine.h"
#include "sound.h"
#include "color.h"

using namespace std;

#define H 20
#define W 15

#include "rotation.h"

char board[H][W] = {};

int x, y, b;

// Phần 5 khai báo hằng số và biến quản lí tốc độ rơi
const int TOC_DO_BAN_DAU = 500;
const int TOC_DO_NHANH_NHAT = 100;
const int BUOC_GIAM_THOI_GIAN = 40;
int tocDoRoi = TOC_DO_BAN_DAU;
int tongSoDongDaXoa = 0;

char blocks[][4][4] = {
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
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

                if (xt < 1 || xt >= W - 1 || yt >= H - 1)
                    return false;

                if (board[yt][xt] != ' ')
                    return false;
            }

    return true;
}

void softDrop() {
    // Xuống nhanh một hàng
    if (canMove(0, 1))
        y++;
}

void hardDrop() {
    // Space đưa khối xuống vị trí thấp nhất có thể
    while (canMove(0, 1))
        y++;
}

void handleDropInput(int key) {
    // Phím mũi tên trong _getch() gồm mã mở rộng rồi đến mã phím
    if (key == 0 || key == 0xE0) {
        int arrowKey = _getch();

        // 80 là mã mũi tên xuống
        if (arrowKey == 80)
            softDrop();

        return;
    }

    if (key == 's' || key == 'S')
        softDrop();
    else if (key == ' ')
        hardDrop();
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

// Phần 5: Hàm tăng tốc độ rơi
void CapNhatTocDo(int SoDongXoa) {
    if (SoDongXoa <= 0) return;

    tongSoDongDaXoa += SoDongXoa;
    tocDoRoi -= SoDongXoa * BUOC_GIAM_THOI_GIAN;

    if (tocDoRoi < TOC_DO_NHANH_NHAT) {
        tocDoRoi = TOC_DO_NHANH_NHAT;
    }
}

// Phan 2: chinh sua lai khung vien va khoi sao cho vuong vuc
void draw() {
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos = { 0, 0 };
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            // Ve 4 goc khung vien
            if (i == 0 && j == 0)
            {
                setColor(getCellColor('#'));
                cout << " ╔";
            }
            else if (i == 0 && j == W - 1)
            {
                setColor(getCellColor('#'));
                cout << "╗ ";
            }
            else if (i == H - 1 && j == 0)
            {
                setColor(getCellColor('#'));
                cout << " ╚";
            }
            else if (i == H - 1 && j == W - 1)
            {
                setColor(getCellColor('#'));
                cout << "╝ ";
            }
            // Ve canh tren va canh duoi
            else if (i == 0 || i == H - 1)
            {
                setColor(getCellColor('#'));
                cout << "══";
            }
            // Ve canh trai va canh phai (ap sat vao long bang)
            else if (j == 0)
            {
                setColor(getCellColor('#'));
                cout << " ║";
            }
            else if (j == W - 1)
            {
                setColor(getCellColor('#'));
                cout << "║ ";
            }
            // Ve long bang: khoang trong hoac block vuong
            else if (board[i][j] == ' ') cout << "  ";
            else
            {
                setColor(getCellColor(board[i][j]));
                cout << "██";
            }
        }
        cout << '\n';
    }

    setColor(7); // Trả về màu trắng mặc đinh của console
    // Phần 5:Hiển thị thông số để quan sát tốc độ rơi
    cout << " So dong da xoa: " << tongSoDongDaXoa
        << " | Toc do (ms): " << tocDoRoi << '\n';
}

int main() {
    SetConsoleOutputCP(65001); // Bat UTF-8 de hien thi ky tu khung va khoi dac
    srand(static_cast<unsigned int>(time(0)));
    x = 5; y = 0; b = rand() % 7;
    initBoard();
    loadActiveBlock();
    initSound(); // Khoi tao he thong am thanh
    sound_BatDauNhacNen(); // Bat dau phat nhac nen

    while (1) {
        boardDelBlock();

        if (_kbhit()) {
            int key = _getch();

            // Xu ly phim S, mui ten xuong va Space
            handleDropInput(key);

            if (key != 0 && key != 0xE0) {
                char c = static_cast<char>(key);

                if (c == 'a' && canMove(-1, 0)) {
                    x--;
                    sound_DiChuyenNgang();
                }

                if (c == 'd' && canMove(1, 0)) {
                    x++;
                    sound_DiChuyenNgang();
                }

                if (c == 'x' && canMove(0, 1))
                    y++;

                if (c == 'w' || c == 'W') {
                    rotateBlock(activeBlock, board, x, y);
                    sound_XoayKhoi();
                }

                if (c == 'q')
                    break;
            }
        }

        if (canMove(0, 1)) {
            y++;
        }
        else {
            block2Board();

            // Phần 5: Gọi removeLine() và cập nhập tốc độ rơi
            int SoDongXoa = removeLine();
            CapNhatTocDo(SoDongXoa);

            if (SoDongXoa > 0) {
                sound_XoaKhoi();// Phat am thanh khi co hang duoc xoa
            }

            bool isOver = false;
            for (int j = 1; j < W - 1; j++) {
                if (board[2][j] != ' ') {
                    isOver = true;
                    break;
                }
            }
            if (isOver) {
                draw();
                cout << "\n";
                cout << "           GAME OVER!" << endl;
                cout << "       Hay choi lai nhe!!" << endl;

                sound_KetThuc();
                Sleep(1500);
                break;
            }

            x = 5; y = 0; b = rand() % 7;
            loadActiveBlock();
        }
        block2Board();
        draw();

        // Phần 5: dùng Sleep(tocDoRoi) thay cho _sleep(500)
        Sleep(tocDoRoi);
    }
    closeSound();
    return 0;
}
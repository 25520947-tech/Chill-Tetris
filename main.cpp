#include <iostream>
#include <conio.h>
#include <windows.h> 
#include <cstdlib>   

using namespace std;

#define H 20
#define W 15
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

bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ') {
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
            if (blocks[b][i][j] != ' ')
                board[y + i][x + j] = blocks[b][i][j];
}

void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blocks[b][i][j] != ' ')
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
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            // Ve 4 goc khung vien
            if (i == 0 && j == 0) cout << " ╔";
            else if (i == 0 && j == W - 1) cout << "╗ ";
            else if (i == H - 1 && j == 0) cout << " ╚";
            else if (i == H - 1 && j == W - 1) cout << "╝ ";
            // Ve canh tren va canh duoi
            else if (i == 0 || i == H - 1) cout << "══";
            // Ve canh trai va canh phai (ap sat vao long bang)
            else if (j == 0) cout << " ║";
            else if (j == W - 1) cout << "║ ";
            // Ve long bang: khoang trong hoac block vuong
            else if (board[i][j] == ' ') cout << "  ";
            else cout << "██";
        }
        cout << '\n';
    }
    // Phần 5:Hiển thị thông số để quan sát tốc độ rơi
    cout << " So dong da xoa: " << tongSoDongDaXoa
        << " | Toc do (ms): " << tocDoRoi << '\n';
}

int main() {
    SetConsoleOutputCP(65001); // Bat UTF-8 de hien thi ky tu khung va khoi dac
    srand(time(0));
    x = 5; y = 0; b = rand() % 7;
    initBoard();

    while (1) {
        boardDelBlock();
        if (kbhit()) {
            char c = getch();
            if (c == 'a' && canMove(-1, 0)) x--;
            if (c == 'd' && canMove(1, 0)) x++;
            if (c == 'x' && canMove(0, 1)) y++;
            if (c == 'q') break;
        }
        if (canMove(0, 1)) {
            y++;
        }
        else {
            block2Board();

            // Phần 5: Gọi removeLine() và cập nhập tốc độ rơi
            int SoDongXoa = removeLine();
            CapNhatTocDo(SoDongXoa);

            x = 5; y = 0; b = rand() % 7;
        }
        block2Board();
        draw();

        // Phần 5: dùng sleep(tocdoroi) thay cho sleep(500)
        Sleep(tocDoRoi);
    }
    return 0;
}

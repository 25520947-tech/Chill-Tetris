#include <iostream>
#include <conio.h>

using namespace std;
#define H 20
#define W 15
char board[H][W] = {};

int x, y, b;
char blocks[][4][4] ={
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

bool canMove(int dx, int dy){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;
                if (xt < 1 || xt >= W-1 || yt >= H-1 ) return false;
                if (board[yt][xt] != ' ') return false;
            }
    return true;
}

void block2Board(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = blocks[b][i][j];
}

void boardDelBlock(){
    for (int i = 0; i < 4; i++ )
        for (int j = 0; j < 4; j++ )
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = ' ';
}

void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (i == 0 || i == H - 1 || j == 0 || j == W - 1) board[i][j] = '#';
            else board[i][j] = ' ';
}

//Phần 2: chỉnh sửa lại khung viền và khối sao cho vuông vức
void draw() {
    system("cls");
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            // Vẽ 4 góc khung viền
            if (i == 0 && j == 0) cout << " ╔";
            else if (i == 0 && j == W - 1) cout << "╗ ";
            else if (i == H - 1 && j == 0) cout << " ╚";
            else if (i == H - 1 && j == W - 1) cout << "╝ ";
            // Vẽ cạnh trên và cạnh dưới
            else if (i == 0 || i == H - 1) cout << "══";
            // Vẽ cạnh trái và cạnh phải (áp sát vào lòng bảng)
            else if (j == 0) cout << " ║";
            else if (j == W - 1) cout << "║ ";
            // Vẽ lòng bảng: khoảng trống hoặc block vuông
            else if (board[i][j] == ' ') cout << "  ";
            else cout << "██";
        }
        cout << '\n';
    }
}

//Chỉnh sửa phần main để di chuyển theo cả dấu mũi tên được
int main() {
    SetConsoleOutputCP(65001);
    srand(time(0));
    x = 5; y = 0; b = rand() % 7;
    initBoard();

    while (1) {
        boardDelBlock();
        if (kbhit()) {
            int c = getch();

            // Phím mũi tên: getch() trả về 224 (hoặc 0) rồi mới đến mã hướng
            if (c == 224 || c == 0) {
                c = getch();
                if (c == 75 && canMove(-1, 0)) x--;   // ←
                if (c == 77 && canMove(1, 0))  x++;   // →
                if (c == 80 && canMove(0, 1))  y++;   // ↓
            }
            else {
                // Giữ nguyên các phím cũ
                if (c == 'a' && canMove(-1, 0)) x--;
                if (c == 'd' && canMove(1, 0))  x++;
                if (c == 'x' && canMove(0, 1))  y++;
                if (c == 'q') break;
            }
        }
        if (canMove(0, 1)) y++;
        else {
            block2Board();
            x = 5; y = 0; b = rand() % 7;
        }
        block2Board();
        draw();
        _sleep(500);
    }
    return 0;
}

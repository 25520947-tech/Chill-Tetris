
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>   // PlaySoundW (dùng cho âm thanh hệ thống)
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "winmm.lib")   // MSVC tự liên kết; MinGW dùng cờ -lwinmm

using namespace std;

// ======================= PHẦN 1: HẰNG SỐ & BIẾN TOÀN CỤC ==============
#define H 20            // Chiều cao bảng (gồm cả 2 hàng viền)
#define W 15            // Chiều rộng bảng (gồm cả 2 cột viền)

char board[H][W] = {};  // Bảng chơi: '#' là viền, ' ' là ô trống, chữ cái là khối đã đặt
int x, y, b;            // x, y: vị trí khối đang rơi | b: chỉ số loại khối

// Quản lí tốc độ rơi (Phần 5)
const int TOC_DO_BAN_DAU     = 500;  // ms ban đầu giữa 2 lần rơi
const int TOC_DO_NHANH_NHAT  = 100;  // ms nhỏ nhất (nhanh nhất)
const int BUOC_GIAM_THOI_GIAN = 40;  // mỗi dòng xóa giảm 40ms
int tocDoRoi = TOC_DO_BAN_DAU;
int tongSoDongDaXoa = 0;

// ======================= PHẦN 2: MÀU SẮC (color.cpp) ==================
// Đổi màu chữ in ra console kể từ lúc gọi hàm
void setColor(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Lấy định danh cửa sổ console
    SetConsoleTextAttribute(hConsole, colorCode);      // Áp dụng mã màu mới
}

// Trả về mã màu tương ứng với từng loại khối
int getCellColor(char cell) {
    switch (cell) {
    case 'I': return 11; // Cyan
    case 'O': return 14; // Vàng
    case 'T': return 13; // Magenta
    case 'S': return 10; // Xanh lá
    case 'Z': return 12; // Đỏ
    case 'J': return 9;  // Xanh dương
    case 'L': return 6;  // Vàng đậm
    case '#': return 8;  // Xám đậm cho khung viền
    default:  return 7;  // Trắng mặc định
    }
}

// ======================= PHẦN 3: ÂM THANH (sound.cpp) =================
namespace {
    // Phát một âm thanh hệ thống (bất đồng bộ, không chặn game)
    void phatAmHeThong(LPCWSTR aliasId) {
        PlaySoundW(aliasId, NULL, SND_ALIAS_ID | SND_ASYNC | SND_NODEFAULT);
    }
}

void sound_BatDauNhacNen() {   // Nhạc mở đầu
    Beep(196, 90);
    Beep(220, 90);
    Beep(262, 110);
}

void sound_DiChuyenNgang() {   // Khi di chuyển trái/phải
    phatAmHeThong((LPCWSTR)SND_ALIAS_SYSTEMASTERISK);
}

void sound_XoayKhoi() {        // Khi xoay khối
    cout << "\a";
}

void sound_XoaKhoi() {         // Khi xóa được dòng
    Beep(262, 110);
    Beep(330, 110);
    Beep(392, 110);
    Beep(523, 280);
}

void sound_KetThuc() {         // Khi thua game
    Beep(392, 120);
    Beep(293, 180);
    Beep(193, 240);
}

// ======================= PHẦN 4: CÁC KHỐI GẠCH ========================
// 7 khối chuẩn: I, O, T, S, Z, J, L (mỗi khối là ma trận 4x4)
// Lưu ý: bản gốc bị lặp khối O nhiều lần nên rand()%7 chỉ ra I và O.
//        Ở đây đã sửa thành đúng 7 khối khác nhau.
char blocks[][4][4] = {
    {{' ',' ',' ',' '},     // I
     {'I','I','I','I'},
     {' ',' ',' ',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},     // O
     {' ','O','O',' '},
     {' ','O','O',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},     // T
     {' ','T',' ',' '},
     {'T','T','T',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},     // S
     {' ','S','S',' '},
     {'S','S',' ',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},     // Z
     {'Z','Z',' ',' '},
     {' ','Z','Z',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},     // J
     {'J',' ',' ',' '},
     {'J','J','J',' '},
     {' ',' ',' ',' '}},
    {{' ',' ',' ',' '},     // L
     {' ',' ','L',' '},
     {'L','L','L',' '},
     {' ',' ',' ',' '}}
};

char activeBlock[4][4];    // Khối đang rơi (bản sao, để có thể xoay)

// Nạp khối loại b vào activeBlock
void loadActiveBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            activeBlock[i][j] = blocks[b][i][j];
}

// ======================= PHẦN 5: THAO TÁC TRÊN BẢNG ===================
// Kiểm tra khối có di chuyển được theo (dx, dy) không
bool canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (activeBlock[i][j] != ' ') {
                int xt = x + j + dx;
                int yt = y + i + dy;

                if (xt < 1 || xt >= W - 1 || yt >= H - 1)  // Chạm tường / đáy
                    return false;
                if (board[yt][xt] != ' ')                  // Chạm khối khác
                    return false;
            }
    return true;
}

// Ghi khối đang rơi lên bảng
void block2Board() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (activeBlock[i][j] != ' ')
                board[y + i][x + j] = activeBlock[i][j];
}

// Xóa khối đang rơi khỏi bảng (trước khi di chuyển)
void boardDelBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (activeBlock[i][j] != ' ')
                board[y + i][x + j] = ' ';
}

// Khởi tạo bảng: viền '#', bên trong trống
void initBoard() {
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (i == 0 || i == H - 1 || j == 0 || j == W - 1) board[i][j] = '#';
            else board[i][j] = ' ';
}

// ======================= PHẦN 6: XOAY KHỐI (rotation.cpp) =============
// Xoay khối 90° theo chiều kim đồng hồ, có "wall kick" nếu bị vướng tường
bool rotateBlock(char piece[4][4], const char brd[H][W], int& pieceX, int pieceY) {
    char rotated[4][4];

    // Xoay ma trận 4x4
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            rotated[col][3 - row] = piece[row][col];

    // Thử vị trí hiện tại trước, rồi dịch ngang nếu bị vướng
    const int kicks[] = {0, -1, 1, -2, 2};

    for (int kick : kicks) {
        int newX = pieceX + kick;
        bool valid = true;

        for (int row = 0; row < 4 && valid; row++) {
            for (int col = 0; col < 4; col++) {
                if (rotated[row][col] == ' ') continue;

                int boardX = newX + col;
                int boardY = pieceY + row;

                if (boardX < 1 || boardX >= W - 1 ||
                    boardY < 1 || boardY >= H - 1 ||
                    brd[boardY][boardX] != ' ') {
                    valid = false;
                    break;
                }
            }
        }

        if (valid) {                       // Chấp nhận phép xoay
            pieceX = newX;
            for (int row = 0; row < 4; row++)
                for (int col = 0; col < 4; col++)
                    piece[row][col] = rotated[row][col];
            return true;
        }
    }
    return false;                          // Không xoay được
}

// ======================= PHẦN 7: XÓA DÒNG (XoaDongDaDay.h) ============
// Kiểm tra một hàng trong vùng chơi đã đầy khối chưa
bool laHangDay(int chiSoHang) {
    for (int cot = 1; cot < W - 1; cot++)
        if (board[chiSoHang][cot] == ' ')
            return false;                  // Còn ít nhất 1 ô trống
    return true;
}

// Xóa mọi hàng đầy, dồn các hàng phía trên xuống, trả về số dòng đã xóa
int removeLine() {
    int soDongDaXoa = 0;
    int hangGhi = H - 2;                   // Con trỏ ghi: bắt đầu từ hàng thấp nhất

    // Duyệt ngược từ đáy lên đỉnh
    for (int hangDoc = H - 2; hangDoc >= 1; hangDoc--) {
        if (laHangDay(hangDoc)) {
            soDongDaXoa++;                 // Hàng đầy -> bỏ qua (xóa)
        } else {
            if (hangGhi != hangDoc)        // Chỉ chép khi có hàng bên dưới đã bị xóa
                for (int cot = 1; cot < W - 1; cot++)
                    board[hangGhi][cot] = board[hangDoc][cot];
            hangGhi--;
        }
    }

    // Làm trống các hàng còn thừa ở phía trên cùng
    for (int hangTrong = hangGhi; hangTrong >= 1; hangTrong--)
        for (int cot = 1; cot < W - 1; cot++)
            board[hangTrong][cot] = ' ';

    return soDongDaXoa;
}

// ======================= PHẦN 8: TĂNG TỐC ĐỘ RƠI ======================
void CapNhatTocDo(int SoDongXoa) {
    if (SoDongXoa <= 0) return;

    tongSoDongDaXoa += SoDongXoa;
    tocDoRoi -= SoDongXoa * BUOC_GIAM_THOI_GIAN;

    if (tocDoRoi < TOC_DO_NHANH_NHAT)
        tocDoRoi = TOC_DO_NHANH_NHAT;
}

// ======================= PHẦN 9: VẼ MÀN HÌNH ==========================
void draw() {
    // Đưa con trỏ về góc trên-trái thay cho system("cls") để đỡ nhấp nháy
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorPos = {0, 0};
    SetConsoleCursorPosition(hConsole, cursorPos);

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            // 4 góc khung viền
            if (i == 0 && j == 0)               { setColor(getCellColor('#')); cout << " ╔"; }
            else if (i == 0 && j == W - 1)      { setColor(getCellColor('#')); cout << "╗ "; }
            else if (i == H - 1 && j == 0)      { setColor(getCellColor('#')); cout << " ╚"; }
            else if (i == H - 1 && j == W - 1)  { setColor(getCellColor('#')); cout << "╝ "; }
            // Cạnh trên và cạnh dưới
            else if (i == 0 || i == H - 1)      { setColor(getCellColor('#')); cout << "══"; }
            // Cạnh trái và cạnh phải
            else if (j == 0)                    { setColor(getCellColor('#')); cout << " ║"; }
            else if (j == W - 1)                { setColor(getCellColor('#')); cout << "║ "; }
            // Lòng bảng: ô trống hoặc khối vuông màu
            else if (board[i][j] == ' ') cout << "  ";
            else {
                setColor(getCellColor(board[i][j]));
                cout << "██";
            }
        }
        cout << '\n';
    }

    setColor(7);   // Trả về màu trắng mặc định
    // Hiển thị thông số để quan sát tốc độ rơi (thêm khoảng trắng để xóa chữ cũ)
    cout << " So dong da xoa: " << tongSoDongDaXoa
         << " | Toc do (ms): " << tocDoRoi << "     \n";
}

// ======================= PHẦN 10: HÀM MAIN ============================
int main() {
    SetConsoleOutputCP(65001);                       // Bật UTF-8 để hiển thị ký tự khung/khối
    system("cls");                                   // Xóa màn hình một lần lúc bắt đầu
    srand(static_cast<unsigned int>(time(0)));       // Khởi tạo số ngẫu nhiên

    x = 5; y = 0; b = rand() % 7;                    // Khối đầu tiên
    initBoard();
    loadActiveBlock();
    sound_BatDauNhacNen();

    while (1) {
        boardDelBlock();                             // Gỡ khối khỏi bảng trước khi xử lí

        // ---- Xử lí phím bấm ----
        if (_kbhit()) {
            int c = _getch();

            // Phím mũi tên: _getch() trả về 224 (hoặc 0) rồi mới đến mã hướng
            if (c == 224 || c == 0) {
                c = _getch();
                if (c == 75 && canMove(-1, 0)) { x--; sound_DiChuyenNgang(); }   // ←
                if (c == 77 && canMove(1, 0))  { x++; sound_DiChuyenNgang(); }   // →
                if (c == 80 && canMove(0, 1))  y++;                              // ↓
                if (c == 72 && rotateBlock(activeBlock, board, x, y))            // ↑ xoay
                    sound_XoayKhoi();
            }
            else {
                // Phím chữ (giữ nguyên các phím cũ)
                if ((c == 'a' || c == 'A') && canMove(-1, 0)) { x--; sound_DiChuyenNgang(); }
                if ((c == 'd' || c == 'D') && canMove(1, 0))  { x++; sound_DiChuyenNgang(); }
                if ((c == 'x' || c == 'X') && canMove(0, 1))  y++;
                if (c == 'w' || c == 'W') {
                    if (rotateBlock(activeBlock, board, x, y)) sound_XoayKhoi();
                }
                if (c == 'q' || c == 'Q') break;
            }
        }

        // ---- Khối tự rơi xuống ----
        if (canMove(0, 1)) {
            y++;
        }
        else {
            // Khối chạm đáy/khối khác: cố định lên bảng
            block2Board();

            // Xóa dòng đầy và cập nhật tốc độ
            int SoDongXoa = removeLine();
            if (SoDongXoa > 0) {
                CapNhatTocDo(SoDongXoa);
                sound_XoaKhoi();
            }

            // Kiểm tra thua: khối chất đến hàng 2
            bool isOver = false;
            for (int j = 1; j < W - 1; j++) {
                if (board[2][j] != ' ') { isOver = true; break; }
            }
            if (isOver) {
                draw();
                cout << "\n   GAME OVER! Tro choi ket thuc." << endl;
                sound_KetThuc();
                break;
            }

            // Sinh khối mới
            x = 5; y = 0; b = rand() % 7;
            loadActiveBlock();
        }

        block2Board();                               // Ghi khối lên bảng
        draw();                                      // Vẽ lại màn hình
        Sleep(tocDoRoi);                             // Tốc độ rơi (thay cho _sleep(500))
    }
    return 0;
}

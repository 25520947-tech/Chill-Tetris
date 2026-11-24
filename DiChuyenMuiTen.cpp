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

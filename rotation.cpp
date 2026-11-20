#include "rotation.h"

bool rotateBlock(
    char piece[4][4],
    const char board[H][W],
    int& pieceX,
    int pieceY
) {
    char rotated[4][4];

    // Xoay ma trận 4x4 90 độ theo chiều kim đồng hồ
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            rotated[col][3 - row] = piece[row][col];
        }
    }

    // Thử vị trí hiện tại trước, rồi dịch ngang nếu bị vướng tường
    const int kicks[] = {0, -1, 1, -2, 2};

    for (int kick : kicks) {
        int newX = pieceX + kick;
        bool valid = true;

        for (int row = 0; row < 4 && valid; row++) {
            for (int col = 0; col < 4; col++) {
                if (rotated[row][col] == ' ')
                    continue;

                int boardX = newX + col;
                int boardY = pieceY + row;

                if (boardX < 1 || boardX >= W - 1 ||
                    boardY < 1 || boardY >= H - 1 ||
                    board[boardY][boardX] != ' ') {
                    valid = false;
                    break;
                }
            }
        }

        if (valid) {
            pieceX = newX;

            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    piece[row][col] = rotated[row][col];
                }
            }

            return true;
        }
    }

    return false;
}
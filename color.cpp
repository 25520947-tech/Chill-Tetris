#include "color.h"
#include <windows.h>

void setColor(int colorCode)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Lấy định danh của cửa sổ dòng lệnh đang chạy game để os biết cần đổi màu cho cửa sổ nào
    SetConsoleTextAttribute(hConsole, colorCode);      // Áp dụng mã màu mới cho ký tự được in ra kể từ lúc gọi hàm này
}

int getCellColor(char cell)
{
    switch (cell)
    {
    case 'I':
        return 11; // Cyan
    case 'O':
        return 14; // Yellow
    case 'T':
        return 13; // Magenta
    case 'S':
        return 10; // Green
    case 'Z':
        return 12; // Red
    case 'J':
        return 9; // Blue
    case 'L':
        return 6; // Gold
    case '#':
        return 8; // Xám đậm cho khung viền
    default:
        return 7; // Trắng mặc định của Console
    }
}

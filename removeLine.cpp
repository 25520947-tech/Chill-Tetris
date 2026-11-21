#include <cstdlib>
#include <windows.h>
#include "removeLine.h"

int removeLine()
{
    int i, j;
    int cleared = 0;
    for(i = H - 2; i > 0; --i)
    {
        for(j = 1; j < W - 1; ++j)
            if (board[i][j] == ' ')
                break;

        if(j == W - 1)
        {
            for(int ii = i; ii > 1; --ii)
                for (int jj = 1; jj < W - 1; ++jj)
                    board[ii][jj] = board[ii - 1][jj];

            for(int jj = 1; jj < W - 1; ++jj)
                board[1][jj] = ' ';

            ++i;
            draw();
            Sleep(200);
        }
    }
    return cleared;
}
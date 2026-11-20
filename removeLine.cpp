#include <cstdlib>
#include "removeLine.h"

void removeLine()
{
    int i, j;
    for(i = H-2; i > 0; --i)
    {
        for(j = 0; j < W; ++j)
            if (board[i][j] == ' ')
                break;
        if(j == W)
        {
            for(int ii = i; ii > 0; --ii)
                for (int jj = 0; jj < W; jj++)
                    board[ii][jj] = board[ii-1][jj];
            ++i;
            draw();
            _sleep(200);
        }
    }
}
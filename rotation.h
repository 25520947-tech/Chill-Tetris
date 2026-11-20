#ifndef ROTATION_H
#define ROTATION_H

#ifndef H
#define H 20
#endif

#ifndef W
#define W 15
#endif

bool rotateBlock(
    char piece[4][4],
    const char board[H][W],
    int& pieceX,
    int pieceY
);

#endif

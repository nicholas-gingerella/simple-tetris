#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>

std::wstring tetromino[7];
int nFieldWidth = 14;
int nFieldHeight = 14;
unsigned char *pField = nullptr;

// func: rotate
// px: x-coordinate
// px: y-coordinate
// r:  rotation value
// return: The "rotated" integer index
//
// rotates a given tetromino index based on 
// "coordinates" of piece in its array representation
// rotation calculation is described in video
// https://www.youtube.com/watch?v=8OK8_tHeCIA
// at 3:13 - 7:24
int Rotate(int px, int py, int r)
{
    switch (r % 4)
    {
        case 0: return (py * 4) + px;         // 0 degrees
        case 1: return 12 + py - (px * 4);  // 90 degrees
        case 2: return 15 - (py * 4) - px;  // 180 degrees
        case 3: return py + (px * 4);       // 270 degrees
    }

    return 0;
}

// func: doesPieceFit
// nTetromino: ID of a tetromino
// nRotation: current rotation for tetromino piece
// nPosX: x-coordinate (top left cell of tetromino)
// nPosY: y-coordinate ((top left cell of tetromino))
// return: true if piece fits in position, false if it overlaps another piece
bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    const int pieceWidth = 4;
    const int pieceHeight = 4;
    for (int px = 0; px < pieceWidth; px++)
    {
        for (int py = 0; py < pieceHeight; py++)
        {
            // Get index into piece
            int pi = Rotate(px, py, nRotation);

            // Get index into field
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
                    {
                        return false; // fail on first hit
                    }
                }
            }

        }
    }

    return true;
}

int main()
{
    // Create assets
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    
    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");
    tetromino[1].append(L".X..");
    tetromino[1].append(L"....");

    tetromino[2].append(L".X..");
    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"..X.");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"..X.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"....");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"..X.");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L".X..");
    tetromino[6].append(L".X..");

    pField = new unsigned char[nFieldWidth * nFieldHeight];
    for (int x = 0; x < nFieldWidth; x++)
    {
        for (int y = 0; y < nFieldHeight; y++)
        {
            pField[(y * nFieldWidth) + x] = (x == 0 || x == (nFieldWidth - 1) || y == nFieldHeight - 1) ? 9 : 0;
        }  
    }

    // init ncurses for drawing characters to the terminal
    initscr();
    WINDOW *win = newwin(nFieldHeight, nFieldWidth, 1, 5);

    bool bGameOver = false;
    while (!bGameOver)
    {

        int nCurrentPiece = 0;
        int nCurrentRotation = 0;
        int nCurrentX = nFieldWidth / 2;
        int nCurrentY = 3;

        // GAME TIMING ===========

        // INPUT ===================

        // GAME LOGIC ==========================

        // RENDER OUTPUT ===========================

        // Draw Field
        for (int x = 0; x < nFieldWidth; x++)
        {
            for (int y = 0; y < nFieldHeight; y++)
            {
                mvwaddch(win, y,x, L" ABCDEFG=#"[pField[(y * nFieldWidth) + x]]);
            }  
        }

        // Draw Current Piece
        for (int px = 0; px < 4; px++)
        {
            for (int py = 0; py < 4; py++)
            {
                if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                {
                    mvwaddch(win, (nCurrentY + py), nCurrentX + px, nCurrentPiece + 65);
                }
            }
        }

        wrefresh(win);
    }
    

    // cleanup ncurses window
    endwin();


    return 0;
}
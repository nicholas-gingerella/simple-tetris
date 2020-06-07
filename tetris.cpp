#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>

std::wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
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
int rotate(int px, int py, int r)
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
            pField[(y * nFieldWidth) + x] = (x == 0 || x == (nFieldWidth - 1) || y == nFieldHeight - 1 || x % ((y * nFieldWidth) + x) == 0) ? 9 : 0;
        }  
    }

    // init ncurses for drawing characters to the terminal screen
    initscr();
    WINDOW *win = newwin(nFieldWidth, nFieldHeight, 0, 0);

    bool bGameOver = false;
    while (!bGameOver)
    {
        for (int x = 0; x < nFieldWidth; x++)
        {
            for (int y = 0; y < nFieldHeight; y++)
            {
                waddch(win, L" ABCDEFG=#"[pField[(y * nFieldWidth) + x]]);
            }  
        }

        wrefresh(win);
    }
    

    // cleanup ncurses window
    endwin();


    return 0;
}
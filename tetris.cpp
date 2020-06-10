#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>

std::wstring tetromino[7];
int nFieldWidth = 16;
int nFieldHeight = 17;
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
    case 0:
        return (py * 4) + px; // 0 degrees
    case 1:
        return 12 + py - (px * 4); // 90 degrees
    case 2:
        return 15 - (py * 4) - px; // 180 degrees
    case 3:
        return py + (px * 4); // 270 degrees
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
    std::srand(std::time(nullptr));

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
    clear();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    cbreak();
    WINDOW *win = newwin(nFieldHeight, nFieldWidth + 15, 0, 5);

    int nCurrentPiece = 0;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;

    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = false;
    int nPieceCount = 0;

    int nScore = 0;

    std::vector<int> vLines;

    bool bGameOver = false;
    while (!bGameOver)
    {
        // GAME TIMING ===========
        usleep(50 * 1000); //50ms
        nSpeedCounter++;
        bForceDown = (nSpeedCounter == nSpeed);

        // INPUT ===================
        int ch = getch();
        switch (ch)
        {
        case KEY_LEFT:
            //move left
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))
            {
                nCurrentX = nCurrentX - 1;
            }
            break;
        case KEY_RIGHT:
            //move right
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))
            {
                nCurrentX = nCurrentX + 1;
            }
            break;
        case KEY_DOWN:
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
            {
                nCurrentY = nCurrentY + 1;
            }
            break;
        case 'z':
            //move rotate piece
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY))
            {
                nCurrentRotation++;
                Rotate(nCurrentX, nCurrentY, nCurrentRotation);
            }

            break;
        case 'x':
            endwin();
            exit(0);
        }

        // GAME LOGIC ==========================
        if (bForceDown)
        {
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
            {
                nCurrentY++;
            }
            else
            {
                // lock piece into field
                for (int px = 0; px < 4; px++)
                {
                    for (int py = 0; py < 4; py++)
                    {
                        if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                        {
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
                        }
                    }
                }

                // Increase speed every 10 pieces
                // speed is #ticks for a move, decreasing
                // the ticks increases the speed (kinda confusing)
                if (nPieceCount % 10 == 0)
                {
                    if (nSpeed >= 10)
                    {
                        nSpeed--;
                    }
                }

                // any horizontal lines?
                for (int py = 0; py < 4; py++)
                {
                    if (nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                        {
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
                        }

                        if (bLine)
                        {
                            // Remove Line, set to '=' symbol
                            for (int px = 1; px < nFieldWidth - 1; px++)
                            {
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            }
                            vLines.push_back(nCurrentY + py);
                        }
                    }
                }

                nScore += 25;
                if (!vLines.empty())
                {
                    // double score for each line completed
                    // simultaneously ( SCORE BONUS)
                    nScore += (1 << vLines.size()) * 100;
                }

                // choose next piece
                nCurrentPiece = std::rand() % 7; //random num 0-6
                nCurrentRotation = 0;
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;

                // if piece does not fit, gameover
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }

            nSpeedCounter = 0;
        }

        // RENDER OUTPUT ===========================

        // Draw Field
        for (int x = 0; x < nFieldWidth; x++)
        {
            for (int y = 0; y < nFieldHeight; y++)
            {
                mvwaddch(win, y, x, L" ABCDEFG=#"[pField[(y * nFieldWidth) + x]]);
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

        // Draw Score
        mvprintw(0, nFieldWidth + 6, "SCORE: %8d", nScore);

        // Remove completed lines
        if (!vLines.empty())
        {
            wrefresh(win);
            usleep(400 * 1000); //500ms

            for (auto &v : vLines)
            {
                for (int px = 1; px < nFieldWidth - 1; px++)
                {
                    for (int py = v; py > 0; py--)
                    {
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                    }
                    pField[px] = 0;
                }
            }
            vLines.clear();
        }

        wrefresh(win);
    }

    // cleanup ncurses window
    endwin();

    std::cout << "Game Over!! Score: " << nScore << std::endl;

    return 0;
}
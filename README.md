# simple-tetris
Simple tetris clone for the linux terminal using ncurses. This version is based on a video tutorial by javidx9

javidx9 youtube: https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA

Terminal window Tetris tutorial video: https://www.youtube.com/watch?v=8OK8_tHeCIA

He builds and runs his version on windows using the WIN32 API to output the characters to the windows console.
My version on the other hand uses [ncurses](https://invisible-island.net/ncurses/) to display the characters to the linux terminal.

# Build
Build and run on a linux system.
Requires the ncurses development libraries to run and build.

```
sudo apt install libncurses5
sudo apt install libncurses5-dev
```
To build, simply run `make` from the source directory.

# How to play
Simply run the binary "tetris"
`./tetris`

**Controls**
* *x* -> exit the game
* *z* -> rotate piece
* *left, right, and down arrows keys* -> move the pieces around the playing field.


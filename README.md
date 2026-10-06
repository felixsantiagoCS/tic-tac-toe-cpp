# tic-tac-toe-cpp
# Tic-Tac-Toe in C++

A console tic-tac-toe game written in C++, with a computer opponent and a local two-player mode. It can be built and played in Visual Studio on Windows.

## Features

- Play against the computer or a friend on the same computer.
- Computer opponent uses minimax to choose optimal moves.
- Red X and blue O when running in a compatible Windows console.
- Scoreboard tracks X wins, O wins, and draws during the session.
- Replay rounds with alternating starting players.
- Input validation rejects invalid entries and occupied squares.
- Enter `Q` to quit.

## How the game works

The board is stored in a nine-element `std::array`. Each cell contains `X`, `O`, or a space for an empty square.

`gameResult()` checks the eight possible winning lines: three rows, three columns, and two diagonals. If there is no winner and every square is occupied, the game is a draw.

The computer plays as O. `minimax()` recursively evaluates possible future moves, scoring wins, losses, and draws. `computerMove()` selects the available square with the best score. The opponent plays optimally, so a player who also plays optimally can achieve a draw.

`readNumber()` validates menu choices and moves. `drawBoard()` displays the board and scores, while `printCell()` applies console colors on Windows.

## Run in Visual Studio

1. Install Visual Studio with the **Desktop development with C++** workload.
2. Create a **Console App** project with **C++** selected as the language.
3. Open the generated project's `.cpp` file under **Source Files**.
4. Replace that file's entire contents with the code from this repository's `.cpp` file, then save. If Source Files is empty, add a C++ source file first.
5. Select **Build > Build Solution**.
6. After the build succeeds, press **Ctrl + F5** to run.

Choose a mode, then enter a number from **1 to 9** and press Enter to place a mark in the corresponding square.

## Concepts demonstrated

- C++ arrays, loops, functions, and conditionals
- Game-state management and win detection
- Recursive search with minimax
- Input parsing and validation
- Windows console color support

// Paste this entire file into a Visual Studio C++ Console App.
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using Board = std::array<char, 9>;

// Show X in red and O in blue on Windows.
void printCell(char cell)
{
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info{};
    bool useColor = GetConsoleScreenBufferInfo(console, &info) != 0;
    if (useColor && (cell == 'X' || cell == 'O'))
    {
        WORD color = cell == 'X'
            ? FOREGROUND_RED | FOREGROUND_INTENSITY
            : FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(console, (info.wAttributes & 0xFFF0) | color);
    }
#endif
    std::cout << cell;
#ifdef _WIN32
    if (useColor)
        SetConsoleTextAttribute(console, info.wAttributes);
#endif
}

void drawBoard(const Board& board, int xWins, int oWins, int draws)
{
    std::cout << "\nX wins: " << xWins << " | O wins: " << oWins
              << " | Draws: " << draws << "\n\n";

    for (int row = 0; row < 3; ++row)
    {
        std::cout << " ";
        for (int column = 0; column < 3; ++column)
        {
            int index = row * 3 + column;
            char cell = board[index] == ' '
                ? static_cast<char>('1' + index) : board[index];
            printCell(cell);
            if (column < 2) std::cout << " | ";
        }
        std::cout << "\n";
        if (row < 2) std::cout << "---+---+---\n";
    }
    std::cout << "\n";
}

// Return X or O for a win, D for a draw, or a space to keep playing.
char gameResult(const Board& board)
{
    const int lines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (const auto& line : lines)
    {
        char mark = board[line[0]];
        if (mark != ' ' && mark == board[line[1]] && mark == board[line[2]])
            return mark;
    }

    for (char cell : board)
        if (cell == ' ') return ' ';

    return 'D';
}

// Try every possible continuation to choose the best computer move.
int minimax(Board& board, bool computerTurn, int depth)
{
    char result = gameResult(board);
    if (result == 'O') return 10 - depth;
    if (result == 'X') return depth - 10;
    if (result == 'D') return 0;

    int best = computerTurn ? -100 : 100;
    for (int i = 0; i < 9; ++i)
    {
        if (board[i] != ' ') continue;
        board[i] = computerTurn ? 'O' : 'X';
        int score = minimax(board, !computerTurn, depth + 1);
        board[i] = ' ';
        best = computerTurn ? std::max(best, score) : std::min(best, score);
    }
    return best;
}

int computerMove(Board& board)
{
    int bestScore = -100;
    int bestMove = -1;
    for (int i = 0; i < 9; ++i)
    {
        if (board[i] != ' ') continue;
        board[i] = 'O';
        int score = minimax(board, false, 0);
        board[i] = ' ';
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = i;
        }
    }
    return bestMove;
}

// Reject invalid input. Return 0 when the player quits or input ends.
int readNumber(const std::string& prompt, int maximum)
{
    while (true)
    {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input) || input == "q" || input == "Q")
            return 0;

        std::istringstream parser(input);
        int number;
        char extra;
        if ((parser >> number) && !(parser >> extra)
            && number >= 1 && number <= maximum)
            return number;

        std::cout << "Enter 1 through " << maximum << ", or Q to quit.\n";
    }
}

int main()
{
    std::cout << "TIC-TAC-TOE\n\n"
              << "1. Play against the computer\n"
              << "2. Play against a friend\n\n";

    int mode = readNumber("Choose a mode (1-2, Q to quit): ", 2);
    if (mode == 0) return 0;

    bool vsComputer = mode == 1;
    if (vsComputer) std::cout << "You are X. The computer is O.\n";
    else std::cout << "Player 1 is X. Player 2 is O.\n";

    int xWins = 0, oWins = 0, draws = 0;
    char startingPlayer = 'X';

    while (true)
    {
        Board board;
        board.fill(' ');
        char turn = startingPlayer;

        while (gameResult(board) == ' ')
        {
            drawBoard(board, xWins, oWins, draws);

            if (vsComputer && turn == 'O')
            {
                std::cout << "Computer's turn...\n";
                board[computerMove(board)] = 'O';
            }
            else
            {
                std::cout << turn << "'s turn.\n";
                int square = readNumber("Choose a square (1-9, Q to quit): ", 9);
                if (square == 0) return 0;
                if (board[square - 1] != ' ')
                {
                    std::cout << "That square is already taken. Try again.\n";
                    continue;
                }
                board[square - 1] = turn;
            }

            turn = turn == 'X' ? 'O' : 'X';
        }

        char result = gameResult(board);
        if (result == 'X') ++xWins;
        else if (result == 'O') ++oWins;
        else ++draws;

        drawBoard(board, xWins, oWins, draws);
        if (result == 'D') std::cout << "It's a draw!\n";
        else std::cout << result << " wins!\n";

        if (readNumber("Play again? 1 = Yes, 2 = No (Q to quit): ", 2) != 1)
            break;

        // Alternate who starts each round; keep the scoreboard.
        startingPlayer = startingPlayer == 'X' ? 'O' : 'X';
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}

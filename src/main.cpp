#include <windows.h>
#include <iostream>
#include <random>
#include <conio.h>
#include <chrono>
#include "maze.h"
#include "generator.h"
#include "render.h"
#include "point.h"
#include "solver.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    while (true)
    {
        Maze maze(21, 11);
        std::random_device rd;
        generateDFS(maze, 1, 1, rd());

        Point player = {1, 1};
        int steps = 0;
        Point exit = {maze.getWidth() - 2, maze.getHeight() - 2};
        auto path = bfsPath(maze, player, exit);

        auto startTime = std::chrono::steady_clock::now();
        int elapsed = 0;

        while (true)
        {
            if (_kbhit())
            {
                int ch;
                do
                {
                    ch = _getch();
                    if (ch == 0 || ch == 224)
                        ch = _getch();
                } while (_kbhit());

                int dx = 0, dy = 0;
                switch (ch)
                {
                case 'w':
                case 'W':
                case 72:
                    dy = -1;
                    break;
                case 's':
                case 'S':
                case 80:
                    dy = +1;
                    break;
                case 'a':
                case 'A':
                case 75:
                    dx = -1;
                    break;
                case 'd':
                case 'D':
                case 77:
                    dx = +1;
                    break;
                case 'q':
                case 'Q':
                    return 0;
                default:
                    break;
                }

                if (dx != 0 || dy != 0)
                {
                    int tx = player.x + dx;
                    int ty = player.y + dy;
                    if (maze.at(tx, ty) == '.')
                    {
                        player = {tx, ty};
                        ++steps;
                    }
                }
            }
            elapsed = static_cast<int>(
                std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - startTime)
                    .count());
            render(maze, player, exit, steps, elapsed, path);
            Sleep(100);

            if (player == exit)
                break;
        }

        COORD resultPos = {0, static_cast<SHORT>(maze.getHeight() + 1)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), resultPos);
        std::cout << "按 R 再来一局，按 Q 退出          " << std::endl;

        while (true)
        {
            int k = _getch();
            if (k == 'r' || k == 'R')
                break;
            if (k == 'q' || k == 'Q')
                return 0;
        }
    }
}

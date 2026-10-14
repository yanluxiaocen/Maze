#include <windows.h>
#include <iostream>
#include <random>
#include <time.h>
#include <conio.h>
#include "maze.h"
#include "generator.h"
#include "render.h"
#include "point.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Maze maze(21, 11);
    std::random_device rd;
    generateDFS(maze, 1, 1, rd());
    Point player = {1, 1};
    render(maze, player);

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
                    player = {tx, ty};
            }
        }
        render(maze, player);
        Sleep(100);
    }

    std::cin.get();
    return 0;
}

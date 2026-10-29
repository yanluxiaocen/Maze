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

        std::vector<Point> path;
        View view;

        auto startTime = std::chrono::steady_clock::now();
        int elapsed = 0;

        bool aiMode = false;
        int pathIndex = 0;
        int aiTick = 0;

        std::vector<Point> visitOrder;
        int searchTick = 0;
        bool searching = false;

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
                case 'e':
                case 'E':
                    aiMode = !aiMode;
                    if (aiMode)
                    {
                        SearchResult result = bfsSearch(maze, player, exit);
                        path = result.path;
                        visitOrder = result.visitOrder;
                        pathIndex = 0;
                        aiTick = 0;
                        searchTick = 0;
                        searching = true;
                    }
                    else
                    {
                        view.path.clear();
                        view.visited.clear();
                        searching = false;
                    }
                    break;
                }

                if (!aiMode && (dx != 0 || dy != 0))
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

            if (searching)
            {
                searchTick += 4;
                if (searchTick > static_cast<int>(visitOrder.size()))
                    searchTick = static_cast<int>(visitOrder.size());

                if (searchTick == static_cast<int>(visitOrder.size()))
                    searching = false;
            }

            if (aiMode && searchTick > 0)
                view.visited.assign(visitOrder.begin(), visitOrder.begin() + searchTick);
            else
                view.visited.clear();

            if (aiMode && !searching && !path.empty())
            {
                if (++aiTick >= 3)
                {
                    aiTick = 0;
                    if (pathIndex + 1 < (int)path.size())
                    {
                        ++pathIndex;
                        player = path[pathIndex];
                        ++steps;
                    }
                }
            }
            if (aiMode && !searching && pathIndex < static_cast<int>(path.size()))
                view.path.assign(path.begin() + pathIndex, path.end());
            else
                view.path.clear();

            view.player = player;
            view.steps = steps;
            view.seconds = elapsed;

            render(maze, view);
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

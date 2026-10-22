#include "render.h"
#include <windows.h>
#include <iostream>
#include <string>

static bool isOnPath(const std::vector<Point> &path, int x, int y)
{
    for (const Point &p : path)
        if (p.x == x && p.y == y)
            return true;
    return false;
}

void render(const Maze &maze, const Point &player, const Point &exit, int steps, int seconds, const std::vector<Point> &path)
{
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), topLeft);

    for (int y = 0; y < maze.getHeight(); y++)
    {
        std::string line;
        for (int x = 0; x < maze.getWidth(); x++)
        {
            if (x == player.x && y == player.y)
                line += '@';
            else if (x == exit.x && y == exit.y)
                line += 'E';
            else if (isOnPath(path, x, y))
                line += '*';
            else
                line += maze.at(x, y);
        }
        std::cout << line << std::endl;
    }
    std::cout << "步数: " << steps << "  耗时: " << seconds << "s          " << std::endl;
    std::cout.flush();
}
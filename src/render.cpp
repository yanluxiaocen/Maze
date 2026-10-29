#include "render.h"
#include <windows.h>
#include <iostream>
#include <string>

static bool isInVec(const std::vector<Point> &v, int x, int y)
{
    for (const Point &p : v)
        if (p.x == x && p.y == y)
            return true;
    return false;
}

void render(const Maze &maze, const View &view)
{
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), topLeft);

    for (int y = 0; y < maze.getHeight(); y++)
    {
        std::string line;
        for (int x = 0; x < maze.getWidth(); x++)
        {
            if (x == view.player.x && y == view.player.y)
                line += '@';
            else if (x == view.exit.x && y == view.exit.y)
                line += 'E';
            else if (isInVec(view.path, x, y))
                line += '*';
            else if (isInVec(view.visited, x, y))
                line += ',';
            else
                line += maze.at(x, y);
        }
        std::cout << line << std::endl;
    }
    std::cout << "步数: " << view.steps
              << "  耗时: " << view.seconds << "s"
              << "  访问: " << view.visited.size() << " 格          " << std::endl;
    std::cout.flush();
}
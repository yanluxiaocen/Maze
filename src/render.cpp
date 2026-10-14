#include "render.h"
#include <windows.h>
#include <iostream>

void render(const Maze &maze, const Point &player)
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
            else
                line += maze.at(x, y);
        }
        std::cout << line << std::endl;
    }
    std::cout.flush();
}
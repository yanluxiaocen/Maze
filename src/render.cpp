#include "render.h"
#include <windows.h>
#include <iostream>

void render(const Maze &maze)
{
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), topLeft);
    for (int i = 0; i < maze.getHeight(); i++)
    {
        std::cout << maze.grid[i] << std::endl;
    }
}
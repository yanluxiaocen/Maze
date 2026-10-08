#include <windows.h>
#include <iostream>
#include "maze.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Maze maze(21, 11);
    maze.carve(3, 5, 5, 7);
    maze.carve(9, 3, 11, 5);
    maze.carve(19, 21, 6, 8);
    for (int i = 0; i < 11; i++)
    {
        std::cout << maze.grid[i];
    }

    return 0;
}

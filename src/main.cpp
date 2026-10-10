#include <windows.h>
#include <iostream>
#include "maze.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Maze maze(21, 11);
    maze.carve(3, 5, 3, 7);
    maze.carve(9, 3, 11, 3);
    maze.carve(17, 9, 19, 9);
    for (int i = 0; i < maze.getHeight(); i++)
    {
        std::cout << maze.grid[i] << std::endl;
    }

    std::cin.get();
    return 0;
}

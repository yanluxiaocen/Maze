#include <windows.h>
#include <iostream>
#include <random>
#include "maze.h"
#include "generator.h"
#include "render.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Maze maze(21, 11);
    std::random_device rd;
    generateDFS(maze, 1, 1, rd());
    render(maze);

    std::cin.get();
    return 0;
}

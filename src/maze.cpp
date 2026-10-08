#include "maze.h"
#include <iostream>

Maze::Maze(int width, int height) : w(width), h(height)
{
    grid.resize(h);
    for (int i = 0; i < h; i++)
        grid[i] = std::string(w, '#');
}

bool Maze::inBounds(int x, int y) const
{
    if (x < 0 || y < 0)
        return false;
    return (x < w && y < h);
}

char Maze::at(int x, int y) const
{
    if (!inBounds(x, y))
        return '#';
    return grid[y][x];
}

void Maze::setAt(int x, int y, char c)
{
    grid[y][x] = c;
}

void Maze::carve(int x1, int y1, int x2, int y2)
{
    grid[(y1 + y2) / 2][(x1 + x2) / 2] = '.';
}
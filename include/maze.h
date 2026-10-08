#pragma once

#include <string>
#include <vector>

struct Maze
{
    int w;
    int h;
    std::vector<std::string> grid;

    Maze(int width, int height);

    bool inBounds(int x, int y) const;
    char at(int x, int y) const;
    void setAt(int x, int y, char c);
    void carve(int x1, int y1, int x2, int y2);
};
#pragma once
#include "maze.h"
#include "point.h"

struct View
{
    Point player;
    Point exit;
    int steps = 0;
    int seconds = 0;
    std::vector<Point> path;
    std::vector<Point> visited;
};

void render(const Maze &maze, const View &view);
#pragma once
#include <vector>
#include "maze.h"
#include "point.h"

struct SearchResult
{
    std::vector<Point> path;
    std::vector<Point> visitOrder;
};

SearchResult bfsSearch(const Maze &maze, const Point &from, const Point &to);

std::vector<Point> bfsPath(const Maze &maze, const Point &from, const Point &to);
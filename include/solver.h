#pragma once

#include <vector>
#include "maze.h"
#include "point.h"

std::vector<Point> bfsPath(const Maze &maze, const Point &from, const Point &to);
#include "generator.h"
#include "point.h"
#include <vector>
#include <random>

const int DX[4] = {0, 1, 0, -1};
const int DY[4] = {-1, 0, 1, 0};

void generateDFS(Maze &maze, int startX, int startY, unsigned int seed)
{
    std::mt19937 rng(seed);

    maze.setAt(startX, startY, '.');
    std::vector<Point> stack;
    stack.push_back({startX, startY});

    while (!stack.empty())
    {
        Point now = stack.back();
        std::vector<Point> candidates;

        for (int d = 0; d < 4; d++)
        {
            int nx = now.x + 2 * DX[d];
            int ny = now.y + 2 * DY[d];

            if (maze.inBounds(nx, ny) && maze.at(nx, ny) == '#')
                candidates.push_back({nx, ny});
        }

        if (!candidates.empty())
        {
            std::uniform_int_distribution<int> pick(0, static_cast<int>(candidates.size()) - 1);
            int idx = pick(rng);
            Point next = candidates[idx];
            maze.carve(now.x, now.y, next.x, next.y);
            maze.setAt(next.x, next.y, '.');
            stack.push_back(next);
        }
        else
            stack.pop_back();
    }
}
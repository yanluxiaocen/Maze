#include <queue>
#include <vector>
#include <algorithm>
#include "solver.h"

const int DX[4] = {0, 1, 0, -1};
const int DY[4] = {-1, 0, 1, 0};

SearchResult bfsSearch(const Maze &maze, const Point &from, const Point &to)
{
    SearchResult result;
    const int w = maze.getWidth();
    const int h = maze.getHeight();

    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
    std::vector<std::vector<Point>> parent(h, std::vector<Point>(w, {-1, -1}));

    std::queue<Point> q;
    q.push(from);
    visited[from.y][from.x] = true;

    while (!q.empty())
    {
        Point cur = q.front();
        q.pop();

        result.visitOrder.push_back(cur);

        if (cur == to)
            break;

        for (int d = 0; d < 4; d++)
        {
            int nx = cur.x + DX[d];
            int ny = cur.y + DY[d];
            if (!maze.inBounds(nx, ny))
                continue;
            if (maze.at(nx, ny) != '.')
                continue;
            if (visited[ny][nx])
                continue;

            visited[ny][nx] = true;
            parent[ny][nx] = cur;
            q.push({nx, ny});
        }
    }

    if (!visited[to.y][to.x])
        return result;

    Point cur = to;
    while (!(cur == from))
    {
        result.path.push_back(cur);
        cur = parent[cur.y][cur.x];
    }
    result.path.push_back(from);
    std::reverse(result.path.begin(), result.path.end());
    return result;
}

std::vector<Point> bfsPath(const Maze &maze, const Point &from, const Point &to)
{
    return bfsSearch(maze, from, to).path;
}
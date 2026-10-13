#include "doctest.h"
#include "generator.h"
#include "point.h"
#include "maze.h"
#include <vector>
#include <queue>

const int DX[4] = {0, 1, 0, -1};
const int DY[4] = {-1, 0, 1, 0};

static int reachableRooms(const Maze &maze)
{
    std::vector<std::vector<bool>> seen(maze.getHeight(), std::vector<bool>(maze.getWidth(), false));
    std::queue<Point> q;
    q.push({1, 1});
    seen[1][1] = true;
    int room = 0;

    while (!q.empty())
    {
        Point cur = q.front();
        q.pop();
        if (cur.x % 2 != 0 && cur.y % 2 != 0)
            room++;
        for (int d = 0; d < 4; d++)
        {
            int nx = cur.x + DX[d];
            int ny = cur.y + DY[d];
            if (maze.inBounds(nx, ny) && maze.at(nx, ny) == '.' && !seen[ny][nx])
            {
                seen[ny][nx] = true;
                q.push({nx, ny});
            }
        }
    }

    return room;
}

TEST_CASE("迷宫通路")
{
    const int W = 21, H = 11;

    for (unsigned int seed = 0; seed < 10; ++seed)
    {
        INFO("当前 seed = ", seed);

        Maze maze(W, H);
        generateDFS(maze, 1, 1, seed);

        const int totalRooms = ((maze.getWidth() - 1) / 2) * ((maze.getHeight() - 1) / 2);
        CHECK(reachableRooms(maze) == totalRooms);
    }
}
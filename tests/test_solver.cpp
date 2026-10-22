// tests/test_solver.cpp
//
// BFS 寻路的不变量测试：
//   1) 返回的路径合法（首尾正确、每步相邻、只走通路）
//   2) 路径长度就是最优（用另一套实现"洪水填充算距离"交叉验证）
//   3) 走不通时返回空
//   4) 起点就是终点时，路径长度为 1
//
// 注：本文件由 AI 代写（用户明确要求），其余业务代码一律用户手写。

#include "doctest.h"

#include "generator.h"
#include "maze.h"
#include "point.h"
#include "solver.h"

#include <queue>
#include <vector>

// 四个方向：寻路走"通路格"，步长 1（不是生成时的 ±2）
static const int DX[4] = {0, 1, 0, -1};
static const int DY[4] = {-1, 0, 1, 0};

// 文件私有辅助：洪水填充，算出 from 到每一格的最少步数；到不了的记 -1。
// 这是一套【独立于 bfsPath】的实现，用来交叉验证 bfsPath 找的确实最短。
static std::vector<std::vector<int>> bfsDistance(const Maze &maze, const Point &from)
{
    const int w = maze.getWidth();
    const int h = maze.getHeight();

    std::vector<std::vector<int>> dist(h, std::vector<int>(w, -1));

    std::queue<Point> q;
    q.push(from);
    dist[from.y][from.x] = 0;

    while (!q.empty())
    {
        Point cur = q.front();
        q.pop();

        for (int d = 0; d < 4; d++)
        {
            int nx = cur.x + DX[d];
            int ny = cur.y + DY[d];

            if (!maze.inBounds(nx, ny))
                continue;
            if (maze.at(nx, ny) != '.')
                continue;
            if (dist[ny][nx] != -1)
                continue;

            dist[ny][nx] = dist[cur.y][cur.x] + 1;
            q.push({nx, ny});
        }
    }
    return dist;
}

TEST_CASE("BFS：路径合法")
{
    const int W = 21;
    const int H = 11;

    for (unsigned int seed = 0; seed < 5; ++seed)
    {
        INFO("seed = ", seed);

        Maze maze(W, H);
        generateDFS(maze, 1, 1, seed);

        const Point from{1, 1};
        const Point to{W - 2, H - 2};

        std::vector<Point> path = bfsPath(maze, from, to);

        // REQUIRE 失败会直接中止本用例，避免后面 path.front() 越界
        REQUIRE(!path.empty());
        CHECK(path.front() == from);
        CHECK(path.back() == to);

        // 每个格子都必须是通路（不能穿墙）
        for (std::size_t i = 0; i < path.size(); ++i)
        {
            INFO("第 ", i, " 格 = (", path[i].x, ", ", path[i].y, ")");
            CHECK(maze.at(path[i].x, path[i].y) == '.');
        }

        // 相邻两格必须"差一步"（曼哈顿距离 == 1）
        for (std::size_t i = 1; i < path.size(); ++i)
        {
            int dx = path[i].x - path[i - 1].x;
            int dy = path[i].y - path[i - 1].y;
            if (dx < 0)
                dx = -dx;
            if (dy < 0)
                dy = -dy;

            INFO("第 ", i - 1, " 格 -> 第 ", i, " 格");
            CHECK(dx + dy == 1);
        }
    }
}

TEST_CASE("BFS：路径长度就是最优")
{
    const int W = 21;
    const int H = 11;

    for (unsigned int seed = 0; seed < 5; ++seed)
    {
        INFO("seed = ", seed);

        Maze maze(W, H);
        generateDFS(maze, 1, 1, seed);

        const Point from{1, 1};
        const Point to{W - 2, H - 2};

        std::vector<Point> path = bfsPath(maze, from, to);
        REQUIRE(!path.empty());

        // 交叉验证：另一套实现算出来的最短步数，必须和 path 的步数一致
        std::vector<std::vector<int>> dist = bfsDistance(maze, from);
        REQUIRE(dist[to.y][to.x] >= 0);

        const int pathSteps = static_cast<int>(path.size()) - 1;
        CHECK(pathSteps == dist[to.y][to.x]);
    }
}

TEST_CASE("BFS：走不通时返回空")
{
    Maze maze(7, 5);       // 全墙迷宫
    maze.setAt(1, 1, '.'); // 点开两个互相孤立的通路格（中间没有打通）
    maze.setAt(3, 3, '.');

    std::vector<Point> path = bfsPath(maze, {1, 1}, {3, 3});
    CHECK(path.empty());
}

TEST_CASE("BFS：起点就是终点")
{
    Maze maze(7, 5);
    maze.setAt(1, 1, '.');

    std::vector<Point> path = bfsPath(maze, {1, 1}, {1, 1});

    REQUIRE(!path.empty());
    CHECK(path.size() == 1u);
    CHECK(path.front() == path.back());
}

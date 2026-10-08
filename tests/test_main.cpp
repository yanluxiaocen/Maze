#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "maze.h"

TEST_CASE("地图初始化")
{
    Maze m(7, 5);
    bool te = true;
    int j = 0;
    for (int x = 0; x < 7; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            if (m.grid[y][x] != '#')
                te = false;
            if (x % 2 == 1 && y % 2 == 1)
                j++;
        }
    }
    CHECK(te);
    CHECK(j == 6);
    CHECK(m.at(-1, 0) == '#');
    CHECK(m.at(100, 100) == '#');
}

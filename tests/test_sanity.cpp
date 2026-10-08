// 脚手架自检用例：唯一目的是确认「测试框架真的接通了」。
//
// 这不是项目功能测试——迷宫和算法的用例会从 v1-a 开始，
// 写在旁边的 tests/test_maze.cpp / tests/test_generator.cpp 里。
// 想删掉本文件时，记得同步从 CMakeLists.txt 的 unit_tests 里去掉它。
#include "doctest.h"

TEST_CASE("脚手架自检：测试框架已接通") {
    CHECK(1 + 1 == 2);
    CHECK_FALSE(1 + 1 == 3);
}

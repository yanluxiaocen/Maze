cmake_minimum_required(VERSION 3.20)
project(Maze LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- 主程序 ----
# TODO(v1)：maze.cpp / generator.cpp / render.cpp 写好后逐个加进来
add_executable(Maze
    src/main.cpp
)
target_include_directories(Maze PRIVATE include)

# ---- 单元测试 ----
# 注意：这个 target 里绝不放带 main() 的文件（doctest 自己提供 main）
enable_testing()
add_executable(unit_tests
    tests/test_main.cpp
    tests/test_sanity.cpp
    # TODO(v1)：被测源文件（src/maze.cpp、src/generator.cpp…）在这里重新编一份
)
target_include_directories(unit_tests PRIVATE include tests)
add_test(NAME unit_tests COMMAND unit_tests)

# ---- 基准 target 留到 v5（算法对照）再加 ----
# add_executable(bench bench/bench_main.cpp src/maze.cpp src/generator.cpp src/solver.cpp)
# target_include_directories(bench PRIVATE include)

# ---- 编码参数：MSVC 用 /utf-8，g++ 用 -finput/-fexec-charset，保证中文注释/输出安全 ----
if(MSVC)
    set(MAZE_UTF8_FLAGS /utf-8)
else()
    set(MAZE_UTF8_FLAGS -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# ---- 编译警告：GCC/Clang 用 -Wall -Wextra，MSVC 用 /W4 ----
if(MSVC)
    set(MAZE_WARN_FLAGS /W4)
else()
    set(MAZE_WARN_FLAGS -Wall -Wextra)
endif()

foreach(tgt Maze unit_tests)
    target_compile_options(${tgt} PRIVATE ${MAZE_UTF8_FLAGS} ${MAZE_WARN_FLAGS})
endforeach()

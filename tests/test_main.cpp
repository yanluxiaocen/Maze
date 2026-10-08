// doctest 的 main 入口。
//
// 只干两件事：给出 main() 函数、把 doctest 的实现编进来。
// 注意：DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN 这个宏在整个项目里
// 只能出现在这一个文件中（否则 main 重复定义）。
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

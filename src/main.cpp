// Maze —— 占位主程序
//
// 这个文件只为「验证脚手架能编译、能运行」而存在，里面没有任何迷宫逻辑。
// v1-c 阶段你会亲手写正式的游戏主循环（输入 → 更新 → 渲染 → Sleep），
// 到时整份替换掉本文件内容即可。
//
// 参考：D:\Tool\VSCode\Memory\Snack\src\main.cpp（游戏主循环骨架）

#include <windows.h>

#include <iostream>

int main() {
    // 控制台按 UTF-8 读写，中文输出才不会变成乱码（Snack 同款做法）
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout << "Maze 脚手架就位：能配置、能编译、能运行、能跑测试。\n";
    std::cout << "下一步 v1-a：写 include/maze.h 的迷宫数据结构（奇偶格方案）。\n";
    return 0;
}

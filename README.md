# Maze

终端版迷宫项目：**迷宫生成 + 寻路可视化 + 人机竞速**（C++17 / STL / CMake）。

> 🚧 脚手架阶段。README 的实质内容（玩法、算法说明、三种寻路算法的实测对比数据、项目结构）
> 会在 v1 出画面之后补齐。

## 构建

```powershell
cmake -S . -B build -G "MinGW Makefiles" `
  -DCMAKE_CXX_COMPILER="D:/Tool/VSCode/mingw64/bin/g++.exe" `
  -DCMAKE_MAKE_PROGRAM="D:/Tool/VSCode/mingw64/bin/mingw32-make.exe"
cmake --build build
```

## 跑测试

```powershell
.\build\unit_tests.exe
```

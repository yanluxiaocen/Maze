# Maze 项目日志

> 本文件是"跨设备共享记忆"：每台电脑/每个账户开工前先读它，收工后更新它，然后提交推送到 GitHub。
> 由 AI 助手维护，用户负责提交推送。

## 一句话简介

终端版迷宫项目：**迷宫生成 + 寻路可视化 + 人机竞速**（C++17 / STL / CMake，纯零依赖）。
当前处于**脚手架阶段**——目录、CMake、doctest、.vscode 已就位，业务源码尚未开始写。

**完整项目规格见 `docs/MAZE_PROJECT_HANDOFF.md`**（交接文档：用户画像、环境事实、v1~v6 路线图、可复用资产），开工前先读它。

## 协作约定（与 Snack / DSForge 一脉相承）

- 所有**业务源码由本人亲手写**；AI 只负责：读代码、指出问题、解释原理、给手把手的改法。
- AI 可直接创建的是**脚手架类文件**：`CMakeLists.txt`、`.vscode/*`、`.gitignore`、`.gitattributes`、
  `tests/test_main.cpp`（doctest 入口）、占位 `src/main.cpp`、`PROJECT_NOTES.md`、`README.md` 骨架。
- **一次只给一个任务**：目标 + 改哪个文件 + 验收标准。
- **动手前先报备**：下载、安装、删文件、大重构、`git` 远端动作，一律先说方案等同意（含 `git init`）。
- **调试以实际运行为准**：涉及编译/测试的结论，AI 必须真跑一遍并贴真实输出。
- 新概念先**白话解释**（是什么 / 为什么需要 / 生活类比），再上代码。
- 不引入任何需要下载的第三方依赖（校园网受限）；不一次性输出超长代码。
- **GitHub 仓库由本人自己创建**（public）；AI 不等同于可以代建仓库。
- 源文件统一 UTF-8（g++ 用 `-finput/-fexec-charset=UTF-8`；MSVC 用 `/utf-8`）。
- **每次收工**：更新本文件 → 提醒 `git add -A && git commit && git push`（**推送前先开 Steam++**）。

## 当前状态

### 已完成

- [x] 交接文档就位：`docs/MAZE_PROJECT_HANDOFF.md`（2026-09-20 由上一对话撰写）
- [x] 项目定名 **Maze**（目录已存在，无需改名；仓库名同为 `Maze`）
- [x] 环境实测通过（真跑，非假设）：g++ 16.1.0 / cmake 4.4.3 / git 2.55.0 均在 PATH
- [x] 目录骨架：`include/` `src/` `tests/` `bench/` `docs/` `build`（构建时生成）
- [x] `tests/doctest.h` 从 `Anime_Archive_Z\tests\` 复制（328,750 字节，2.4.11，**未下载**）
- [x] `CMakeLists.txt`（**add_executable 模式**，双 target：主程序 `Maze` + `unit_tests`；编码参数与警告参数按编译器分支，抽成 `MAZE_UTF8_FLAGS` / `MAZE_WARN_FLAGS` 复用）
- [x] `.vscode/` 四件套（`tasks.json` / `launch.json` / `settings.json` / `c_cpp_properties.json`）从 Snack 复制改造，输出名改 `Maze.exe`
- [x] `.gitignore`（用 **`build*/`** 而非 `build/`，免得日后的 `build-rel/` 被误提交）+ `.gitattributes`（换行统一）
- [x] 脚手架占位源码：`src/main.cpp`（设 UTF-8 代码页 + 打印一行）、`tests/test_main.cpp`（doctest 入口）、`tests/test_sanity.cpp`（自检用例）
- [x] `README.md` 骨架（实质内容待 v1 完成后再写）

### 待办（下次严格按此顺序）

- [ ] **v1-a**：`include/maze.h` + `src/maze.cpp` —— 奇偶格迷宫数据结构（宽高取奇数；偶数行/列是墙；"打通两格之间的墙" = 改一个字符）+ 单测（尺寸、墙/格子数量关系）
- [ ] **v1-b**：`include/generator.h` + `src/generator.cpp` —— DFS 回溯生成（栈；随机取未访问邻居；打通墙后入栈，走不动就回溯）+ 单测（生成必连通）
- [ ] **v1-c**：`src/render.cpp` + `src/main.cpp` —— 静态渲染（复用 Snack 的 "grid 缓冲 + 光标回位覆盖重绘" 做法）→ **迷宫第一次出现在屏幕上**
- [ ] **`git init` + 首次提交**（等本人确认后再做）
- [ ] **建 GitHub 仓库**：本人在 GitHub 上建 `yanluxiaocen/Maze`（public）→ 本地 `git remote add origin https://github.com/yanluxiaocen/Maze.git` → push（先开 Steam++）
- [ ] v2 起照路线图推进（玩家移动 → BFS 画路 → 搜索动画 → A\* 带权 → 人机竞速）

## 关键决定记录

- **项目定名 Maze**（用户拍板；交接文档里的候选 `Ariadne` 未采用）。目录本来就叫 `Maze`，因此不涉及重命名与 build 缓存失效问题（对比踩坑 #27）。
- **沿用 Snack 的工程基座**：CMake（`add_executable` 模式，**不是** DSForge 那份 header-only `INTERFACE` 库写法）+ doctest 单测 + `-Wall -Wextra` + Debug/Release 分离 + `.vscode` 配置。
- **`.gitignore` 一开始就写 `build*/`**：Snack 用的是 `build/`，测性能的 `build-rel/` 会漏网（见踩坑）。
- **`unit_tests` target 不含 `src/main.cpp`**：doctest 自带 main，混入会重复定义 main 导致链接失败（Snack 踩坑 #22）。
- **`include/` 与 `bench/` 放 `.gitkeep`**：空目录 git 不跟踪，留个占位文件免得换机 clone 后目录消失。（不需要时可直接删。）
- **`bench` target 先注释掉**：`bench/bench_main.cpp` 要到 v5（算法对照）才有，现在挂上去只会让构建找不到源文件。
- **首次提交前先报备**：`git init` 属"动手前先报备"范围，等用户点头。

## 踩坑库

- **本项目自身的坑**：见下方"Maze 新增"。目前只有脚手架，还没有新坑。
- **通用踩坑库（34 条，教科书级）**：见 `D:\Tool\VSCode\Memory\Snack\PROJECT_NOTES.md`
  的"踩坑库"章节（`enum class` 不能从 int 构造、函数声明与定义必须逐字一致、CMake 未定义变量静默展开为空、
  目录改名后 build 缓存失效、`-Wall -Wextra` 的价值、PowerShell 的 UTF-8 编码坑……）。
  **撞上类似问题时直接引用编号**，不重复抄写。

### Maze 新增

（暂无）

## 知识点地图

脚手架阶段暂无新增知识点。开工后按 Snack 的格式累积，预计会覆盖：
奇偶格迷宫表示法、DFS 回溯生成、BFS / Dijkstra / A\* 三者的关系与差异、
优先队列（`priority_queue` 默认最大堆，要 `greater` 反转）、启发式为什么只影响搜索量不影响最优性、
Windows 控制台覆盖重绘（不闪屏）、`<random>` 的 `mt19937` + `uniform_int_distribution`。

## 常用命令备忘

- 每日开工：先读本文件 + `docs/MAZE_PROJECT_HANDOFF.md`；换机先 `git pull`
- 收工：更新本文件 → `git add -A` → `git commit -m "docs: 更新项目日志"` → `git push`（先开 Steam++）
- 首次配置（新机器 / 删了 build 目录）：
  `cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=D:/Tool/VSCode/mingw64/bin/g++.exe -DCMAKE_MAKE_PROGRAM=D:/Tool/VSCode/mingw64/bin/mingw32-make.exe`
- 日常构建：`cmake --build build`
- 跑单测（详细）：`.\build\unit_tests.exe`（`--success` 看通过项 / `-tc="名字"` 过滤 / `--list-test-cases` 列用例）
- 跑单测（批量）：`ctest --test-dir build --output-on-failure`
- 跑主程序：`.\build\Maze.exe`
- 终端看中文先敲：`[Console]::OutputEncoding = [System.Text.Encoding]::UTF8`
- VSCode：任意文件按 **F5** = 先跑 `CMake: 构建` 再启动 `build\Maze.exe`（外部控制台）

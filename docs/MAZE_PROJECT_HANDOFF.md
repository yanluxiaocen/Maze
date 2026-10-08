# 迷宫项目交接文档（写给下一个对话的 AI 助手）

> **阅读须知**：这是一份"零上下文开机"文档。你（下一个对话的 AI 助手）对之前发生的一切一无所知，请**完整读完本文件**再动手。
> 由更早的对话（Snack → DSForge）的 AI 助手撰写，目的是把**用户画像、协作硬规则、环境事实、项目规格、教学偏好**完整交接给接手者。
> 最后更新：2026-09-20。

---

## 0. 一句话摘要

用户是**自学 C++/算法、已过 PAT 乙级**的学生，正在用项目练手。**前两个项目的教训非常明确：他不缺能力，缺的是"看得见的反馈"——抽象、无交互的项目（DSForge 手写数据结构库）让他提不起劲，游戏/交互型项目（Snack 贪吃蛇）让他一路做到结项。**
用户发言：其实搞错了一点，我不是希望能尽早见到效果，只是希望不要一上来就全是自己没见过的语法和格式，有时候无从下手才是我最担心的，当然，像迷宫这种具体的目标，我认为稳扎稳打更好，没必要急功求利，这反而会带来意想不到的负面作用

**本项目定为：迷宫生成 + 寻路可视化 + 人机竞速（终端版）**——视觉反馈极强、算法密度高（DFS/Prim 生成、BFS/Dijkstra/A\* 寻路）、且能直接复用他在 Snack 里练熟的全部技能。

**项目名候选（下一个 AI 让用户拍板，推荐第一个）**：

1. **`Ariadne`** ⭐（希腊神话：阿里阿德涅给忒修斯的**线团**带他走出迷宫——名字短、好念，问起来有故事可讲）
2. `MazeForge`（延续 `DSForge` 的命名风格）
3. `PathForge` / `MazeLab` / `Labyrinth`（后者撞名较多）

**建议仓库位置**：`D:\Tool\VSCode\Memory\<项目名>`（与 Snack、DSForge 同级）

---

## 1. 用户画像（决定你怎么说话、怎么排任务）

### 1.1 技术水平

**会（可放心的）**：

- C++17：类、`enum class`、STL（`vector`/`deque`/`string`/`queue`）、const 正确性、运算符重载（基础）
- **算法**：PAT 乙级已过；BFS/DFS 网格搜索、队列、栈、二分、简单 DP 都练过（在 Snack 里亲手写过 BFS 寻路 + 父节点回溯路径）
- **工程**：CMake（配置/构建/target/INTERFACE 库/变量）、doctest 单元测试、`-Wall -Wextra`、Debug/Release 分离、`git` 双机协作
- **Windows 控制台**：`_kbhit`/`_getch` 非阻塞读键、`SetConsoleCursorPosition` 覆盖重绘（防闪屏）、`SetConsoleOutputCP(CP_UTF8)` 中文、`Sleep` 帧控制
- **实测验证习惯**：已经内化"调试以实际运行为准"

**薄弱（避开或慢慢补）**：

- 指针/内存管理、对象生命周期、模板元编程、异常安全（**DSForge 就是死在这块的学习路径上——不是不懂，是提不起劲**）
- 并发、网络、图形库、Linux 工具链（完全没碰过）

### 1.2 ⚠️ 最重要的偏好：动力优先

**这是从两个项目的对比中得出的核心结论，请务必遵守：**

| 项目 | 性质 | 结果 |
|------|------|------|
| Snack（贪吃蛇 + BFS 自动寻路 AI） | **有画面、能交互、每 20 分钟看得见变化** | 一路做到结项，还主动加功能（AI 迭代三轮，得分 70→170） |
| DSForge（手写数据结构库） | **抽象、无交互、反馈只在测试输出里** | 脚手架和第一个结构做完后失去动力，主动喊停 |

**结论：选任务时，优先保证"每一步都能在屏幕上看到变化"。**

具体做法：

- 每个阶段都要有**可视化或可玩**的产出（不要连续多个阶段只产出"通过测试"）
- 先"能看"→ 再"能玩"→ 最后"能比"（动画/交互/竞速，逐层加）
- **验收尽量可量化**（他喜欢"分数从 70 涨到 170"这种数字；讨厌"要一直盯着看"才能验收的东西）

### 1.3 沟通与协作偏好

| 他喜欢 | 他讨厌 |
| -------- | -------- |
| 中文沟通 | 术语轰炸（他明确抗议过："一堆没怎么见过的专业术语"） |
| **新概念必须白话解释**（他明确要求："有新东西的时候可以给我留更多的解释"） | 过度吹捧、空话 |
| **一次只给一个任务**，写明目标 + 改哪个文件 + 验收标准 | 一次抛一堆任务 |
| **他亲手写代码**，AI 指路、审查、解释原理、给改法 | AI 直接改他的代码（除非明确授权） |
| **实际运行验证**（AI 必须真跑编译/测试并贴真实输出） | 只在嘴上"诊断" |
| **动手前先报备**（下载、安装、删除、大重构） | **先斩后奏**（他严肃提醒过："下次不可先斩后奏了，我同意你再做"） |
| 失败当教材、边做边学 | 长文档式说教 |
| 幽默、玩梗（他把游戏结束语改成"你寄了"） | — |

### 1.4 时间模式

- 只能用**碎片时间**推进：课间 10~20 分钟、午休十几分钟、晚上若干段
- **每个任务都要能在 15~60 分钟内完成并看到结果**
- 会**换机工作**（VS2022 主力机 + 本机），换机前必须 `git commit` + `git push`

---

## 2. 协作硬规则（不可违背）

1. **代码由他亲手写**；你只负责读代码、指问题、讲原理、给手把手的改法（可以给完整参考代码让他照敲，但别替他改文件）。
   - 例外：**构建/配置文件**（`CMakeLists.txt`、`.vscode/*`、`.gitignore`）经他同意后可改；`PROJECT_NOTES.md`（项目日志）**由你维护**，他负责提交推送。
2. **一次只给一个任务**：目标 + 要改哪个文件 + 验收标准。
3. **动手前报备**：下载、安装、删文件、大范围重构，先说方案等他同意。
4. **调试以实际运行为准**：涉及编译/测试的结论，你必须真跑一遍（本机有 g++ 16.1 + cmake 4.4.3），并把真实输出贴给他。
5. **失败是最好的教材**：先让他读错误信息，再解释原因。
6. **发现 bug 直接指出**，但讲清"为什么错、怎么改、怎么验证"。
7. 新概念必须先**白话解释**（是什么 / 为什么需要 / 生活类比），再上代码。
8. **不做的事**：不引入需要下载的第三方依赖（校园网受限）；不在他没要求时大改风格或重命名（他说过"可读性暂时算了，懒得改"）；不一次输出超长代码。
9. **每次收工**：更新 `PROJECT_NOTES.md` → 提醒他 `git add -A && git commit && git push`（**推送前先开 Steam++**）。

---

## 3. 环境事实清单（照抄，别猜）

### 3.1 机器与工具链

| 项 | 值 |
| --- | --- |
| 工作区 | `D:\Tool\VSCode\Memory`（**曾用名 `Save`**，2026-09 改名） |
| 已有项目 | `Memory\Snack`（游戏，**工作流与代码参考**）、`Memory\DSForge`（数据结构库，**已暂停**）、`Memory\Anime_Archive_Z`（Qt 阶段）、`Memory\PAT`、`Memory\learn1` |
| 编译器 | `g++ 16.1.0`，**已在 PATH**（也可用全路径 `D:\Tool\VSCode\mingw64\bin\g++.exe`）；同目录有 `gdb.exe`、`mingw32-make.exe` |
| CMake | `D:\Tool\CMake\cmake-4.4.3-windows-x86_64\bin\{cmake,ctest}.exe`，**已在 PATH** |
| doctest | 本地复制：`Anime_Archive_Z\tests\doctest.h`（328,750 字节，doctest 2.4.11）——**不要下载** |
| 加速器 | `D:\Tool\Game\Steam++\Steam++.exe`（推送 GitHub 前先开；即使开着也可能报 `SEC_E_NO_CREDENTIALS`，那就换机推） |
| git | 2.55.0，身份已配（`yanluxiaocen`），`core.autocrlf=true`（可加 `.gitattributes` 统一） |

### 3.2 校园网限制（关键）

- **命令行直连外网基本不可用**：`git ls-remote` 报 `schannel: SEC_E_NO_CREDENTIALS`；`Invoke-WebRequest` 访问 github.com / cmake.org 报"基础连接已关闭"（**百度也不行**）
- **浏览器正常**（走校园网门户认证）
- 推论：**任何需要下载的方案先报备**；技术选型优先"纯 STL、零依赖"
- `winget` 在本机是坏的（返回负数退出码）

### 3.3 编码与显示（三个高频坑）

1. 源码统一 UTF-8：g++ 用 `-finput-charset=UTF-8 -fexec-charset=UTF-8`；MSVC 用 `/utf-8`
2. PowerShell 看中文先敲：`[Console]::OutputEncoding = [System.Text.Encoding]::UTF8`
3. PowerShell 读 UTF-8 文件加 `-Encoding UTF8`（默认按 GBK 读，会把合法 JSON 读成"语法错误"）

### 3.4 构建 / 测试 / 运行命令

**首次配置**：

```powershell
cmake -S . -B build -G "MinGW Makefiles" `
  -DCMAKE_CXX_COMPILER="D:/Tool/VSCode/mingw64/bin/g++.exe" `
  -DCMAKE_MAKE_PROGRAM="D:/Tool/VSCode/mingw64/bin/mingw32-make.exe"
```

**日常构建**：`cmake --build build`
**跑单测**：`.\build\unit_tests.exe`（`-tc="*名字*"` 过滤 / `--success` / `--list-test-cases`）；批量 `ctest --test-dir build --output-on-failure`
**跑 Release（测性能用）**：新建 `build-rel` 目录配 `-DCMAKE_BUILD_TYPE=Release` → `.\build-rel\bench.exe`
（`.gitignore` 里要写 **`build*/`**，否则 `build-rel/` 会被误提交）

---

## 4. 项目规格：迷宫生成 + 寻路可视化 + 人机竞速

### 4.1 定位与目标

> 在终端里**生成迷宫**、**把寻路算法的搜索过程画出来**、最后让**玩家和 AI 竞速走同一个迷宫**。

- **首要目标**：视觉反馈（迷宫长出来、搜索波纹扩散、路径被画出来）
- **次要目标**：算法密度（生成算法 + 三种寻路算法的对照与实测）
- **非目标**：不做图形库（校园网+ABI 限制，SFML 以后再说）；不追求"完美可玩"

### 4.2 交付物

1. **迷宫生成器**：至少两种（DFS 回溯式、随机 Prim；有余力加 Kruskal+并查集）
2. **寻路器**：BFS（无权最短路）、Dijkstra（带权）、A\*（启发式，曼哈顿距离）
3. **终端渲染 + 动画**：格子墙、玩家、搜索波纹（未访问/待访问/已访问/最终路径用不同字符）
4. **人机竞速模式**：玩家 WASD 走，AI 同时走，比谁先到出口
5. **单元测试**：算法不变量（生成必连通、路径合法、最短路长度一致）
6. **基准**：三种算法在同一迷宫上的访问节点数 / 耗时对比（写进 README）
7. **README**：玩法 + 算法说明 + 实测数据 + 结构说明

### 4.3 建议结构与仓库

```
<项目名>/
├── include/            # maze.h（迷宫数据）/ generator.h / solver.h / render.h
├── src/                # 对应实现 + main.cpp（游戏主循环）
├── tests/              # test_main.cpp（doctest 入口）+ test_generator.cpp / test_solver.cpp
├── bench/               # 算法对照基准
├── CMakeLists.txt       # 参考 §5.1（这次有 src/，用 add_executable 模式）
├── README.md
├── PROJECT_NOTES.md     # 跨设备共享记忆（AI 维护）
└── .vscode/             # 从 Snack / DSForge 复制改造
```

**可直接复用的脚手架**：`D:\Tool\VSCode\Memory\Snack\.vscode\*` 与 `Memory\DSForge\CMakeLists.txt`（注意 DSForge 那份是 header-only `INTERFACE` 库，本项目要改回 `add_executable` 模式）。

### 4.4 关键技术设计（开局就要定对）

**① 迷宫的表示法（推荐"奇偶格"方案）**

最省事的做法是把迷宫放大一倍：**奇数行/奇数列是"格子"，偶数行/偶数列是"墙"**。

```
###########
#.#...#...#
#.#.#.#.#.#
#...#...#.#
###########
```

- 好处：墙就是字符位置，渲染直观；生成时"打通两格之间的墙"就是改一个字符
- 数据集：`width`/`height` 为奇数（如 41×21），内部格子数 `(w-1)/2 × (h-1)/2`
- 也可以用"每格 4 面墙"的 bitset 表示（更"正统"，但渲染要转换）——**二选一，别混**

**② 生成算法（v1 用 DFS 回溯式）**

```
从起点格开始：标记已访问、入栈
while 栈非空:
    取栈顶，若它有未访问的邻居 → 随机选一个、打通两格之间的墙、把它入栈
    否则 → 出栈（回溯）
```

直觉解释：像"蚯蚓钻土"，一直往没去过的地方钻，走不动就退回上一个岔路口。

**③ 寻路的三种算法（逐层加）**

- **BFS**：无权最短路；用队列；`visited` + `parent` 表回溯路径（**他在 Snack 里已经写过一遍**）
- **Dijkstra**：把迷宫格子加上"地形代价"（草地 1、沼泽 5、水 10）→ 用优先队列（`priority_queue`，注意是最大堆，要用 `greater` 反转）
- **A\***：Dijkstra + 启发式（到终点的曼哈顿距离）→ 直观对比"访问的格子数减少了多少"，**这是项目最有说服力的数据**

**④ 动画渲染（复用 Snack 的经验，别用 `system("cls")`）**

```cpp
COORD topLeft = {0, 0};
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), topLeft);   // 光标回位覆盖重绘，不闪屏
```

逐帧 `Sleep(20~60ms)` 就能看到搜索波纹扩散。**这一条是 Snack 踩出来的经验，务必复用**。

**⑤ 人机竞速**

- 玩家：复用 Snack 的输入轮询（`_kbhit` + `_getch` + 方向键两字节处理 + 清空排队键）
- AI：每 N 帧走一步（从它算好的路径上取下一个格子）
- 显示：两条路径用不同字符画在同一个迷宫上，谁先到出口直接看得出来

### 4.5 分阶段路线图（**每阶段都看得见东西**）

| 阶段 | 内容 | 看得见什么 | 估时 |
| ------ | ------ | ----------- | ------ |
| **v1** | 奇偶格数据集 + DFS 回溯生成 + 静态渲染 | **迷宫出现在屏幕上** | 2~3 h |
| **v2** | 玩家移动（WASD/方向键）+ 走到出口判定 + 步数/计时 | **你能亲手走迷宫** | 2~3 h |
| **v3** | BFS 寻路 + 路径绘制 + AI 自动走（按 E 切换） | **AI 把路画出来并走出去** | 3~4 h |
| **v4** | 搜索过程动画（波纹扩散）+ DFS/Dijkstra 对照 + 访问节点数统计 | **看到不同算法的"探索风格"差异** | 3~5 h |
| **v5** | A\* + 带权地形（草地/沼泽/水）+ 三算法基准对比 | **数据表：A\* 访问格数远少于 Dijkstra** | 3~4 h |
| **v6** | 人机竞速模式 + 多生成算法对比 + 视野受限（迷雾）等花活 | **同一个迷宫里人机同台竞技** | 4~6 h |

**总量约 20~25 小时**，但 **v1 只需 2~3 小时就能看到第一张迷宫图**——务必先把这个甜头给他。

### 4.6 测试策略（继续练测试驱动，但别喧宾夺主）

算法不变量最适合写单测：

- 生成的迷宫**必然连通**（从起点 BFS 能到达所有格子）
- 生成的迷宫**墙/格子数量关系正确**（奇偶格方案下墙数固定）
- BFS 找到的路径**合法**（每一步相邻、不穿墙、不越界）
- **BFS 路径长度 == Dijkstra 在无权图上的结果**（交叉验证两种实现）
- A\* 与 Dijkstra 的**路径长度相同**（启发式只影响搜索量，不影响最优性）
- 目标：10~20 条用例（够用即可，别为凑数写测试）

### 4.7 基准策略

- 固定随机种子（`srand(固定值)`）生成**同一张迷宫**，跑三种算法对比：
  - 访问节点数（最能说明问题）
  - 耗时（`std::chrono::steady_clock`，Release 下测）
- 结果表格写进 README：**"A\* 比 Dijkstra 少访问 60% 的格子"** 这种句子是简历上的亮点

---

## 5. 继承资产与复用清单

### 5.1 CMakeLists.txt 模板（**本项目用 add_executable 模式**）

```cmake
cmake_minimum_required(VERSION 3.20)
project(<项目名> LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- 主程序 ----
add_executable(<主程序> src/main.cpp src/maze.cpp src/generator.cpp src/solver.cpp src/render.cpp)
target_include_directories(<主程序> PRIVATE include)

# ---- 单元测试（绝不含任何带 main() 的文件：doctest 自己提供 main）----
enable_testing()
add_executable(unit_tests
    tests/test_main.cpp
    tests/test_generator.cpp
    tests/test_solver.cpp
    src/maze.cpp src/generator.cpp src/solver.cpp     # 被测代码重新编一份
)
target_include_directories(unit_tests PRIVATE include tests)
add_test(NAME unit_tests COMMAND unit_tests)

# ---- 基准（独立 target）----
add_executable(bench bench/bench_main.cpp src/maze.cpp src/generator.cpp src/solver.cpp)
target_include_directories(bench PRIVATE include)

# ---- 编码参数 + 警告开满（变量复用；别漏空格！）----
if(MSVC)
    set(UTF8_FLAGS /utf-8)
    set(WARN_FLAGS /W4)
else()
    set(UTF8_FLAGS -finput-charset=UTF-8 -fexec-charset=UTF-8)
    set(WARN_FLAGS -Wall -Wextra)
endif()

foreach(tgt <主程序> unit_tests bench)
    target_compile_options(${tgt} PRIVATE ${UTF8_FLAGS} ${WARN_FLAGS})
endforeach()
```

### 5.2 .vscode 配置

从 `D:\Tool\VSCode\Memory\Snack\.vscode\`（或 DSForge）复制改造：

- `tasks.json`：`CMake: 构建`（默认）+ `CMake: 首次配置`
- `launch.json`：`cppdbg` + `gdb.exe` + `externalConsole: true`（**游戏必须有真控制台**）+ `preLaunchTask`
- `settings.json`：`cmake.cmakePath`、`files.exclude` 隐藏 `**/build*/`

### 5.3 doctest 接法

- `tests/test_main.cpp` 两行：`#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` + `#include "doctest.h"`
- `TEST_CASE("中文描述") { CHECK(...); CHECK_FALSE(...); }`
- **该宏只能出现在一个文件里**

### 5.4 Snack 项目可复用的**代码级**资产（重点！）

路径：`D:\Tool\VSCode\Memory\Snack`

| 资产 | 位置 | 直接复用方式 |
| ------ | ------ | ------------- |
| **grid 渲染 + 光标回位覆盖重绘（不闪屏）** | `src/UI.cpp` | 抄结构：`char grid[H][W]` 铺底 → 填内容 → `SetConsoleCursorPosition(0,0)` → 逐行输出 |
| **无回车键盘 + 方向键两字节 + 清空排队键** | `src/main.cpp` 输入块 | 方向键 `_getch()` 先返回 0/224，再返回 72/75/77/80；每帧只认最后一次按键（`do-while(_kbhit())`） |
| **游戏主循环骨架**（输入→更新→渲染→Sleep） | `src/main.cpp` | 直接照搬结构 |
| **BFS + 父节点表回溯路径** | `src/AutoPilot.cpp` | **寻路部分几乎可以直接搬**（`bfsPath` / `bfsFirstStep`） |
| **纯函数 + 单测的 AI 分层设计** | `src/AutoPilot.cpp` + `tests/test_autopilot.cpp` | 寻路器写成纯函数（输入迷宫+起终点 → 输出路径），测试极好写 |
| **日志结构、踩坑库、知识点地图** | `PROJECT_NOTES.md` | 照抄格式 |

### 5.5 踩坑库精华（前两个项目累积，新项目大概率还会撞上）

**C++ 语言**

1. `enum class` 不能从 `int` 构造（`Direction(0)` 编译错，写 `Direction::Up`）
2. 容器对象不能和 `int` 比（要比长度用 `.size()`）
3. C++17 里 `x != y` 需要显式 `operator!=`（C++20 才自动推导）→ 测试写 `CHECK_FALSE(x == y)`
4. **函数定义必须和声明逐字一致**（`const T&` 写成 `T` = 另一个重载 → `undefined reference`）
5. **声明了函数却忘了写定义** → 编译过、链接报 `undefined reference`
6. **有返回值的调用结果被丢弃**（`f(x);` 忘了 `return`）→ 逻辑退化，编译器不报错
7. **逗号表达式**：`return (a, b, c);` 合法但只返回最后一个值（不是函数调用！）
8. **模板只声明不定义** → `undefined reference to Vector<int>::...`（模板实现必须在头文件里；非模板项目不涉及）
9. **引用/迭代器失效**：容器扩容或删除后，旧引用/指针/迭代器可能作废（Snack 里 `move()` 先拷贝 head 就是这个原因）
10. 浮点比较用 `doctest::Approx`，别用 `==`

**CMake / 构建**
11. **CMake 是两步**：先"配置"（`-S . -B build -G ...`）再"构建"（`--build build`）；报 "not a directory / 不是目录" 实为"没配置过"
12. **新源文件忘了加进 `add_executable`** → `undefined reference`；看构建日志有没有那行 `.obj`
13. **`target_link_libraries` 名字拼错** → 链接器报 `cannot find -lXXX`（`-l` 后面就是线索）
14. **CMake 未定义变量静默展开为空**（`${XXX}` 拼错一个字 = 参数全空、零报错）
15. **CMake 列表靠空格分隔**：`${A}${B}` 会连体，必须写 `${A} ${B}`
16. **目录改名/移动后 `build/` 缓存失效**（写死了旧绝对路径）→ 删 `build/` 重新配置
17. **`.gitignore` 的 `build/` 只匹配该名字**，不匹配 `build-rel/` → 用 `build*/`

**工具与环境**
18. **`-Wall -Wextra` 必开**：能抓出未使用参数/变量/死代码（Snack 开警告当天就抓到两个真 bug）
19. **PowerShell 显示中文乱码** → `[Console]::OutputEncoding = [System.Text.Encoding]::UTF8`
20. **`Get-Content` 默认 GBK** 读 UTF-8 → 加 `-Encoding UTF8`
21. **MSVC 读无 BOM 源码按 GBK** → 要 `/utf-8`
22. **改代码没 Ctrl+S** → 排查"改了没反应"前先检查保存
23. **测性能必须 Release**（Debug 测出来的是"编译器有多慢"）；`volatile` 防优化；用 `steady_clock`
24. **git 直连 GitHub 失败**（校园网）→ 开 Steam++ 或换机推送
25. **单元测试用例互相独立**（别用 `static` 保存状态）；期望值别恰好等于默认值（否则函数坏了测试也"蒙对"）

### 5.6 DSForge 的现状（**已暂停，别混淆**）

- 路径：`D:\Tool\VSCode\Memory\DSForge`；日志：`PROJECT_NOTES.md`
- 已完成：脚手架（CMake/.vscode/doctest/.gitignore/bench）+ `include/vector.h` 接口骨架 + 基础测试
- **暂停原因**：抽象、无交互，用户失去动力（**不是能力问题**）
- 可参考的东西：CMake 变量技巧、`.gitignore` 通配、`.vscode` 配置、日志里那份很详细的"知识点地图"
- **不要**在新项目里劝他回去做 DSForge（除非他主动提）

---

## 6. 教学风格建议

### 6.1 每个任务的标准节奏

1. **讲清目标 + 看得见什么**（"这一步做完，屏幕上会出现什么"）
2. **新概念白话解释**（是什么 / 为什么需要 / 生活类比）
3. **给结构或伪代码**（不是完整代码）
4. **让他亲手写**
5. **你实际编译 + 运行 + 贴真实输出**
6. **用失败教学**（先生读错误，再解释）
7. **收尾**：更新日志 + 提醒提交推送

### 6.2 讲"为什么"，但用他看得见的东西讲

- 为什么 DFS 生成的迷宫"走廊很长"？（因为它是"钻到底再回头"）
- 为什么 A\* 比 Dijkstra 少访问那么多格子？（因为启发式把搜索"拉向终点"）
- 为什么 BFS 的波纹是圆形扩散、A\* 是锥形？（同上）
- **都用屏幕上看到的现象来解释**，比讲渐近复杂度有效得多

### 6.3 别做的事

- 别让连续两个阶段都只产出"测试通过"（他会失去动力）
- 别一上手就要求"完美架构"（先把迷宫画出来最重要）
- 别一次给多个任务
- 别在没实测的情况下下结论

---

## 7. 第一个会话的开场清单（照此执行）

1. **读本文件** → 再扫一眼 `D:\Tool\VSCode\Memory\Snack\PROJECT_NOTES.md`（了解工作流长什么样）
2. **环境实测**（用 pwsh 真跑，别假设）：

   ```powershell
   g++ --version ; cmake --version
   Test-Path "D:\Tool\VSCode\Memory\Anime_Archive_Z\tests\doctest.h"
   Test-Path "D:\Tool\VSCode\Memory\Snack\.vscode"
   ```

3. **和他确认两件事**：
   - 项目名（候选见 §0，推荐 `Ariadne`）
   - 仓库位置（建议 `D:\Tool\VSCode\Memory\<项目名>`）；是否现在建 GitHub 远程仓库（**需先开 Steam++**，也可以先本地开工）
4. **搭脚手架**（30 分钟内，先让他看到"空项目能构建 + 测试跑绿"）：
   - 建 `include/ tests/ bench/ src/`
   - 复制改造 `CMakeLists.txt`（照 §5.1）、`.vscode/*`（照 §5.2）、`doctest.h`
   - 写最小 `test_main.cpp` + 一条自检用例 → 配置 → 构建 → 跑测试（应显示 1 用例通过、零警告）
   - 写 `PROJECT_NOTES.md` 初版（照 Snack 的结构）
   - `git init` / 首次提交（**先报备**）
5. **开 v1：迷宫生成 + 静态渲染**——目标是**让迷宫出现在屏幕上**（2~3 小时内完成）。
   先讲：奇偶格表示法（为什么这么设计）→ DFS 回溯生成（直觉 + 伪代码）→ 渲染（复用 Snack 的 grid 做法）→ 让他写 → 你实测截图/输出给他看。

---

## 8. 给他的开场问候（可直接说）

> 欢迎来到迷宫项目！我读了交接文档，知道你的偏好：**代码你写、我指路审查；一次一个任务；动手前先报备；一切以实际运行为准**；以及最重要的一条——**每个阶段都要让你在屏幕上看到变化**。
> 今天建议先花半小时搭脚手架（从 Snack / DSForge 搬现成的），然后直接开 v1：**DFS 回溯生成迷宫 + 终端渲染**，目标就是 2~3 小时内让一张迷宫图出现在你屏幕上。
> 先告诉我：项目名就用 `Ariadne` 吗？仓库建在 `D:\Tool\VSCode\Memory\Ariadne` 行不行？

---

## 附录 A：项目速查

| 项目 | 类型 | 状态 | 可复用资产 |
| ------ | ------ | ------ | ----------- |
| `Memory\Snack` | 控制台贪吃蛇 + BFS 自动寻路 AI | **已结项**（AI 得分 70→170） | grid 渲染、键盘输入、主循环、BFS+回溯、`PROJECT_NOTES` 结构、34 条踩坑 |
| `Memory\DSForge` | 手写数据结构库 | **已暂停**（动力不匹配） | CMake 技巧、`.vscode`、日志里的知识点地图 |
| `Memory\Anime_Archive_Z` | 动漫档案管理（已到 Qt） | 进行中 | `doctest.h`、CMake presets 经验 |

## 附录 B：用户原话（理解他的偏好）

- "以后有新东西的时候可以给我留更多的解释。"
- "下次不可先斩后奏了，我同意你再做。"
- "我打算不同的项目分不同的对话，因为你们的上下文长度有限。"
- "虽然合格了乙级，但我认为自己的基本功没有得到系统性的学习，全都是靠刷题总结经验得到的结果。"
- "**感觉自己不是很适合这个项目，虽然有很多解释说明，但几乎都看不懂**……**主要是提不起劲，或许还是游戏、有交互的项目更适合我一些**。"（DSForge 暂停时的原话——**这份文档最重要的一条信息**）
- "项目要完成，不要完美。"
- 关于 CMake："以后的项目都是直接用 cmake 吗？"（**约定：单文件练习直接 `g++`；多文件项目用 CMake**）

# Maze 项目日志

> 本文件是"跨设备共享记忆"：每台电脑/每个账户开工前先读它，收工后更新它，然后提交推送到 GitHub。
> 由 AI 助手维护，用户负责提交推送。
> 最后更新：**2026-10-29**

## 一句话简介

终端版迷宫项目：**迷宫生成 + 寻路可视化 + 人机竞速**（C++17 / STL / CMake，纯零依赖）。

**进度：v1 + v2 + v3 + v4（a/b）完成** —— 迷宫随机生成、原地重绘、玩家能亲手走、出口判定、步数计时、按 R 重开；
BFS 求最短路；**按 `E` 能看到 AI 的"搜索波纹"逐帧扩散、然后沿路径自己走过去**。
`Maze`（数据）/ `generateDFS`（生成）/ `solver`（寻路 + 搜索记录）/ `render`（显示，收 `View`）/ 主循环 五层；
**单测 7 用例 / 611 断言全绿、clean 构建零警告**。
**下一步 v5：换成 A\*，和 BFS 对比"访问格数"** —— 那片 `,` 会小一大截，这是 README 里最值钱的数据。

**完整项目规格见 `docs/MAZE_PROJECT_HANDOFF.md`**（交接文档：用户画像、环境事实、v1~v6 路线图、可复用资产），开工前先读它。

> ⚠️ **两台机器的环境完全不同，两份文档里的"环境事实"只对主力机成立！**（2026-10-11 实测补齐）
> **本机（VS2022 这台）没有 `g++` / `cmake` / `mingw64`**，`D:\Tool\CMake`、`D:\Tool\VSCode\mingw64` **都不存在**；
> 只有 VS2022 自带的一套（MSVC + 它自己的 cmake/ninja），而且 **Windows SDK 装在非默认的 `E:\Windows Kits\10`**。
> 详细事实与命令见 **「本机环境事实（VS2022 这台）」** 一节，动手前先看那一节，别照抄下面的 MinGW 命令。

## 协作约定（与 Snack / DSForge 一脉相承）

- 所有**业务源码由本人亲手写**；AI 只负责：读代码、指出问题、解释原理、给手把手的改法。
- AI 可直接创建的是**脚手架类文件**：`CMakeLists.txt`、`.vscode/*`、`.gitignore`、`.gitattributes`、
  `tests/test_main.cpp`（doctest 入口）、占位 `src/main.cpp`、`PROJECT_NOTES.md`、`README.md` 骨架。
- **一次只给一个任务**：目标 + 改哪个文件 + 验收标准。
- **动手前先报备**：下载、安装、删文件、大重构、`git` 远端动作，一律先说方案等同意（含 `git init`）。
- **调试以实际运行为准**：涉及编译/测试的结论，AI 必须真跑一遍并贴真实输出。
- 新概念先**白话解释**（是什么 / 为什么需要 / 生活类比），再上代码。
- 不引入任何需要下载的第三方依赖（校园网受限）；不一次性输出超长代码。
- **GitHub 仓库由本人自己创建**（public）；AI 不代建仓库。
- 源文件统一 UTF-8（g++ 用 `-finput/-fexec-charset=UTF-8`；MSVC 用 `/utf-8`）。
- **每次收工**：更新本文件 → 提醒 `git add -A && git commit && git push`（**推送前先开 Steam++**）。

## 当前状态

### 已完成

**脚手架（2026-10-08）**

- [x] 交接文档就位：`docs/MAZE_PROJECT_HANDOFF.md`（上一对话撰写）
- [x] 项目定名 **Maze**（目录已存在，无需改名；仓库名同为 `Maze`）
- [x] 环境实测通过（真跑，非假设）：g++ 16.1.0 / cmake 4.4.3 / git 2.55.0 均在 PATH
- [x] 目录骨架：`include/` `src/` `tests/` `bench/` `docs/`
- [x] `tests/doctest.h` 从 `Anime_Archive_Z\tests\` 复制（328,750 字节，2.4.11，**未下载**）
- [x] `CMakeLists.txt`（**add_executable 模式**，双 target：主程序 `Maze` + `unit_tests`；编码参数与警告参数按编译器分支，抽成 `MAZE_UTF8_FLAGS` / `MAZE_WARN_FLAGS` 复用）
- [x] `.vscode/` 四件套（`tasks.json` / `launch.json` / `settings.json` / `c_cpp_properties.json`）从 Snack 复制改造，输出名改 `Maze.exe`
- [x] `.gitignore`（用 **`build*/`** 而非 `build/`，免得日后的 `build-rel/` 被误提交）+ `.gitattributes`（换行统一）
- [x] `tests/test_main.cpp`（doctest 入口）、`tests/test_sanity.cpp`（脚手架自检）、`README.md` 骨架

**版本库与远端（2026-10-08）**

- [x] `git init` → 分支名 **`main`**（本机 `init.defaultBranch = main`；Snack 那个 `master` 是早先建库的产物，两者日常操作无差别）
- [x] 首次提交 `fbacfa3`：`chore: 搭好脚手架（CMake + doctest + .vscode）`
- [x] 用户自行在 GitHub 建 **public** 仓库 `yanluxiaocen/Maze`
- [x] 远端接上并推送成功：`git remote add origin https://github.com/yanluxiaocen/Maze.git` → `git push -u origin main`
      （**HTTPS 而非 SSH**：校园网墙了 22 端口；凭据走系统级 `credential.helper = manager`）

**v1-a 迷宫数据层 + 首次上图（2026-10-08 ~ 2026-10-10 完成）**

- [x] `include/maze.h`：`Maze` 结构（w/h/grid 公开成员 + 5 个函数声明）
- [x] `src/maze.cpp`：
      `Maze(width, height)` 造"全是墙"的迷宫（`grid.resize(h)` + 每行 `std::string(w, '#')`）、
      `inBounds`（负数与上界都管）、
      `at`（**越界一律返回 `'#'`**）、
      `setAt`、`carve`（`grid[(y1+y2)/2][(x1+x2)/2] = '.'`，取中点找门）
- [x] `tests/test_main.cpp` 用例"地图初始化"：全墙校验 + 房间数 == 6 + 两个越界取值
- [x] `CMakeLists.txt` 挂上 `src/maze.cpp`（**两个 target 都要挂**：`Maze` 与 `unit_tests`）
- [x] 实测验收：`cmake --build build --clean-first` **零警告**；`.\build\unit_tests.exe` **2 用例 / 6 断言全绿**
- [x] `src/main.cpp`：造 21×11 迷宫 + 手动 `carve` 三个门 + 逐行打印 → **迷宫骨架首次出现在屏幕上** 🎉
      实测输出：11 行 × 每行 21 字符，三个门落在 `(10,3)` `(3,6)` `(18,9)` —— **全都在"一奇一偶"的合法门位置上**，
      这是 `carve` 取中点算法正确性的直接证据（算歪一点就会落到偶偶承重柱上）
- [x] `include/maze.h` + `src/maze.cpp` 追加 `getWidth()` / `getHeight()`（**用户主动加的**，打印循环改用 `maze.getHeight()` 不再写死 11）。
      当时缺 `const`，**后已补上** —— 补得正是时候：v1-c 的 `render(const Maze&)` 立刻用上了（见"知识点地图"里的 const 成员函数）

**v1-b DFS 回溯生成（2026-10-10 完成）**

- [x] `include/point.h`：`Point { int x; int y; }` —— **纯数据口袋（聚合类型）**，用 `Point{1, 2}` / `{x, y}` 构造，不写构造函数
- [x] `include/generator.h` + `src/generator.cpp`：`void generateDFS(Maze& maze, int startX, int startY, unsigned int seed)`
      - 找"邻居房间"用方向数组 `DX/DY` 走 **±2**（房间之间隔着一格墙）；中间那道墙交给 `carve` 自己算中点
      - **"去过没去过"直接用迷宫本身记录**（房间还是 `'#'` = 没挖过），**不需要额外的 visited 表**
      - `while` 栈非空：`back()` **看**栈顶（不是弹）→ 收集候选 → 非空就随机挑一个挖过去，空才 `pop_back()` **回溯**
      - 随机用 `<random>`：`std::mt19937 rng(seed)` **建一次、全程共用** + `uniform_int_distribution` 抽签
- [x] `tests/test_generator.cpp`：**文件私有 `static`** 辅助函数 `reachableRooms()`（洪水填充 = BFS），用例"迷宫通路"跑 seed 0~9
- [x] 实测验收：AI 探针跑 **20 个种子全部通过**（可达房间 50/50；门数 49 = 房间数 − 1 = **生成树**；承重柱 0 个被碰）；
      clean 构建零警告；`.\build\unit_tests.exe` 全绿；**连跑两次指纹不同**（每次都是新迷宫）

**v1-c 渲染拆成模块 + 原地覆盖重绘（2026-10-10 完成）**

- [x] `include/render.h` + `src/render.cpp`：`void render(const Maze& maze)`
      —— 参数是 **`const Maze&`（只读引用）**；用 `GetStdHandle` + `COORD` + `SetConsoleCursorPosition` 做**光标回位覆盖重绘**
- [x] `src/main.cpp`：删掉内联 `for` + `cout`，改调 `render(maze)`
- [x] 顺手把 `tests/test_generator.cpp` 里的**变长数组（VLA）**换成 `std::vector<std::vector<bool>>`（见 M15）
- [x] 实测验收：连跑三次，屏幕上**始终只有一张迷宫**（覆盖重绘生效，不再往下滚出一屏又一屏）

**v2-a 玩家移动（2026-10-10 完成）**

- [x] `include/render.h` + `src/render.cpp`：签名加玩家 → `void render(const Maze& maze, const Point& player)`；
      逐行**攒成一个 `std::string` 再输出**（比逐字符 `cout` 干净也快），画到玩家那格时输出 `'@'`，帧末统一 `flush()`
- [x] `src/main.cpp` 换成**游戏主循环**：`while (true) { 输入 → 更新 → render → Sleep(100) }`
- [x] 键盘输入（照 `D:\Tool\VSCode\Memory\Snack\src\main.cpp` 的 **52~101 行**抄的）：
      `_kbhit()` 非阻塞 + `_getch()` 无回车；`do { ch = _getch(); if (ch == 0 || ch == 224) ch = _getch(); } while (_kbhit())`
      —— **把排队的键掏空、每帧只认最后一次按键**；`switch` 同时接 WASD 与扫描码（72↑ 75← 77→ 80↓）；`Q` 退出
- [x] 移动：`switch` 里直接给出 `dx` / `dy`（**没有引入第三份 `DX/DY`**）；`maze.at(tx, ty) == '.'` 才许走（**撞墙判定**）；
      玩家起点 `Point player = {1, 1}`；**步长是 1**（走通路格，不是生成时的 ±2）
- [x] 实测验收：clean 零警告、单测全绿；AI 用看门狗跑主程序 1.5 秒抓到 **14 帧，每帧 11 行 × 21 字符、恰好一个 `@`**；
      用户实机确认"效果不错"

**v2-b 出口 + 步数 + 计时 + 重开（2026-10-20 完成）**

- [x] `src/render.cpp`：出口画 `'E'`（玩家那一格优先 `'@'`）；迷宫下方多画一行状态 `步数: N  耗时: Xs`
      （行尾补空格 + `endl`，用来擦掉上次更长的残留）
- [x] `src/main.cpp`：**外层局循环 + 内层本局循环**；`Point exit = {w-2, h-2}`；`int steps` 只在**真的移动了**才 `++`
- [x] **计时**（完整用法见"知识点地图 · 计时"）：
      `auto startTime = std::chrono::steady_clock::now()` 放**外层**（一局只取一次）；
      `elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count())`
      放**内层循环、每帧重算**
- [x] 通关判定 `player == exit`（用的就是 v1-b 后在 `point.h` 里加的 `operator==`）；
      结算写到**固定行**（`SetConsoleCursorPosition` 定位到第 `h+1` 行），不会每重开一局就往下漂
- [x] 结算：`通关！你用了 N 步、X 秒` + `按 R 再来一局，按 Q 退出`；读键改用 `_getch()`（不按回车），`R` → `continue` 换新迷宫
- [x] 实测：用户自测通过；AI 复跑 `Maze.exe`，帧里有 `@` 和 `E` 各一个、状态行 `步数: 0  耗时: 0s`；
      单测 3 用例 / 16 断言全绿、clean 构建零警告

**v3-a BFS 寻路 + 路径绘制（2026-10-22 完成）**

- [x] `include/solver.h` + `src/solver.cpp`：`std::vector<Point> bfsPath(const Maze&, const Point& from, const Point& to)`
      —— `visited` + `parent` 两张表（都是 `[y][x]`），BFS 队列步长 **±1**，从 `parent` 回溯后 `std::reverse`；
      走不通返回 `{}`，起点==终点返回 1 格
- [x] `src/render.cpp`：签名加 `const std::vector<Point>& path`，`isOnPath()`（**文件私有 `static`**）判断后画 `'*'`
- [x] `src/main.cpp`：`auto path = bfsPath(maze, player, exit);` 放在**内层循环外**（迷宫不变，算一次就够）
- [x] `include/point.h`：**删掉手写的 `operator=`**（见 M21，它让赋值静默失效 —— 玩家原本根本不会动）
- [x] `tests/test_solver.cpp`：**AI 代写**（用户明确要求，属协作约定的例外）。4 条用例：
      路径合法（首尾 + 每步相邻 + 只走通路）、**长度最优（用独立的洪水填充算距离交叉验证）**、不可达返回空、起点==终点
- [x] 实测：**7 用例 / 611 断言全绿**、零警告；探针验证 seed=42 时路径 63 格（62 步）且合法

**v3-b 按 E 切换「显示路径 / AI 自动走」（2026-10-26 完成）**

- [x] `src/main.cpp` 新增状态：`bool aiMode` / `std::vector<Point> path` / `int pathIndex` / `int aiTick`，
      **全部声明在外层局循环里** —— 按 R 重开时自动重置，不会"重开以后 AI 还在自己开"
- [x] 按 `E` 切换：**进入 AI 模式时才**算 `path = bfsPath(maze, player, exit)`。两个好处：
      默认不算 → 不剧透；路径从**玩家当前位置**出发 → AI 像"从你身边接管"，不是从头再走一遍
- [x] AI 推进：`if (++aiTick >= 3)` 才走一格（**帧计数**控制节奏）→ `++pathIndex; player = path[pathIndex]; ++steps;`
- [x] 玩家输入：`if (!aiMode && (dx != 0 || dy != 0))` —— AI 开车时方向键无效（见 M22，括号不能省）
- [x] 渲染：`render(..., aiMode ? path : std::vector<Point>())` —— **`render.cpp` 一行都没改**
      （`render` 只负责"把给它的那段画出来"，`pathIndex` 这种游戏状态留在 `main`）
- [x] 实测：默认帧只有 `#` `.` `@` `E`（无 `*`）；7 用例 / 611 断言全绿、零警告
- [x] **用户决定：AI"走过路径逐帧清除"的效果不做**（原话"算了，这样就行"）。
      方案已给过、随时可加：`shownPath.assign(path.begin() + pathIndex, path.end())` —— 只画还没走的部分，
      看起来像"路线被一口一口吃掉"；想留拖尾就把起点改成 `pathIndex - 3`

**v4-a 记录搜索范围 + View 打包（2026-10-28 完成）**

- [x] `include/solver.h`：新增 `struct SearchResult { std::vector<Point> path; std::vector<Point> visitOrder; };`
      + `SearchResult bfsSearch(...)`；`bfsPath` 退化成一行 `return bfsSearch(...).path;`
      —— **旧调用点和 611 条断言一行都没动**，它们成了"重构没改坏行为"的保险丝
- [x] `visitOrder` 记的是**出队顺序**（BFS 用队列时入队==出队；但将来换 DFS，只有出队顺序才是真实探索顺序）；
      **走不通时也返回 `visitOrder`** —— "探了多少格才确认走不通"本身最有信息量
- [x] `include/render.h`：把 6 个参数打包成 `struct View { player / exit / steps / seconds / path / visited }`，
      签名变成 `render(const Maze &maze, const View &view)` —— **重构先行，再加功能**
- [x] `src/render.cpp`：新增 `isInVec()`；画格优先级 `@ > E > * > , > 原字符`；状态行加 `访问: N 格`（并回同一行）
- [x] 重构中踩到"残渣"bug：`std::reverse(path...)` 反的是遗留的局部变量而不是 `result.path` → 整条路径倒过来（见 M23）
- [x] 实测：611 断言全绿；默认帧只有 `#` `.` `@` `E`

**v4-b 搜索动画（波纹扩散）（2026-10-29 完成）**

- [x] 新状态：`std::vector<Point> visitOrder` / `int searchTick` / `bool searching`（**都在外层局循环里**）
- [x] 按 `E`：算出 `bfsSearch` 后**不直接显示**，而是 `searching = true; searchTick = 0;` 从头播
- [x] 每帧：`searchTick += 4`（约 4.7 秒播完 ~190 格，**这个数就是"手感"旋钮**）；
      `view.visited.assign(visitOrder.begin(), visitOrder.begin() + searchTick)` → 波纹逐帧长出来，
      `访问: N 格` 的数字跟着涨（白送一个动效）
- [x] 动画期间 `view.path` **保持空**（不剧透）；`!searching` 同时拦住"AI 走路"和"路径切片"两处
- [x] 播完 → `*` 路径出现 → AI 出发 → 走过的 `*` 被一口口吃掉（v3 的切片逻辑原样复用，只多一个 `!searching`）
- [x] 实测：零警告、611 断言全绿；用户实机确认"成功了"

### 待办（下次严格按此顺序）

- [ ] **★ 第一件事：提交推送 v4（a/b）的改动**（`include/solver.h`、`src/solver.cpp`、`include/render.h`、
      `src/render.cpp`、`src/main.cpp` + 本日志）
      `git add -A && git commit && git push`（**先开 Steam++**）
- [ ] **一号机待办（在 VS2022 那台做）**：preset `x64-debug` 的构建路线还没真跑过（M18 的 Ninja/`libucrt.lib` 问题）。
      验收 = 能编出来并运行。**换机时先 `git pull`，再选对 preset**（`x64-debug` ↔ `mingw-debug`，别互串）。
- [ ] **★ 然后开 v5：A\* + 带权地形 + 与 BFS 的"访问格数"对比**（全项目最值钱的数据）
      - `solver`：`aStarSearch(maze, from, to)` —— 优先队列按 **`g + h`** 排序：
        `g` = 从起点走到这里的步数，`h` = 到终点的**曼哈顿距离** `|dx| + |dy|`
      - ⚠️ `std::priority_queue` 默认是**最大堆**，要比最小值 → `std::greater<>` 或自定义比较器
      - **复用 `SearchResult`**（`path` + `visitOrder`）→ **只换搜索策略，`main` / `render` 一行都不用改**
      - **验收**：同一个迷宫、同一个种子，BFS `访问: 187 格` vs A\* `访问: 40 格` —— 数字摆出来
      - **单测**：**A\* 的路径长度必须等于 BFS 的路径长度**（启发式只影响搜索量，不影响最优性）；
        再加一条：A\* 的 `visitOrder` 长度 ≤ BFS 的（专抓"启发式写反变成更差"）
- [ ] **★ 之后（用户提的、很有意思）：AI 做成独立角色**（`A`），和 `@` 同场走、各算各的步数 → **人机竞速的雏形**
      （`View` 已经打包好了，加字段就是一行）
- [ ] **可选：AI 走过的路径逐帧清除**（用户看过方案，说"算了，这样就行"，先不做）：
      `shownPath.assign(path.begin() + pathIndex, path.end())`，只画还没走的部分；留拖尾就把起点设成 `pathIndex - 3`
- [ ] **可选（用户说"以后再说"）：给迷宫上色** —— `#` 深灰 / `.` 灰白 / 路径亮黄 / `@` 亮绿 / `E` 亮红。
      代码已验证过能编（要点：`SetConsoleTextAttribute` 改的是**设备状态**，而 `cout` 有缓冲，
      必须 `std::cout << std::unitbuf` 才能保证"改颜色 → 写字"的顺序不乱）
- [ ] **手感微调（可选，顺手就能做）**：`Sleep(100)` = 10 帧/秒，按键响应最多延迟 100ms，略钝；`Sleep(30)`（约 33 帧/秒）会更跟手
- [ ] **顺手清理（可选）**：`src/main.cpp` 里 `#include <time.h>` 没用到；`src/render.cpp` 里 `<time.h>` / `<conio.h>` 也没用到
- [ ] **补测（不着急，什么时候都行）**：v1-b 还剩两条不变量用例 ——
      **完美迷宫**（门数 == 房间数 − 1，专门抓"生成时开出环"这类**连通性测试抓不到**的 bug）、
      **承重柱永远是墙**（偶偶格必须始终是 `'#'`）
- [ ] **v1-b 的可选打磨**：`DX`/`DY` 目前在 `src/generator.cpp` 与 `tests/test_generator.cpp` 各有一份（重复常量），以后可挪进共享头文件
- [ ] v3 起照路线图推进（BFS 画路 → 搜索动画 → A\* 带权 → 人机竞速）
- [ ] 可选打磨（**不着急**）：`at()` 里 `x >= w || y >= h || !inBounds(x, y)` 的前两个条件是重复的（`inBounds` 已经在管），可简化成 `if (!inBounds(x, y)) return '#';` —— 好处是"越界规则只有一处"；`setAt` / `carve` 目前不做任何合法性检查（传错参数会静默写坏数据甚至越界崩，v1-a 就撞过两次），以后可加保护；`include/.gitkeep` 已可删（`include/` 有真文件了）

## 关键决定记录

- **项目定名 Maze**（用户拍板；交接文档里的候选 `Ariadne` 未采用）。目录本来就叫 `Maze`，因此不涉及重命名与 build 缓存失效问题（对比 Snack 踩坑 #27）。
- **远端用 HTTPS + `main` 分支**：22 端口被校园网墙死，SSH 不可用；`main` 与本机 `init.defaultBranch` 和 GitHub 默认分支一致，避免"推 `master` 但仓库首页显示空 `main`"这个坑。
- **沿用 Snack 的工程基座**：CMake（`add_executable` 模式，**不是** DSForge 那份 header-only `INTERFACE` 库写法）+ doctest + `-Wall -Wextra` + Debug/Release 分离 + `.vscode` 配置。
- **`.gitignore` 一开始就写 `build*/`**：Snack 用的是 `build/`，测性能的 `build-rel/` 会漏网。
- **`unit_tests` target 不含 `src/main.cpp`**：doctest 自带 main，混入会重复定义 main 导致链接失败（Snack 踩坑 #22）。
      **这带来一个额外红利：主程序半成品时，单测照样能全绿** —— 数据层与显示层天然解耦，改 main 不会动摇测试。
- **`include/` 与 `bench/` 放 `.gitkeep`**：空目录 git 不跟踪，留个占位文件免得换机 clone 后目录消失。（现在 `include/` 有真文件了，`.gitkeep` 可以删。）
- **`bench` target 先注释掉**：`bench/bench_main.cpp` 要到 v5（算法对照）才有，现在挂上去只会让构建找不到源文件。
- **新增源文件必须挂进 `CMakeLists.txt`，而且两个 target 都要挂** —— 这是 AI 的活，别让用户踩（本轮就漏了一次，见 M7）。
- **`Maze` 的成员直接公开、不写 getter**：它就是一堆数据，没有要保护的不变量；Snack 用 getter 是因为蛇身被外人乱改会破坏游戏规则。等以后真有不变量（比如"宽高必须是奇数"）再私有化。
- **⚠️ 目前 getter 与公开成员"两套并存"**（2026-10-10 用户主动加了 `getWidth` / `getHeight`）：功能上不冲突，但**两种读法会让人不知道该用哪个**（更荒诞的是：`render(const Maze&)` 里读公开成员 `grid[i]` 能过，被拦住的反而是非 const 的 getter）。
  暂时不动（v1-a 已收工，不折腾），等以后真有不变量要守时**一次性决定**：要么全公开、要么私有化 + 全 const getter。**但 `const` 必须现在补**（见"待办"）。
- **`at()` 越界一律返回 `'#'`**：把"界外"统一当成墙，后面写渲染、寻路时**调用方完全不用自己判边界**，能省掉一大把 `if (x < 0 || x >= w)`。
- **网格用 `vector<string>` 而不是 `vector<vector<char>>`**：可以直接 `std::cout << grid[y]` 整行打印、内存连续；先选省事的，有性能需求再换。
- **`git init` 前先报备**：本轮用户点头后才做；`--clean-first` 才能真验"零警告"（增量构建不会重编老文件）。
- **`generateDFS` 收一个 `seed` 参数，不在内部自己播种**：为了**可复现**（测试挂了能重放）和**可对比**（v5 基准要在同一张迷宫上比三种算法）。
  随机引擎在函数开头**建一次**、全程共用（`main` 用 `std::random_device` 每次给新种子）。
- **`Point` 做成"纯数据口袋"，不写构造函数**：它只是装两个 `int`，没有不变量要守，用 `{}` 初始化最省事。
  （用户一开始写了构造函数声明却没写定义 → 链接报 `undefined reference`，Snack 踩坑 #10 同款；后删除。）
- **渲染单独成模块 `render.cpp`**：一是让 `main` 只管主循环（Snack 拆 `UI.cpp` 的同款分层），二是 v2 的动画要靠"覆盖重绘"——
  有了独立模块，v2 只需给 `render` 加个玩家参数。
- **`render` 收 `const Maze&`**：画迷宫绝不改迷宫 —— 只读引用把这个承诺写进类型里，编译期强制。
- **"先确定性、后随机"的调试策略**：v1-b 先做**固定顺序**取邻居的版本（生成规则花纹，但逻辑一目了然），确认回溯跑对之后**才加随机**。
  这样"逻辑错"和"随机错"不会混在一起。
- **移动用 `switch` 直接给出 `dx` / `dy`，不再引一份 `DX/DY`**：输入处理本来就要 `switch` 键码，顺手把方向写出来最省事，
  也避免 `DX/DY` 出现第三份拷贝（`generator.cpp` 与 `test_generator.cpp` 各一份已经够多了）。
- **撞墙判定用 `maze.at(tx, ty) == '.'`**：又是那个"**越界当墙**"设计的红利 —— **不用自己写边界判断**，越界自然返回 `'#'`，走不过去。
  （注意移动的**步长是 1**，不是生成时的 ±2。）
- **每帧都重绘（哪怕这一帧没动）**：覆盖重绘很便宜，而且为 v4 的"搜索波纹动画"铺好了路 —— 到时候画面内容逐帧变化，
  **主循环一行都不用改**。
- **玩家位置是 `main` 的局部变量，不进 `Maze`**：`Maze` 只管"迷宫长什么样"，玩家/步数/计时都是**游戏状态**，
  归主循环持有（Snack 里"分数、食物在 main，UI 只读画"的同一条原则）。
- **计时用 `steady_clock` + 每帧重算**：`startTime` 一局取一次、`elapsed` 每帧重算（理由见"知识点地图 · 计时"）。
  变量名用 `elapsed` 而不是 `seconds`，是为了避开 `std::chrono::seconds` 的类型名。
- **结算文字写到固定行**：用 `SetConsoleCursorPosition` 定位到迷宫下方第 `h+1` 行，而不是 `cout << "\n..."` ——
  否则每重开一局就往下漂两行，越玩越低。
- **状态行也由 `render` 画**（设计 A）：所有"画"集中在一处，光标管理只在一个地方；代价是 `render` 参数会越加越多
  （v3 要加"AI 访问节点数"），**到那时打包成结构体**，现在先不绕路。
- **`bfsPath` 收 `const Maze&`、返回 `std::vector<Point>`**：寻路只读迷宫；"走不通"用**空 vector** 表示，不引入异常
  （与 Snack"食物不可达就维持方向"同一条思路：能返回就返回，别让调用方陷入异常处理）。
- **`path` 在 `main` 的循环外算一次**：迷宫是静态的，每帧重算纯属浪费，也顺手把 `bfsPath` 的调用点挪出了帧循环。
- **路径"不整洁"要用颜色解决，而不是换字符**（用户 2026-10-22 决定**延后**）：`*` 铺满整条路会把迷宫花纹盖住；
  换成更轻的 ASCII 字符只是"没那么丑"，**颜色高亮才是真干净**。已写好并验证能编的版本记在"待办"里。
- **`tests/test_solver.cpp` 由 AI 代写**（用户明确要求"我有点写不动了"）：这是**协作约定的例外**，
  记在这里以免以后误以为"测试也归 AI 写"。**其余业务代码仍一律用户手写。**
- **AI 模式驱动的是同一个 `player`**（2026-10-26 用户第一反应是"AI 把玩家接管了"，并觉得挺有意思）：
  这是刻意的简化 —— 一个角色、一个步数，AI 走完直接给出**"最短步数"**，方便和你自己的成绩比。
  **独立 AI 角色**（两个角色同场、双步数、真正人机竞速）已排进待办，属于 v6。
- **`View` 只装"这一帧要画的东西"**：`render` 完全不知道 `aiMode`、"走到第几格"这类游戏状态 —— 那些留在 `main`。
  **`path`（源头：完整路径）与 `view.path`（派生：这一帧的切片）是"源 + 派生"，不是两份真相**；
  真正的"两份真相"是同时存在 `showPath` 和 `view.path`（v4-a 重构时就出现过，症状是"算了却没人画"）。
- **`visitOrder` 用出队顺序记**：BFS 队列下入队==出队，但换 DFS（栈）后两者完全不同；现在就按"出队时记"，以后换算法不用改。
- **搜索动画的"速度"用帧步长（`searchTick += 4`）调**：这是**手感旋钮**，不是性能参数。
  因为 `visitOrder` 本身就是波纹顺序，所以"每帧多显示 N 个"就自动是由内向外扩散 —— **不需要算半径、不需要排序**。

## 踩坑库

- **通用踩坑库（34 条，教科书级）**：见 `D:\Tool\VSCode\Memory\Snack\PROJECT_NOTES.md` 的"踩坑库"章节
  （`enum class` 不能从 int 构造、函数声明与定义必须逐字一致、CMake 未定义变量静默展开为空、
  目录改名后 build 缓存失效、`-Wall -Wextra` 的价值、PowerShell 的 UTF-8 编码坑……）。
  **撞上类似问题时直接引用编号**，不重复抄写。

### Maze 新增（M1~M23）

1. **`vector<string>` 是"容器套容器"，两层都得先分配。**
   `grid.resize(h)` 只让柜子有 h 层，**每层还是空 string**；每行还要再填 w 个字符。只做一半 → 行内索引越界。
2. **构造表达式当语句写 = 造出来当场扔掉。**
   `std::string(w, '#');` 完全合法、编译器一声不吭，但那个字符串没人接、立刻销毁（等价于 `3 + 4;`）。
   要写 `grid[i] = std::string(w, '#');`。（Snack 踩坑 #28"有返回值的调用结果被丢弃"的同款。）
3. **`push_back(int)` 会被隐式转成 `char`，编译器不警告。**
   `grid[i].push_back(w)` 里 `w = 7` → `char(7)` = **响铃符 `\a`**（不是字符 `'7'`）。
   `int → char` 是标准转换，`-Wall -Wextra` 不管（要 `-Wconversion` 才喊）。
   **这类错看编译日志永远抓不到，只能看数据** —— 本轮靠探针打印才发现"每行长度 1、字符码 7"。
   区分工具：`push_back(c)` = **追加一个**；`resize(n, c)` / `assign(n, c)` / `string(n, c)` = **一次变成 n 个**。
4. **二维索引是"先行后列"：`grid[y][x]`，不是 `grid[x][y]`。**
   在方阵上写错照样跑得通，**一换成长方形就炸**。写法习惯：**外层循环走 y（行），内层循环走 x（列）**，这样 `grid[y][x]` 永远顺眼。
5. **doctest 的 `CHECK` 里不许写 `&&` / `||`。**
   报 `static assertion failed: Expression Too Complex Please Rewrite As Binary Comparison!` ——
   它要自己拆等号两边才能打印"左是几、右是几"，条件一复杂就罢工。**一条拆成两条** `CHECK(a == 6); CHECK(b == 4);`
6. **负数传给 `string::operator[]` 会变成巨大的无符号数 → 越界崩。**
   `at(-1, 0)` 里 `-1 >= w` 是假，于是掉进正常分支去读 `grid[0][-1]`，实际访问的是天文数字下标。
   **负坐标必须在越界判断里显式拦掉**（`inBounds` 里那两句 `x < 0 || y < 0` 就是干这个的）。
7. **新增源文件忘了列进 `CMakeLists.txt` → `undefined reference`。**
   症状：编译全过、链接报 `undefined reference to 'Maze::Maze(int, int)'`。文件在、头文件在，就是没编进来。
   （Snack 踩坑 #26 同款；本轮是 AI 漏挂 `src/maze.cpp`，而且 `Maze` 和 `unit_tests` **两个 target 都得挂**。）
8. **`g++ -fsyntax-only` 抓不到 `-Wreturn-type`。**
   同一份代码，`-fsyntax-only` 零输出，真实构建报 `warning: control reaches end of non-void function`。
   它做完语法分析就停手、不做控制流检查。**所以这个命令只能当"打字检查"，不能当验收** ——
   验收永远用 `cmake --build build`。（本轮 AI 给错了自查命令，害用户以为过了。）
9. **断言式的越界检查是"运气好"**：本机 MinGW 的 libstdc++ 默认开着越界断言，
   把 `vector` / `string` 的越界下标变成了明确的 `Assertion '__n < this->size()' failed` 崩溃。
   否则这类错会变成"偷偷改坏内存、程序随机发疯"，难查十倍。**崩溃信息里的行号就是贼窝。**
10. **长方形迷宫里 x 和 y 的合法范围不一样，别把宽度的范围套到高度上。**
    21×11 的迷宫中，**x 能到 19，y 只能到 9**（`x ∈ {1,3,…,19}` 共 10 个，`y ∈ {1,3,5,7,9}` 共 5 个，房间数 10×5=50）。
    本轮 `carve(17, 19, 19, 19)` 用了 `y=19`，算出的门 `(18,19)` → 直接 `vector` 越界崩。
    这和 M4（索引顺序）是**同一类陷阱的亲戚：方阵上怎么写都不会错，一换成长方形就露馅**。
    **写死坐标前先默念一遍"这张迷宫多宽多高"。**
11. **把"决策"写进 `for` 循环里 = 一次迭代做了 N 次决策。**
    收集候选的 `for` 里不该有 `if/else`。写进去后：第一圈（`d = 0`）候选**必然还是空的** → 直接走 `else`
    把栈里仅有的起点**误弹掉**；而且 `while (!stack.empty())` 只在**整个 `for` 跑完**之后才检查一次，
    `for` 转到一半栈空了没人管 → 空栈 `pop_back()` → `Assertion '!this->empty()' failed`。
    **口诀：`for` 是"查账"，`if/else` 是"决策" —— 查完账才能决策，不能查一条决定一次。**
12. **随机数引擎建在循环里 = 每轮重新播种 = 每次都拿到同一个数。**
    `std::mt19937 rng(seed);` 写在 `while` 里 → 每轮都新建、都用同一个种子重新上发条 → `rng()` 永远返回"序列的第一个数"。
    实测（seed = 42）：循环里播种 5 次得到 `1608637542` × 5；只播一次则 5 个完全不同的数。
    **比喻**：`seed` 决定"从书的哪一页开始读"，`rng()` 是"往下读一句" —— 你的写法等于**每读一句就合上书翻回第一页**。
    症状很典型：随机"退化成了查表"（结果只由 `candidates.size()` 决定），迷宫结构基本不变，用户原话是"变化只局限于开头"。
13. **变量从循环里搬到循环外，就失去了"每轮自动重置"这个免费保障 —— 重置成了你自己的责任。**
    `candidates` 被搬到 `while` 外面却忘了 `clear()` → 旧候选永远留在名单里、**只涨不跌** →
    `!candidates.empty()` 永远为真 → `else`（`pop_back()`）永远不执行 → **栈只增不减 → 死循环**。
    副产品知识：`clear()` **只清元素、不释放内存**（`capacity()` 保留），所以"放外面 + 每轮 `clear()`"比"放里面"少些反复分配。
14. **一串 `&&` 里条件的"极性"全写反 = 条件恒假，症状极好认。**
    洪水填充返回 `1`（只淹了起点自己）→ 反推："四个邻居一个都没进队" → 守卫条件**恒为假**。
    根因：把**生成器**那句 `maze.at(nx, ny) == '#'`（找"还没挖的房间"）搬进了**洪水填充**（要沿着 `'.'` 通路走）——
    字面搬对了、语义搬反了。**同一张迷宫，两个阶段看的是相反的东西。**
    附带：`!maze.inBounds(...)` 那个 `!` 来自另一种写法 —— **排除法**（`if (!x) continue;`）。
    **排除法的条件不能搬进筛选法的 `&&` 里**，意思会整个反。推荐用"排除法三连" `if (...) continue;` 写这段，每条件独立、不易反。
15. **`bool seen[h][w]`（尺寸运行时才知道）= 变长数组（VLA），不是标准 C++。**
    `-pedantic` 报 `ISO C++ forbids variable length array 'seen' [-Wvla]`。
    GCC 当"扩展"放行（所以本机看不出问题），**MSVC 不支持** → 换到主力机 VS2022 **直接编译失败**；而且 VLA 在栈上分配，尺寸大了爆栈。
    标准写法：`std::vector<std::vector<bool>> seen(h, std::vector<bool>(w, false));`
    ⚠️ **装 MinGW 的那台机器（主力机）看不见这类"只在 MSVC 上炸"的坑，双机协作时尤其危险。**
    （**M18 正好是它的镜像：VS2022 这台也一样看不见"只在 MinGW 上能过"的东西 —— 两边的坑，只能靠对方那台机器暴露。**）
16. **"没输出 + 不退出" = 死循环的签名，跟"崩溃"是两种病，别混。**
    - **崩溃**（越界 / 断言失败）：会甩出明确的报错信息 + 退出码是负数
    - **死循环**：**零输出、进程一直不退出**（本轮 `generateDFS` 就是这种）
    - （第三种）卡在 `std::cin.get()` 等输入：**但那时输出早就打印出来了**，容易区分
    查出方法：给命令加**看门狗**（`Start-Process` + `WaitForExit(毫秒)`），超时就判定死循环、直接掐掉。
17. **`_getch()` 读的是"控制台"，不是 `stdin` —— 管道喂不进按键。**
    `echo q | Maze.exe` 这种写法**完全无效**：`_getch()` 直接从控制台输入缓冲区取键，**绕过了标准输入重定向**。
    后果：**带主循环的主程序没法用脚本驱动**（没法自动按键走迷宫）—— 无头环境下只能验证"它在跑 + 画面对不对"。
    AI 的验证办法：`Start-Process -RedirectStandardOutput 文件 -PassThru -NoNewWindow` + `Start-Sleep` + `Kill`，
    再把输出文件按"**每 11 行一帧**"切开检查（本项目实测 1.5 秒 → 14 帧、每帧恰好一个 `@`）。
    （顺带：stdout 被重定向成文件后 `SetConsoleCursorPosition` 会失败 —— 无害，只是退化成"追加输出"，正好方便数帧。）

18. **VS2022 这台机器：CMake 用 `Ninja` 生成器 → 必然倒在 `LNK1104: 无法打开文件"libucrt.lib"`（2026-10-11 实测）。**
    症状极有欺骗性：**编译一路全过，只在链接那一刻炸**，报的是"找不到一个 C 运行库文件"，看着像代码或环境烂了。
    真相是**生成器路线问题** —— 这台机器的 Windows SDK **不在默认位置**（注册表 `KitsRoot10 = E:\Windows Kits\10`），
    `libucrt.lib` 实际在 `E:\Windows Kits\10\Lib\10.0.26100.0\ucrt\x64\`（**文件是好的、齐全的**）。
    - **MSBuild（`.vcxproj` / "Visual Studio 17 2022" 生成器）能找到它**：VC 工具集去读注册表 `KitsRoot10`，自己拼 Lib 路径。
      证据：`Snack\Snack\Snack\x64\Debug\Snack.log` 写着 `Snack.vcxproj -> ...\Snack.exe`（真的编出来过）。
    - **Ninja（VS"打开文件夹"的默认）找不到**：它靠 `LIB` 环境变量，而**不加载开发者环境时 `LIB` / `WindowsSdkDir` / `VCINSTALLDIR` 全是空的**。
      证据：Maze 与 Snack 的 `CMakeFiles\CMakeConfigureLog.yaml` 里**同一个错各出现好几遍**（Maze 在 `CMakeDetermineCompilerABI` 处反复重试）。
    **正确做法**：用 **`Visual Studio 17 2022` 生成器**（本仓库已加 `CMakePresets.json` preset `x64-debug`），
    或改用 **"x64 Native Tools Command Prompt for VS 2022"**（`vcvars64.bat` 会把 SDK 的 Lib 路径填进 `LIB`）。
    **别去装 SDK、别改系统的 Windows Kits 位置** —— SDK 本来就是好的。
    附带教训：一条路线失败时，**先去日志和隔壁能跑的项目里找"哪条路线成功过"**，比盯着报错猜有效得多。
19. **DSH 升级到 0.2.1-rc.2 后，沙箱进程是"低完整性"，写不进没有低完整性标签的目录（2026-10-20 实测，踩得很惨）。**
    症状：`cmake --build build` 报 `CMake Error: Cannot open file for write ... Permission denied`，
    但**用户自己在 VS Code / cmd 里构建完全正常** → 说明不是项目坏了、也不是代码问题。
    判断：`whoami /groups` 里出现 `Mandatory Label\Low Mandatory Level`；再对比"能写的目录"和"不能写的目录"的标签。
    根因：新版沙箱把命令进程降为 Low 完整性，靠**给工作区根打"低完整性"标签 + 向下继承**来开放写入；
    而它有一条优化"根上权限项已存在就跳过重新传播" → **升级前就在的老目录漏掉了标签**（实测 `Memory` 有、`Maze` 没有）。
    ⚠️ **这跟 DACL 完全无关** —— 补"完全控制"权限一点用没有（我第一次就是这样误判，白折腾了一轮）。
    修法（用户在普通权限终端跑两条）：
    `icacls "<工作区>\Maze" /grant:r "%USERNAME%:(OI)(CI)F"`
    → `icacls "<工作区>\Maze" /setintegritylevel (OI)(CI)L /T`
    （第一条必须把完全控制设成**可继承**，否则子目录只有从"已认证用户"继承来的"修改"权限、**没有改标签的资格**，
    第二条 `/T` 会在第一个子目录（本项目是 `.git`）就断掉，然后整棵树都没打上。）
    **新文件不用再手动加** —— 标签可继承，实测新建文件后能继续写。只有**换一个全新工作区根目录**才可能再遇到。
20. **`duration_cast<seconds>` 里的 `seconds` 是类型名 —— 变量千万别也叫 `seconds`。**
    写成 `int seconds = duration_cast<seconds>(...)` → 编译器分不清"类型"和"变量"，满屏报错（本项目实际踩到）。
    三条铁律：**变量改名**（用 `elapsed`）+ **全写 `std::chrono::`** + **最后 `.count()` 才是数字**。
    另外 `std::chrono::seconds` 的**对象不能当 `int` 传参**，要 `static_cast<int>(...)`；
    `startTime` 也不能跨文件用（`render.cpp` 里根本没有它）。
    更根本的一条：**计时那句必须在循环里**，放外面只算一次 → 永远 0。
    （实测对比：错版本 5 帧全 `0 秒`；对版本 `0/1/2/2/3` 在涨。）
21. **手写 `operator=` 写错 → 赋值静默失效，比编译错误还难查（2026-10-22，本项目真踩到）。**
    用户写了 `Point operator=(const Point &other) const { return other; }`：`const` + 返回 `Point`（不是引用）+ **根本没改 this**。
    结果：`p = q;` 能编过、不报警、**`p` 一点没变**（实测 `p` 仍是 `(1,1)`）。
    后果极隐蔽：`main` 里的 `player = {tx, ty};` 不生效 → **玩家根本走不动**，而编译器一声不吭。
    铁律：**只含普通成员（`int`/`double`/`std::string` 等）的结构体，永远不要手写 `operator=`**，编译器生成的就对。
    只有"自己管资源"（裸指针、需要深拷贝）时才手写，那时也必须 **非 `const`、返回 `Point&`、并真的改 `*this`**。
    （GCC 会给一条 `-Wdeprecated-copy` 的间接线索，但它不直说"你写错了"，很容易被忽略。）
22. **`&&` 优先级比 `||` 高 —— 混着写会静默改变语义。**
    `if (dx != 0 || dy != 0 && !aiMode)` 的真实含义是 `dx != 0 || (dy != 0 && !aiMode)`，
    **完全不是**"两个都不是 0 且不在 AI 模式"。正确写法：`if (!aiMode && (dx != 0 || dy != 0))`。
    **看到 `||` 和 `&&` 混在一句里，无条件给 `||` 那一组加括号。** 编译器不报，只有怪现象才暴露。
23. **重构"改了主体没改全" —— 最隐蔽的一类 bug（2026-10-28 真踩到）。**
    把 `bfsPath` 重构成 `bfsSearch` 后，"主体"从 `path` 变成了 `result.path`，
    但 `std::reverse(path.begin(), path.end());` 那句还反着**残留的局部 `std::vector<Point> path;`**（空的）。
    结果 `result.path` 一直是**倒的**（终点排到起点）。为什么难查：
    - **编译器不报警**：`path.begin()` 是合法用法，那个残留变量也"被用了"（begin/end），不算未使用
    - **症状有特征**：`path.front() == from` 和 `path.back() == to` **同时失败且正好互换** → 一眼判定"整体反了"
    **教训**：重构时把"旧主体"的声明**删干净**，别留；重构后**立刻跑测试**（611 条断言就是保险丝）。
    附带一条 AI 自己的教训：排查时用 `shownPath` 去搜用户的 `showPath`，**差一个字母**就误判成"功能没做"，白绕一圈 ——
    找代码时最好搜**行为特征**（比如 `begin() +`），别只搜变量名。
24. **`std::priority_queue` 是最大堆**（v5 写 A\* 前先记住）：默认 `top()` 给**最大**的那个元素。
    要比最小值必须 `std::priority_queue<T, std::vector<T>, std::greater<T>>`（或自定义比较器）。
    这个"默认方向反了"跟"索引先行后列"一样，是最容易静默错的地方。

## 知识点地图

**迷宫表示法（本轮新增）**

- **奇偶格表示法**：让"墙"自己占一个格子。奇数行 + 奇数列 = **房间**（能走的格）；偶偶 = **永久承重柱**（生成算法永不碰）；一奇一偶 = **门的位置**（打通就是 `.`）。
  代价是"缩水"（21×11 的画布只有 10×5 = 50 个房间），换来的是**屏幕上看到的就是内存里存的**，渲染不用翻译。
  因为房间只能落在奇数坐标，**宽高必须是奇数**。
- **"取中点找门"**：相邻房间一个坐标相同、另一个差 2，正中间那格就是门。两个奇数相加是偶数 → `(x1+x2)/2` 一定整除，不会出现小数坐标。
- **长方形迷宫的坐标系是不对称的**（`Maze(w, h)` 里 w 是列数、h 是行数）：21×11 时
  `x（列）∈ {1,3,…,19}` 共 10 个，`y（行）∈ {1,3,5,7,9}` 共 5 个 → 房间共 **50** 个。
  **上限由不同的变量决定**（`x < w`、`y < h`），这是本项目最容易咬人的地方。

**C++ / STL（本轮新增）**

- **容器套容器**：`vector<string>` 外层管"几行"、内层管"每行几个字符"，两层各自独立分配。
- **填充构造 / 填充变长**：`string(n, c)` 造一个 n 个 c 的串；`resize(n, c)` / `assign(n, c)` 把已有容器变成 n 个 c。
  对比 `push_back(c)` **只加一个**（`vector` 与 `string` 都有 `resize(n, 值)` 这版）。
- **表达式 vs 语句**：有"结果"的表达式（构造、函数调用）必须被接住（赋值/初始化/传参），当语句裸写就白算了。
- **隐式数值转换是静默的**：`int → char` 属标准转换，`-Wall -Wextra` 不管；`-Wconversion` 才喊。
- **无符号下标的陷阱**：`operator[]` 收 `size_type`，负数会绕成巨大正数。
- **函数每条出口都要 `return`**：漏一条 → `-Wreturn-type` 警告 + 未定义行为（返回垃圾值）。
- **`inBounds` 复用**：越界规则写在**一个地方**，调用点别重复判断 —— 以后改规则（如环形迷宫）才不会漏改。

**设计 / 测试（本轮新增）**

- **"越界当墙"的接口设计**：`at()` 越界返回 `'#'`，把边界处理收进类内部，调用方彻底不用想边界。
- **数据层与显示层解耦**：`unit_tests` 只编被测源文件（不含 `main.cpp`），所以主程序还是半成品时单测照样全绿 —— 改显示层不会动摇测试。
- **诊断探针**：编译器和测试都沉默时，写个十行小 `main` 把"实际造出来的数据"打印出来（本轮靠它抓到 `push_back` 塞进响铃符）。
  重建命令见"常用命令备忘"。
- **测试用例的写法**：一个 `CHECK` 一个条件；判奇偶用 `% 2`；循环上界用 `m.w` / `m.h` 别写死数字。

**算法（v1-b 新增）**

- **DFS 回溯生成迷宫**：从起点房间开始，一路往"没去过"的邻居钻、打通中间的墙；走不动了就退回上一个岔路口（靠栈记住来路）。
  直觉："**蚯蚓钻土**"。
- **它生成的必然是"生成树"**：50 个房间恰好 **49 道门** —— **一定连通、一定无环**。这是算法本身保证的，不需要额外去环。
  （判据：门数 == 房间数 − 1。这是比"连通"更强的不变量。）
- **状态长在数据里**：不需要额外的 `visited` 表 —— 房间还是 `'#'` 就代表"没挖过"，挖开就是 `'.'`。省一张表。
- **两套"步长"千万别搞混**：**生成时**房间到房间是 **±2**（中间隔着一格墙）；**洪水填充 / 寻路**沿着通路走是 **±1**（房间和门都是通路格）。
  理解方式：生成是"**跳着走**"，洪水是"**挨着走**"。
- **栈**：`std::vector` 就是栈 —— `push_back` 入栈、`back()` **看**栈顶（不删）、`pop_back()` 出栈。
  **"看"和"弹"是两件事**：先看栈顶还有没有路，确实没路了才弹。
- **洪水填充 = 早就在用的 BFS**（Snack 里的 BFS 换个用途）：从起点往外漫水，用 `seen` 表避免重复。

**随机数（v1-b 新增）**

- **`std::mt19937 rng(seed)` 是一台"机器实例"**，不是"产生一个数"。`seed` 决定从序列的哪一页开始，`rng()` 每调一次往前走一格。
  **建一次、反复用**；建在循环里 = 每轮重新上发条（见 M12）。
- **`std::uniform_int_distribution<int> pick(0, n-1)` 是"转换器"**：把引擎吐的 32 位大数翻译成 `0..n-1` 的**均匀**整数（**闭区间**，两边都含）。
  它自己不携带随机性，所以放循环里外都行；**只有引擎必须放外面**。
- **为什么不用 `rand()`**：① `rand() % n` 有**取模偏差**；② 状态是**全局**的（`srand` 污染全程序、测试难独立）；③ 算法**由库决定** —— **换编译器/平台结果不同**。
  `mt19937` 是**标准规定**的算法 → **同一个 seed 在任何平台生成同一张迷宫**，这是 v5 基准测试的前提。
- **把种子做成参数**：**可复现**（测试挂了记下 seed 就能原样重放）+ **可对比**（v5 要在同一张迷宫上比三种寻路算法）。
  `main` 里用 `std::random_device` 每次换新种子。

**Windows 控制台（v1-c 新增）**

- **覆盖重绘三件套**：`GetStdHandle(STD_OUTPUT_HANDLE)`（拿"标准输出"的**句柄**，句柄像"取件码"）+ `COORD{0, 0}`（光标坐标，x 列 y 行，**跟迷宫坐标同一套约定**）+ `SetConsoleCursorPosition(句柄, 坐标)`（把光标挪回去）。
  之后输出的字就**盖在原来的字上** → 不闪屏、不滚动。**这是 v2 动画的地基。**
- **为什么不用 `system("cls")`**：它要**启动一个额外的命令行进程**去清屏（慢），而且整屏**先擦空再重画** → **闪**。覆盖重绘是直接在原地改写。

**设计 / 测试（v1-c 新增）**

- **`const` 成员函数的利息**：`render(const Maze&)` 里只能调 `const` 成员函数 —— v1-a 给 `getWidth()/getHeight()` 补的 `const`，到这里才显出必要性（没补就编不过）。
- **模块分层**：`maze`（数据）/ `generator`（算法）/ `render`（显示）/ `main`（主循环），各自只干一件事。
  Snack 的同一招：拆出 `UI.cpp` 的当天，`Snack` / `Point` / 单测**一个都不用改**。
- **`static` 函数 = 内部链接**：名字只在本 `.cpp` 里可见。多个测试文件编进同一个可执行文件时，**辅助函数必须加 `static`**，
  否则以后 `test_solver.cpp` 里再写个同名的 `reachableRooms` 就会**重复定义**。
  （文件作用域的 `const` 变量本来就有内部链接，**只有函数需要显式 `static`**。）
- **`INFO("seed = ", seed)`**：用例里套循环时，失败信息会**带上当前 seed**，一眼定位是哪一轮挂的；成功时完全静默不刷屏。
- **避免"断言数量爆炸"**：50 个位置 × 10 个种子 = 500 条断言会刷屏、真出错也找不着。
  改成"**数违规个数，`CHECK(违规数 == 0)`**"。
- **诊断探针**：编译器和测试都沉默（或症状古怪）时，写个十行小 `main` 把"实际数据"打印出来。
  本项目靠它抓到过 `push_back` 塞进响铃符（M3）、`vector` 下标越界（M9）、`mt19937` 重复播种（M12）。

**游戏循环 / 输入（v2-a 新增）**

- **游戏主循环**：`while (true) { 输入 → 更新 → 渲染 → Sleep }`。每转一圈叫**一帧**，所有实时游戏都是这个骨架。
- **`Sleep` 不能省**：没有它，`while` 会全速空转、把一个 CPU 核跑到 **100%**。帧率 ≈ `1000 / Sleep毫秒`。
- **帧率就是操作延迟的上限**：`Sleep(100)` → 10 帧/秒 → 按键响应最多延迟 **100ms**（略钝）；`Sleep(30)` → 约 33 帧/秒，更跟手。
- **`_kbhit()` 不阻塞地问"有键吗"**，**`_getch()` 不等回车**地取一个键 —— 这俩才是实时游戏的输入方式。
  对比 `cin >>`：它会**卡住等回车**，游戏直接"停"在那儿。
- **键盘队列要每帧掏空**：按住某键时系统会**往队列里塞一串**；每帧只取一个的话队列越积越多 → **松手后角色还会自己走一段**。
  所以 `do { ... } while (_kbhit())`，**每帧只认最后一次按键**。
- **方向键是两字节**：`_getch()` 先返回 `0` 或 `224`（前缀），**必须再调一次**才拿到真正的扫描码：↑72 ←75 →77 ↓80。
- **控制台坐标系**：y 是行、**向下增大** → "上" 是 `dy = -1`（和数学坐标反着）。
- **`render` 的 `const Maze&` 成了护栏**：它保证玩家**绝不会被"写进"迷宫** —— 一旦 `grid` 被污染，v3 的寻路 AI 会读到脏数据。
- **"帧计数"控制节奏，"真实时钟"控制计时，两者别混**：AI 每 3 帧走一格（`aiTick`）要的是**节奏感**，用帧数最简单；
  而 `elapsed` 必须问 `steady_clock`。同一份代码里两种时间概念各管各的，混用就会出"AI 越走越快"这类怪事。
- **开关型状态放外层循环**：`aiMode` / `path` / `pathIndex` / `aiTick` 都声明在"一局"的花括号里，
  按 R 重开自动归位 —— 不用写任何重置代码，也就不会漏。
- **参数膨胀的处理**：函数参数到 6 个就该打包成结构体（本项目就是 `View`）。好处：调用点自解释、
  加字段只改一处、结构体本身就是"这一帧长什么样"的完整描述。

**计时（v2-b 新增）**

- **四件套**：`now()` 取**时间点** → 两个时间点**相减**得**时间段** → `duration_cast<单位>` **换单位** → `.count()` **取数字**。
  ⚠️ `now()` 返回的不是数字，别想直接打印或相加。
- **必须用 `steady_clock`，不能用 `system_clock`**：后者是"墙上时钟"，用户改时间或系统自动对时会**跳**，可能算出负数。
  `steady_clock` 是**单调**的，只往前走。（v5 测算法耗时也用它。）
- **"开始时刻"只取一次，"已用时间"每帧重算** —— 时钟不会主动把新值写进变量，**每次都得去看表**。
  放循环外面只算一次 → 永远 0（实测：5 帧全是 `0 秒`）。
- **变量名躲开类型名**：别叫 `seconds`（跟 `std::chrono::seconds` 撞名），用 `elapsed`。
- **帧号不是时间**：不能靠"每画一帧 +1 秒"来计时 —— 那跟真实时间毫无关系（10 帧/秒时 10 秒就跳 10 次）。

**寻路（v3-a 新增）**

- **BFS 一层层往外扩散**：每圈代表"再走 N 步"，所以**第一次碰到终点时的路径必然最短**（无权图）。
- **`visited` + `parent` 两张表**：`visited` 管"别再重复排队"，`parent` 管"我是从哪来的"；到终点后顺 `parent` 倒回起点就是路径。
- **`visited` 要在入队时就标记**，不是出队时 —— 否则同一格会被多个邻居重复入队，队列爆炸。
- **回溯终止条件**：`while (!(cur == from))`，一路倒回起点为止。（Snack 踩坑 #32 的老坑：写 `while (!(parent[cur] == from))` 只会取到第一步，且起点相邻时越界。）
- **交叉验证**：`bfsPath` 说"最短"，就用**另一套独立实现**（洪水填充算距离表）去核对 —— 两个实现互相校准，才能确认"最短"不是自我安慰。
- **步长两套别混**：生成 → **±2**（房间之间隔着一格墙）；**寻路 / 洪水填充 → ±1**（在通路格之间走）。

## 本机环境事实（VS2022 这台，2026-10-11 实测）

> **两台机器完全不同**：下面这些只对"VS2022 这台"成立。装 MinGW 的主力机按改名前的老命令走（那一套在本机**全部是死路**）。

| 项 | 本机实测值 |
| --- | --- |
| 工作区 | `E:\Tool\Visual Studio\Memory`（**不是** `D:\Tool\VSCode\Memory`） |
| `g++` / `mingw64` | **不存在**（`D:\Tool\VSCode` 没有这个目录，`g++` 也不在 PATH） |
| `cmake`（独立安装） | **不存在**（`D:\Tool\CMake` 不存在） |
| 实际用的工具链 | VS2022 Community：`E:\Tool\Visual Studio\Microsoft Visual Studio 2022`；MSVC `14.44.35207`（cl 19.44） |
| CMake（VS 自带） | `...\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`（**3.31.6**，不是文档里写的 4.4.3） |
| Ninja（VS 自带） | `...\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe` |
| Windows SDK | **非默认位置**：`E:\Windows Kits\10`（`Include` / `Lib` 都是 `10.0.26100.0`）；注册表 `KitsRoot10 = E:\Windows Kits\10\` |
| 能跑的构建路线 | ✅ `Visual Studio 17 2022` 生成器（= MSBuild）；✅ `vcvars64.bat` + Ninja |
| 跑不了的路线 | ❌ **裸 Ninja**（VS"打开文件夹"的默认）→ 见 M18；❌ MinGW Makefiles（没有 g++） |

**本机命令（在 Maze 目录下）**

```powershell
# 路线一：preset（推荐，和 VS 里选 preset 等价）
cmake --preset x64-debug
cmake --build --preset x64-debug

# 路线二：VS 的"开发者环境"里跑 Ninja（vcvars64 会把 SDK 的 Lib 路径填进 LIB）
cmd /c "call `"E:\Tool\Visual Studio\Microsoft Visual Studio 2022\VC\Auxiliary\Build\vcvars64.bat`" && cmake --preset x64-debug"
```

- ⚠️ **VS 生成器是多配置**：产物在 `build\x64-debug\Debug\`，**不是** `build\x64-debug\`；命令行构建要 `--config Debug`
  （preset 里可以写 `"configuration": "Debug"` 免掉这一步）。
- ⚠️ **`.vscode\` 那套 F5 在本机用不了**：它写死了 `D:/Tool/CMake/...` 和 `D:/Tool/VSCode/mingw64\...`（本机不存在）。
  本机用 **VS2022** 打开文件夹 + 选 preset，别指望 VSCode 那条链。要不要给 `.vscode` 做双机分支，以后单独决定。
- **（2026-10-11 的环境修复，本机一次性）**：DSH 的 Windows 沙箱要给工作区改 DACL，前提是**运行它的账户就是工作区目录属主**；
  本机跑 DSH 的是 `present\learn`、而 `Memory` 属主是 `yanluxiaocen`，且 `learn` 的管理员组被 UAC 过滤 →
  每条命令都报 `SetNamedSecurityInfoW failed (Win32 5)`。**已用管理员 PowerShell 执行一次**：
  `icacls "E:\Tool\Visual Studio\Memory" /grant "present\learn:(WDAC,WO)"` → 之后免提权可用。
  **换工作区路径时要重做这一条**（它只作用于被授权的那个目录）。

## 常用命令备忘

- 每日开工：先读本文件 + `docs/MAZE_PROJECT_HANDOFF.md`；换机先 `git pull`
- 收工：更新本文件 → `git add -A` → `git commit -m "..."` → `git push`（**先开 Steam++**）
- 首次配置（新机器 / 删了 build 目录）：
  `cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=D:/Tool/VSCode/mingw64/bin/g++.exe -DCMAKE_MAKE_PROGRAM=D:/Tool/VSCode/mingw64/bin/mingw32-make.exe`
- 日常构建：`cmake --build build`
- **验零警告**（增量构建不会重编老文件，必须 `--clean-first`）：`cmake --build build --clean-first`
- 跑单测（详细）：`.\build\unit_tests.exe`（`--success` 看通过项 / `-tc="名字"` 过滤 / `--list-test-cases` 列用例）
- 跑单测（批量）：`ctest --test-dir build --output-on-failure`
- 跑主程序：`.\build\Maze.exe`
- **验证主程序在不在跑**（它有主循环、不会自己退出，而且 `_getch()` 喂不进按键，见 M17）：
  ```powershell
  $out = "build\run.txt"; Remove-Item $out -EA SilentlyContinue
  $p = Start-Process -FilePath "build\Maze.exe" -RedirectStandardOutput $out -PassThru -NoNewWindow
  Start-Sleep -Milliseconds 1500
  if (-not $p.HasExited) { $p.Kill(); $p.WaitForExit() }   # 还活着 = 主循环正常
  (Get-Content $out) | Select-Object -First 11              # 一帧 = 11 行
  ```
- **诊断探针**（把 `Maze` 实际造出来的 grid 打印出来，文件放 `build/` 里、不进 git）：
  `g++ -std=c++17 -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall -Wextra -Iinclude build/probe.cpp src/maze.cpp -o build/probe.exe` 然后 `.\build\probe.exe`
  （探针源码 2026-10-08 用完已删，需要时让 AI 十秒重建一份。）
- ⚠️ **别拿 `g++ -fsyntax-only` 当验收**（抓不到 `-Wreturn-type` 这类控制流警告），只能当打字检查
- 终端看中文先敲：`[Console]::OutputEncoding = [System.Text.Encoding]::UTF8`
- VSCode：任意文件按 **F5** = 先跑 `CMake: 构建` 再启动 `build\Maze.exe`（外部控制台）
- **VS2022（本机）**：打开 `Maze` 文件夹 → 顶部选 preset **`x64-debug`** → 配置 → F5。
  （**不要**用默认的 Ninja 路线，见 M18；`.vscode` 那套命令本机是死路，见「本机环境事实」）
- **主力机（MinGW 那台）**：命令行 `cmake --preset mingw-debug` → `cmake --build --preset mingw-debug`；
  或用老命令：`cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=D:/Tool/VSCode/mingw64/bin/g++.exe -DCMAKE_MAKE_PROGRAM=D:/Tool/VSCode/mingw64/bin/mingw32-make.exe` → `cmake --build build`。
  ⚠️ **换机时 preset 要跟着换**（`x64-debug` ↔ `mingw-debug`），两个 preset 各自只在自己的机器上有效。

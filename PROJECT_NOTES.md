# Maze 项目日志

> 本文件是"跨设备共享记忆"：每台电脑/每个账户开工前先读它，收工后更新它，然后提交推送到 GitHub。
> 由 AI 助手维护，用户负责提交推送。
> 最后更新：**2026-10-08**

## 一句话简介

终端版迷宫项目：**迷宫生成 + 寻路可视化 + 人机竞速**（C++17 / STL / CMake，纯零依赖）。

进度：**v1-a 的"迷宫数据层"已完成**（`Maze` 五个函数全部正确、单测 2 用例 / 6 断言全绿、编译零警告、远端仓库已通）。
**当前卡在 v1-a 的最后一步**：`src/main.cpp` 还是半成品（三个 `carve` 参数不是相邻房间 + 打印缺换行），跑起来会崩。
修完那两个点，迷宫就会第一次出现在屏幕上，v1-a 正式收工。

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

**v1-a 迷宫数据层（2026-10-08 完成）**

- [x] `include/maze.h`：`Maze` 结构（w/h/grid 公开成员 + 5 个函数声明）
- [x] `src/maze.cpp`：
      `Maze(width, height)` 造"全是墙"的迷宫（`grid.resize(h)` + 每行 `std::string(w, '#')`）、
      `inBounds`（负数与上界都管）、
      `at`（**越界一律返回 `'#'`**）、
      `setAt`、`carve`（`grid[(y1+y2)/2][(x1+x2)/2] = '.'`，取中点找门）
- [x] `tests/test_main.cpp` 用例"地图初始化"：全墙校验 + 房间数 == 6 + 两个越界取值
- [x] `CMakeLists.txt` 挂上 `src/maze.cpp`（**两个 target 都要挂**：`Maze` 与 `unit_tests`）
- [x] 实测验收：`cmake --build build --clean-first` **零警告**；`.\build\unit_tests.exe` **2 用例 / 6 断言全绿**

### 待办（下次严格按此顺序）

- [ ] **★ 明天第一件事：修 `src/main.cpp`（v1-a 收尾，只改这一个文件）**
      1. 三个 `carve` 参数**不是相邻房间** —— `carve` 要求"一个坐标相同、另一个差 2"，房间坐标必须是奇数。
         现有 `carve(3,5, 5,7)` / `carve(9,3, 11,5)` 都是**对角线**；`carve(19,21, 6,8)` 更离谱（y=21 超出高度 11，
         `(6,8)` 是偶偶承重柱，算出的中点 `(12,14)` 直接越界）。
         合法例子：水平相邻 `carve(1,1, 3,1)` → 门在 `(2,1)`；垂直相邻 `carve(3,1, 3,3)` → 门在 `(3,2)`。
      2. 打印循环**缺 `'\n'`**，11 行会挤成一行；顺手把写死的 `11` 换成 `maze.h`。
      **验收**：`cmake --build build` + `.\build\Maze.exe` → 屏幕上出现 11 行 × 21 字符的迷宫骨架，墙里露出几个 `.`
- [ ] **v1-a 收工后**：`git add -A && git commit && git push`（先开 Steam++）
- [ ] **v1-b**：`include/generator.h` + `src/generator.cpp` —— **DFS 回溯生成**（栈；随机取未访问邻居；打通墙后入栈，走不动就回溯）+ 单测（**生成必连通**）→ 这才是真正"挖"出迷宫的一步
- [ ] **v1-c**：`src/render.cpp` —— 把渲染从 main 里拆出来（复用 Snack 的 "grid 缓冲 + 光标回位覆盖重绘" 做法）
- [ ] v2 起照路线图推进（玩家移动 → BFS 画路 → 搜索动画 → A\* 带权 → 人机竞速）
- [ ] 可选打磨（**不着急**）：`at()` 里 `x >= w || y >= h || !inBounds(x, y)` 的前两个条件是重复的（`inBounds` 已经在管），可简化成 `if (!inBounds(x, y)) return '#';` —— 好处是"越界规则只有一处"；`setAt` / `carve` 目前不做任何合法性检查（传错参数会静默写坏数据），以后可加保护

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
- **`at()` 越界一律返回 `'#'`**：把"界外"统一当成墙，后面写渲染、寻路时**调用方完全不用自己判边界**，能省掉一大把 `if (x < 0 || x >= w)`。
- **网格用 `vector<string>` 而不是 `vector<vector<char>>`**：可以直接 `std::cout << grid[y]` 整行打印、内存连续；先选省事的，有性能需求再换。
- **`git init` 前先报备**：本轮用户点头后才做；`--clean-first` 才能真验"零警告"（增量构建不会重编老文件）。

## 踩坑库

- **通用踩坑库（34 条，教科书级）**：见 `D:\Tool\VSCode\Memory\Snack\PROJECT_NOTES.md` 的"踩坑库"章节
  （`enum class` 不能从 int 构造、函数声明与定义必须逐字一致、CMake 未定义变量静默展开为空、
  目录改名后 build 缓存失效、`-Wall -Wextra` 的价值、PowerShell 的 UTF-8 编码坑……）。
  **撞上类似问题时直接引用编号**，不重复抄写。

### Maze 新增（M1~M9）

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

## 知识点地图

**迷宫表示法（本轮新增）**

- **奇偶格表示法**：让"墙"自己占一个格子。奇数行 + 奇数列 = **房间**（能走的格）；偶偶 = **永久承重柱**（生成算法永不碰）；一奇一偶 = **门的位置**（打通就是 `.`）。
  代价是"缩水"（21×11 的画布只有 10×5 = 50 个房间），换来的是**屏幕上看到的就是内存里存的**，渲染不用翻译。
  因为房间只能落在奇数坐标，**宽高必须是奇数**。
- **"取中点找门"**：相邻房间一个坐标相同、另一个差 2，正中间那格就是门。两个奇数相加是偶数 → `(x1+x2)/2` 一定整除，不会出现小数坐标。

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
- **诊断探针**（把 `Maze` 实际造出来的 grid 打印出来，文件放 `build/` 里、不进 git）：
  `g++ -std=c++17 -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall -Wextra -Iinclude build/probe.cpp src/maze.cpp -o build/probe.exe` 然后 `.\build\probe.exe`
  （探针源码 2026-10-08 用完已删，需要时让 AI 十秒重建一份。）
- ⚠️ **别拿 `g++ -fsyntax-only` 当验收**（抓不到 `-Wreturn-type` 这类控制流警告），只能当打字检查
- 终端看中文先敲：`[Console]::OutputEncoding = [System.Text.Encoding]::UTF8`
- VSCode：任意文件按 **F5** = 先跑 `CMake: 构建` 再启动 `build\Maze.exe`（外部控制台）

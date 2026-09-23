# 更新日志

本文件记录 DODHooks 的所有重要变更。

## [1.6.3] - 2026-09-24

### 核对结论（无需修改）

- **已对当前服务端二进制（`server.dll`，PE 时间戳 `0x6AAA1368`，2026-09-16）
  逐字节复核 gamedata：**
  - 9 条 Windows x86 签名**全部唯一命中**，无歧义、无偏移漂移。
  - `g_pObjectiveResource` 偏移 `67` 经反汇编确认落在 `CreateStandardEntities`
    中创建 `dod_objective_resource` 那条 `mov [g_pObjectiveResource], eax`
    指令的 4 字节操作数上。（偏移 40 是 `g_pPlayerManager`，**不要把 67 改成 40**。）
  - 结论：**签名与偏移本次都不需要更新。**

### 修复

- **严重：单个 detour 失败不再导致整个扩展加载失败。** 原先 9 条签名中任意
  一条解析失败，`SetupDetours()` 就返回 false，扩展直接加载不出来（`sm exts list`
  里看不到）。现在失败只报警，扩展照常加载并打印
  `DODHooks: loaded - N/9 detours active`，逐个列出失效的钩子。
- **严重：回合状态切换被吞掉。** `RoundState` 用 `static iPreviousRoundState`
  去重，遇到重复值直接 return 而**不调用原函数**，会丢掉合法的状态转换
  （且缓存只在插件返回 `Plugin_Handled` 时更新），可能导致回合卡死。已移除该缓存。
- **严重：forward 注册/释放顺序。**
  - forward 改为在 `SDK_OnLoad` 中、**启用任何 detour 之前**创建（detour 回调
    会解引用 forward 指针，先装钩子后建 forward 等于埋空指针）。
  - `SDK_OnUnload` 改为**先拆 detour 再释放 forward**（原先顺序相反，detour
    仍生效时 forward 已被释放）。
- **`SDK_OnAllLoaded` 不再因 bintools 缺失而提前 return。** 原先会跳过 offset
  解析和 `InitializeValveGlobals()`。现在只影响调用型 native。
- **未解析的 sendprop 不再写坏内存。** `GetSendPropOffset` 失败返回 -1
  （0xFFFFFFFF），native 会直接往这个地址写。现改为返回
  `INVALID_SEND_PROP_OFFSET`，所有 native 先校验再写，并在加载时于控制台告警。
- **`DOD_SetNumControlPoints` 缺少范围校验**，可写入任意值。已加 `0..MAX_CONTROL_POINTS` 校验。
- **`dodhooks.inc` 兵种枚举整体错位 1。** inc 里写的是
  `DODClass_None = 0, DODClass_Rifleman = 1 ... DODClass_Rocket = 6`，而游戏
  （以及 `extension.h`）用的是 `None = -1, Rifleman = 0 ... Rocket = 5`
  （已验证：`HandleCommand_JoinClass` 用 `-2` 判断随机兵种）。所有兵种都偏了 1。
  现已与游戏对齐，两侧都加了防回归注释。

### 变更

- **更正：`bin/x64/server.dll` 确实是 DoD:S 的服务端二进制。** 之前"DoD:S 没有
  64 位服务端"的说法是错的 —— 这个 x64 DLL 导出的 8 个符号与 x86 版完全一致
  （`CDODBombDispenser`、`CAreaCapture`、`CWeaponDODBase`、`CreateInterface`、
  `cvar`），只是调用约定换成了 x64 写法，并且同样包含 `dod_objective_resource`、
  `DODRoundState: entering`、`BOMB_TARGET_ACTIVE` 等服务端字符串。它是一个
  **完整可用的 64 位服务端二进制**。
- **已预置 `windows64` 签名**，为将来 Valve 启用 64 位服务端做准备。已定位并
  提供签名的有：`CreateStandardEntities`、`DODRespawn`、`AddWaveTime`、
  `RoundState`、`PlayerState`、`BombTargetState`。
  `VoiceCommand`、`JoinClass`、`PopHelmet`、`SetWinningTeam` 在 x64 中有多个
  形态接近的候选函数、无法唯一确定，**故意不提供** —— 宁可让那一个 detour
  报"未启用"，也不冒险钩错函数导致崩溃。
- **x64 下 `g_pObjectiveResource` 的解析方式重写。** x86 存的是绝对地址
  （`mov [abs32], eax`），而 x64 用 RIP 相对寻址（`mov [rip+disp32], rax`），
  那 4 字节是**相对位移**而非指针。现在 x64 的 offset 指向位移字段，由
  `vglobals.cpp` 计算 `target = pAddress + offset + 4 + disp32`。
  （`windows64` 的 75 = 第二条 store = 目标资源；第一条 39 是 `g_pPlayerManager`。）

### 新增

- **`dodhooks.txt` 增加 `linux64` 键**：64 位 Linux 服务端会优先查 `linux64`，
  缺失则符号解析静默失败。
- **诊断用 native**：`DOD_IsAvailable()`、`DOD_GetDetourCount()`、
  `DOD_GetDetourTotal()`、`DOD_IsObjectiveResourceReady()`。
- **`dodhooks_test.sp` 诊断增强**：`dodhooks_test` 会打印 detour 健康度
  （`Detours active: 9/9`）、objective resource 就绪状态、兵种枚举自检，
  并说明每种失败模式代表什么；`dodhooks_status` 输出各 forward 触发次数，
  在"钩子装上了但一个都没触发"时给出提示。

## [1.6.1] - 2026-08-24

### 修复

- **关键修复：扩展自动加载功能失效。** `dodhooks.inc` 头文件中 `public Extension` 块的 `file` 字段被错误地写为 `"dodhooks.ext.2.dods"`，但 SourceMod 的自动加载机制期望填写**基础文件名** `"dodhooks.ext"`，由引擎自动追加游戏后缀（`.2.dods`）和平台扩展名（`.so`/`.dll`）。写死完整文件名导致解析失败，扩展无法自动加载，必须手动执行 `sm exts load dodhooks` 才能生效。现已修正为 `file = "dodhooks.ext"`——与 2015 年 psychonic 原版及所有标准 SourceMod 扩展（sdkhooks、dhooks 等）保持一致。

### 新增

- `__ext_dodhooks_SetNTVOptional()` — 当未定义 `REQUIRE_EXTENSIONS` 时，将全部 19 个 native 标记为可选，使插件在扩展不可用时仍能正常加载（调用时抛出运行时错误而非阻止插件加载）。恢复自 2015 年 psychonic 原版头文件。
- 通过 `AUTOLOAD_EXTENSIONS` 和 `REQUIRE_EXTENSIONS` 宏条件控制 `autoload` / `required`，允许插件作者在编译时控制扩展加载行为。恢复自 2015 年 psychonic 原版头文件。
- 恢复 2015 年 psychonic 原版中的枚举定义：
  - `DODRoundState` — 回合状态（RoundInit、PreGame、StartGame、PreRound、RoundRunning、AlliesWin、AxisWin、Restart、GameOver）
  - `DODPlayerState` — 玩家状态（Active、Welcome、PickingTeam、PickingClass、DeathAnim、ObserverMode）
  - `DODBombTargetState` — 炸弹点状态（Inactive、Active、Armed）
  - `DODVoiceCommand` — 全部 39 个语音命令 ID（Attack、Hold、Move、Medic、Grenade、Sniper 等）
- `#define MAX_CONTROL_POINTS 8` 常量。
- `stock bool IsPlayerClassValid(DODPlayerClass playerClass)` 工具函数。

### 变更

- `DOD_SetRoundState`、`DOD_SetPlayerState` 和 `DOD_SetBombTargetState` 现在使用强类型枚举参数（`DODRoundState`、`DODPlayerState`、`DODBombTargetState`）替代原始 `int`，为插件作者提供编译期类型检查。

## [1.6.0] - 2026-08-23

### 新增

- 通过 SourcePawn 头文件自动加载：`dodhooks.inc` 中新增 `public Extension __ext_dodhooks` 声明，任意 `#include <dodhooks>` 的插件都会由 SourceMod 在运行时自动加载 `dodhooks.ext`（`dodhooks.ext.2.dods`），无需 `.autoload` 标记文件或手动 `sm exts load`。
- 单命令同时构建两种架构：
  - `build.bat`（Windows）构建 x86 + x64，并生成 `DODHooks-<版本>-sm1.12-windows.zip`
  - `build.sh`（Linux）构建 x86 + x64，并生成 `DODHooks-<版本>-sm1.12-linux.tar.gz`
  - `build_linux_docker.sh` 在官方 AlliedModders 构建容器内运行 Linux 构建
- 发布打包结构：
  - 32 位二进制放在 `extensions/`（默认）
  - 64 位二进制放在 `extensions/x64/` 子目录
  - GameData 取自仓库内的 `gamedata/dodhooks.txt`
  - `dodhooks.inc` 一并打包到 `scripting/include/`
- GitHub Actions 工作流：构建 Windows（x86+x64）和 Linux（x86+x64），并在打 tag 时发布三个压缩包：
  - `DODHooks-<tag>-sm1.12-windows.zip`
  - `DODHooks-<tag>-sm1.12-linux.zip`
  - `DODHooks-<tag>-source.zip`
- 双语文档（`README.md` / `README_zh.md`）及本更新日志。

### 修复

- 修正 `configure.py` 参数：所有构建脚本、`Dockerfile` 与 CI 统一使用 `--arch=x86|x64`（原先为 `--target`）以及 `--sdks=dods`（原先为 `dod`）。
- `PackageScript` 现在从仓库的 `gamedata/dodhooks.txt` 复制 GameData，并从 `sourcemod/scripting/include/dodhooks.inc` 复制头文件（原先引用了错误路径）。
- 依赖版本统一：构建脚本、CI 与依赖设置脚本现在均使用 Metamod:Source `1.12-dev` 分支（原先 Windows 相关构建使用 `1.11-dev`）。

### 变更

- 整合构建脚本，移除了无法工作的 Windows→Linux 交叉编译脚本，以及冗余的 `docker/`、`scripts/`、`build_linux.sh` 文件。
- GameData 默认段明确面向胜利之日：起源（游戏目标 `dod`）。

### 兼容性

- SourceMod 1.12 / 1.13
- Metamod:Source 1.12 / 2.0
- Windows / Linux，32 位与 64 位

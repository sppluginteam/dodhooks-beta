<p align="center">
  <img src="https://shared.cdn.queniuqe.com/store_item_assets/steam/apps/300/header.jpg?t=1763407002" alt="胜利之日：起源" width="460" />
</p>

<h1 align="center">DODHooks</h1>

<p align="center">
  <strong>面向《胜利之日：起源》(Day of Defeat: Source) 的 SourceMod 扩展——提供函数劫持与原生接口</strong>
</p>

<p align="center">
  <a href="https://github.com/sppluginteam/dodhooks-beta/actions/workflows/master.yaml">
    <img src="https://github.com/sppluginteam/dodhooks-beta/actions/workflows/master.yaml/badge.svg" alt="构建状态" />
  </a>
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases/latest">
    <img src="https://img.shields.io/github/v/release/sppluginteam/dodhooks-beta?style=flat-square&label=%E6%9C%80%E6%96%B0%E7%89%88%E6%9C%AC" alt="最新版本" />
  </a>
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases">
    <img src="https://img.shields.io/github/downloads/sppluginteam/dodhooks-beta/total?style=flat-square&label=%E4%B8%8B%E8%BD%BD%E9%87%8F" alt="总下载量" />
  </a>
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases/latest">
    <img src="https://img.shields.io/github/release-date/sppluginteam/dodhooks-beta?style=flat-square&label=%E5%8F%91%E5%B8%83%E6%97%A5%E6%9C%9F" alt="发布日期" />
  </a>
  <img src="https://img.shields.io/badge/SourceMod-1.12%20%2F%201.13-orange?style=flat-square" alt="SourceMod" />
  <img src="https://img.shields.io/badge/%E5%B9%B3%E5%8F%B0-Windows%20%2F%20Linux-blue?style=flat-square" alt="平台" />
  <img src="https://img.shields.io/badge/%E6%9E%B6%E6%9E%84-x86%20%2F%20x64-green?style=flat-square" alt="架构" />
</p>

---

## 下载

<p align="center">
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases/latest">
    <img src="https://img.shields.io/github/v/release/sppluginteam/dodhooks-beta?style=for-the-badge&label=%E6%9C%80%E6%96%B0%E7%A8%B3%E5%AE%9A%E7%89%88%E6%9C%AC&color=brightgreen" alt="最新稳定版本" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases/latest">
    <img src="https://img.shields.io/badge/%E4%B8%8B%E8%BD%BD-Windows%E7%89%88-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="下载 Windows 版" />
  </a>
  &nbsp;
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases/latest">
    <img src="https://img.shields.io/badge/%E4%B8%8B%E8%BD%BD-Linux%E7%89%88-FCC624?style=for-the-badge&logo=linux&logoColor=black" alt="下载 Linux 版" />
  </a>
  &nbsp;
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases/latest">
    <img src="https://img.shields.io/badge/%E4%B8%8B%E8%BD%BD-%E6%BA%90%E4%BB%A3%E7%A0%81-4285F4?style=for-the-badge&logo=github&logoColor=white" alt="下载源代码" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/sppluginteam/dodhooks-beta/releases">
    <img src="https://img.shields.io/badge/%E6%9F%A5%E7%9C%8B-%E5%85%A8%E9%83%A8%E7%89%88%E6%9C%AC-gray?style=flat-square" alt="全部版本" />
  </a>
</p>

> 每个发布压缩包均包含 32 位扩展二进制、存放 64 位二进制的 `x64/` 子目录、`gamedata/dodhooks.txt` 以及 `scripting/include/dodhooks.inc`。上方版本号由 GitHub 最新发布自动检测，无需手动更新。

---

## 简介

DODHooks 是一个面向 **《胜利之日：起源》(Day of Defeat: Source)** 的 SourceMod 扩展，提供：

- **函数劫持（Detours）**：拦截关键游戏函数——语音命令、选择兵种、头盔脱落、玩家复活、波次时间、胜利阵营、回合状态、玩家状态、炸弹目标状态。
- **原生接口（Natives）**：在 SourcePawn 插件中控制玩家兵种、控制点图标、回合计时器以及游戏规则。
- **转发事件（Forwards）**：允许插件拦截并修改游戏事件。

本版本为持续维护的分支，特性包括：

- 支持 **SourceMod 1.12 与 1.13**
- 支持 **Metamod:Source 1.12 与 2.0**
- 同时编译 **32 位（x86）与 64 位（x86_64）** 架构
- 兼容 **Windows 与 Linux**
- 使用最新的 **AMBuild 2.x** 构建系统
- 修复了旧版本中存在的服务器崩溃问题
- 使用现代 C++17 编译标准
- **通过头文件自动加载**——无需手动执行 `sm exts load`（v1.6.1 修复）

## 环境要求

| 依赖 | 版本 | 说明 |
|------|------|------|
| SourceMod | 1.12 / 1.13 | 编译需要源代码 |
| Metamod:Source | 1.12 / 2.0 | 编译需要源代码 |
| AMBuild | 2.2+ | 基于 Python 的构建系统 |
| Python | 3.8+ | AMBuild 运行所需 |
| 编译器 | GCC 9+ / Clang 10+ / MSVC 2019+ | 需支持 C++17 |

## 构建依赖

### Linux

```bash
# Debian/Ubuntu
sudo apt-get update
sudo apt-get install -y build-essential clang-22 python3 python3-pip git

# 安装 AMBuild
pip3 install --upgrade git+https://github.com/alliedmodders/ambuild.git
```

### Windows

```powershell
# 从 python.org 安装 Python 3.12+
# 安装 Visual Studio 2019+（社区版即可）
# 从 git-scm.com 安装 Git
# 安装 AMBuild
python -m pip install --upgrade git+https://github.com/alliedmodders/ambuild.git
```

## 编译

一次运行即可同时产出 32 位（x86）和 64 位（x64）二进制文件，并整理到可直接发布的 `dist/` 目录中（扩展通过 SourcePawn 头文件自动加载）。

### 快速开始（Linux）

```bash
# 克隆仓库
git clone https://github.com/sppluginteam/dodhooks-beta.git
cd dodhooks

# 克隆依赖
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/metamod-source.git mmsource
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod

# 同时构建 32 位 + 64 位，整理到 dist/ 并生成 .tar.gz
./build.sh
```

产物位于 `dist/addons/sourcemod/extensions/`，包含 32 位 `.so`、存放 64 位 `.so` 的 `x64/` 子目录、附带的 `dodhooks.inc` 头文件，以及 `dist/addons/sourcemod/gamedata/dodhooks.txt`。

### Windows 编译

```powershell
# 打开"VS 开发者命令提示符"（或任意终端；脚本会自动定位 vcvarsall.bat）

git clone https://github.com/sppluginteam/dodhooks-beta.git
cd dodhooks

git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/metamod-source.git mmsource
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod

# 同时构建 32 位 + 64 位，整理到 dist/ 并生成 .zip
build.bat
```

### 手动 / 高级编译（直接使用 AMBuild）

如需手动编译单一架构：

```bash
mkdir build && cd build
python3 ../configure.py \
    --sm-path ../sourcemod \
    --mms-path ../mmsource \
    --arch=x86 \
    --enable-optimize
ambuild
# 64 位：使用 --arch=x64
```

> **注意：** configure 参数为 `--arch=x86` / `--arch=x64`（而非 `--target`）。SDK 选择参数为 `--sdks=dods`。

### 生成 Visual Studio 工程（Windows）

```powershell
python ..\configure.py `
    --sm-path ..\sourcemod `
    --mms-path ..\mmsource `
    --arch=x86 `
    --enable-optimize `
    --gen=vs
```

## Docker 编译

仓库提供了 `Dockerfile` 和一键封装脚本（`build_linux_docker.sh`），可在容器内完成可复现的 Linux 编译（无需在宿主机安装工具链）：

```bash
# 在官方 AlliedModders 容器内编译（依赖会克隆到 deps/）：
./build_linux_docker.sh

# 或手动执行：
docker build -t dodhooks-builder .
docker run --rm -v $(pwd):/work/dodhooks -w /work/dodhooks dodhooks-builder \
    bash -c "pip3 install --upgrade ambuild; ./build.sh"
```

## 安装

编译完成后（或下载发布压缩包），将内容复制到游戏服务器的根目录：

```
addons/
└── sourcemod/
    ├── extensions/
    │   ├── dodhooks.ext.2.dods.dll        (Windows 32 位)
    │   ├── dodhooks.ext.2.dods.so         (Linux 32 位)
    │   └── x64/
    │       ├── dodhooks.ext.2.dods.dll    (Windows 64 位)
    │       └── dodhooks.ext.2.dods.so     (Linux 64 位)
    ├── gamedata/
    │   └── dodhooks.txt
    └── scripting/
        └── include/
            └── dodhooks.inc
```

### 加载扩展

扩展会**自动加载**：任何 `#include <dodhooks>` 的插件都会在运行时由 SourceMod 自动加载 `dodhooks.ext`（解析为 `dodhooks.ext.2.dods`）。这一机制由 `dodhooks.inc` 中的 `public Extension __ext_dodhooks` 块实现，因此**不需要** `.autoload` 标记文件，也不需要手动执行命令。

> **重要提示（v1.6.1 修复）：** `dodhooks.inc` 中的 `file` 字段必须为基础名 `"dodhooks.ext"`。SourceMod 会自动追加游戏后缀 `.2.dods` 和平台扩展名。**切勿**写为 `"dodhooks.ext.2.dods"`——这会导致自动加载解析失败。

如需手动加载（例如调试），使用：

```
sm exts load dodhooks
```

> **不要**使用 `meta load`——该命令用于 Metamod:Source 插件，对 `.dll`/`.so` 扩展会报"File type not supported"错误。

## 可用原生接口（Natives）

| 原生接口 | 说明 |
|----------|------|
| `DOD_GetPlayerClass(client)` | 获取玩家当前兵种 |
| `DOD_SetPlayerClass(client, class)` | 设置玩家当前兵种 |
| `DOD_GetDesiredPlayerClass(client)` | 获取玩家期望兵种 |
| `DOD_SetDesiredPlayerClass(client, class)` | 设置玩家期望兵种 |
| `DOD_PopHelmet(client, velocity[3], origin[3])` | 强制脱落玩家头盔 |
| `DOD_SetNumControlPoints(num)` | 设置控制点数量 |
| `DOD_PrecacheCPIcon(material)` | 预缓存控制点图标材质 |
| `DOD_SetCPIcons(index, ...)` | 设置控制点图标 |
| `DOD_SetCPVisible(index, visible)` | 显示/隐藏控制点 |
| `DOD_PauseTimer(timer)` | 暂停回合计时器 |
| `DOD_ResumeTimer(timer)` | 恢复回合计时器 |
| `DOD_SetTimeRemaining(timer, seconds)` | 设置计时器剩余时间 |
| `DOD_GetTimeRemaining(timer)` | 获取计时器剩余时间 |
| `DOD_RespawnPlayer(client, useClass)` | 强制复活玩家 |
| `DOD_AddWaveTime(team, delay)` | 为阵营增加波次时间 |
| `DOD_SetWinningTeam(team)` | 设置胜利阵营 |
| `DOD_SetRoundState(state)` | 设置回合状态 |
| `DOD_SetPlayerState(client, state)` | 设置玩家状态 |
| `DOD_SetBombTargetState(entity, state)` | 设置炸弹目标状态 |

## 可用转发事件（Forwards / Hooks）

| 转发事件 | 说明 |
|----------|------|
| `OnVoiceCommand(client, &voiceCommand)` | 玩家使用语音命令时触发 |
| `OnJoinClass(client, &playerClass)` | 玩家选择兵种时触发 |
| `OnPopHelmet(client, velocity[3], origin[3])` | 玩家头盔脱落时触发 |
| `OnPlayerRespawn(client)` | 玩家即将复活时触发 |
| `OnAddWaveTime(team, &delay)` | 增加波次时间时触发 |
| `OnSetWinningTeam(team)` | 设置胜利阵营时触发 |
| `OnEnterRoundState(&roundState)` | 回合状态变更时触发 |
| `OnEnterPlayerState(client, &playerState)` | 玩家状态变更时触发 |
| `OnEnterBombTargetState(entity, &bombState)` | 炸弹目标状态变更时触发 |

## 枚举类型

头文件提供以下枚举，支持类型安全的开发：

| 枚举 | 取值 |
|------|------|
| `DODPlayerClass` | Random、None、Rifleman、Assault、Support、Sniper、Machinegunner、Rocket |
| `DODRoundState` | RoundInit、PreGame、StartGame、PreRound、RoundRunning、AlliesWin、AxisWin、Restart、GameOver |
| `DODPlayerState` | Active、Welcome、PickingTeam、PickingClass、DeathAnim、ObserverMode |
| `DODBombTargetState` | Inactive、Active、Armed |
| `DODVoiceCommand` | 39 个语音命令 ID（Attack、Hold、Move、Medic、Grenade、Sniper 等） |

## 相较原版的改进

- **兼容 SourceMod 1.12/1.13** — 更新了 API 与构建系统
- **支持 64 位** — 可在 64 位服务器上编译并运行
- **现代 C++17** — 更新了编译标志与标准
- **修复崩溃** — 解决了多种服务器崩溃场景：
  - detour 回调中的 NULL 指针检查
  - 64 位 ThisCall 调用约定的正确栈对齐
  - 更安全的 gamedata 签名解析与更清晰的错误信息
  - 防止无效实体引用
- **改进错误处理** — 缺失 gamedata 或签名时提供更清晰的错误信息
- **GitHub Actions CI** — 自动构建 4 种平台（Win/Linux x86/x64）
- **Docker 支持** — 通过容器化实现可复现构建
- **修复头文件自动加载（v1.6.1）** — 修正 `file` 字段为基础名 `"dodhooks.ext"`，扩展无需手动 `sm exts load` 即可自动加载
- **恢复头文件特性（v1.6.1）** — `MarkNativeAsOptional` 降级机制、`AUTOLOAD_EXTENSIONS`/`REQUIRE_EXTENSIONS` 条件编译、全部枚举（RoundState/PlayerState/BombTargetState/VoiceCommand）、`MAX_CONTROL_POINTS` 常量、`IsPlayerClassValid()` 工具函数
- **强化类型安全（v1.6.1）** — `SetRoundState`/`SetPlayerState`/`SetBombTargetState` 使用枚举类型参数

## 更新日志

完整版本历史请参阅 [CHANGELOG_zh.md](CHANGELOG_zh.md)。
（英文版：[CHANGELOG.md](CHANGELOG.md)）

## 故障排查

扩展加载不起来，或者加载了没反应？先看这里：

- 中文：[TROUBLESHOOTING_zh.md](TROUBLESHOOTING_zh.md)
- English：[TROUBLESHOOTING.md](TROUBLESHOOTING.md)

从源码编译：[BUILD.md](BUILD.md)

## 许可证

GPL v2 — 详见 [LICENSE](LICENSE)。

## 致谢

- **Andersso** — 原作者
- **ChesterSmitty** — 前维护者
- **Apfelwurm** — CI 改进
- **DNA-styx** — Gamedata 文件
- **Kittenks** — 当前维护者（1.12/1.13 更新、构建与打包）— https://github.com/sppluginteam/dodhooks-beta
- **AlliedModders** — SourceMod、Metamod:Source、AMBuild

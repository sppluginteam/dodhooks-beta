# 构建 DODHooks

DODHooks 同时为 Windows 和 Linux 编译 **32 位 (x86) 与 64 位 (x64)** 两个版本。
构建脚本会生成可直接发布的 `dist/` 目录和压缩包。

## 环境要求

- Python 3.8+，安装 AMBuild：`pip install ambuild`（或从
  <https://github.com/alliedmodders/ambuild> 源码安装）
- 依赖（放在 `deps/` 下，或通过构建脚本参数指定路径）：
  - SourceMod（`1.12-dev` 分支）→ `deps/sourcemod`
  - Metamod:Source（`1.12-dev` 分支）→ `deps/mmsource`
  - HL2SDK（DOD:S 的 **`dods` 分支，始终用最新**）→ `deps/hl2sdk-dods`

一键拉取依赖：

```bash
./setup_dependencies.sh
git clone --depth 1 -b dods https://github.com/alliedmodders/hl2sdk.git deps/hl2sdk-dods
```

CI 工作流始终克隆 `alliedmodders/hl2sdk` 的**最新** `dods` 分支，
因此 GitHub Actions 的构建不依赖本地 SDK。

## Windows（本机编译，产出 x86 + x64 两个 .dll）

在 VS 开发者命令行里执行（或直接运行 `build.bat`，它会自动定位 `vcvarsall.bat`）：

```bat
build.bat
REM 产物：
REM   dist\addons\sourcemod\extensions\dodhooks.ext.2.dods.dll          (32 位)
REM   dist\addons\sourcemod\extensions\x64\dodhooks.ext.2.dods.dll      (64 位)
REM   DODHooks-<ver>-sm1.12-windows.zip
```

## Linux（本机编译，产出 x86 + x64 两个 .so）

```bash
./build.sh          # 等同于 ./build_linux.sh
# 产物：
#   dist/addons/sourcemod/extensions/dodhooks.ext.2.dods.so          (32 位)
#   dist/addons/sourcemod/extensions/x64/dodhooks.ext.2.dods.so      (64 位)
#   DODHooks-<ver>-sm1.12-linux.tar.gz
```

32 位 Linux 构建需要 multilib：`apt-get install gcc-multilib g++-multilib libstdc++6:i386`

## Linux（Docker，最省事，无需本地工具链）

```bash
./build_linux_docker.sh
```

使用 AlliedModders 官方构建镜像。

## GitHub Actions

`.github/workflows/master.yaml` 定义了三个 job：

- `windows` — 构建 Windows x86 + x64
- `linux` — 构建 Linux x86 + x64
- `release` — 从 `smsdk_config.h` 读取版本号，发布
  `DODHooks-<ver>-sm1.12-windows.zip`、`DODHooks-<ver>-sm1.12-linux.zip`
  和 `DODHooks-<ver>-source.zip`

要获取 Linux 的 `.so` 而不想本地准备 SDK，推送到 `master` 让 `linux` job 运行即可。

## 安装与加载

把 `dist/` 里的 `addons/` 目录整个复制到游戏目录。压缩包已在
`addons/sourcemod/extensions/` 根目录包含一个 0 字节的 `dodhooks.autoload` 标记，
因此 SourceMod 在 32 位和 64 位服务器上都会自动加载 DODHooks ——
不需要插件 `#include`，也不需要手动 `sm exts load`。用以下命令验证：

```
meta list          ; 确认对应架构的 Metamod 已加载（win64 / win32）
sm exts list       ; DODHooks 应显示为 "running"
sm dhookstest      ; 打印 detour 数量和 forward 状态
```

完整的 natives/forwards 列表见 `README_zh.md`；如果扩展没有自动加载，
见 `TROUBLESHOOTING_zh.md`。

_(English document: [BUILD.md](BUILD.md))_

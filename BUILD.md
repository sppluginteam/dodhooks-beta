# 构建 DODHooks

## 环境要求

- Python 3.8+，装 AMBuild：`pip install ambuild`（或从
  <https://github.com/alliedmodders/ambuild> 源码安装）
- 依赖（放在 `deps/` 下，或由环境变量指定）：
  - SourceMod（`1.12-dev`）：`deps/sourcemod`
  - Metamod:Source（`1.12-dev`）：`deps/mmsource`
  - HL2SDK（DOD:S 分支）：`deps/hl2sdk-dods`

一键拉取依赖：

```bash
./setup_dependencies.sh            # 克隆 sourcemod + mmsource
git clone --depth 1 -b dods https://github.com/alliedmodders/hl2sdk.git deps/hl2sdk-dods
```

## Linux（本机编译，产出 x86 + x64 两个 .so）

```bash
./build_linux.sh          # 或 ./build.sh
# 产物：dist/addons/sourcemod/extensions/dodhooks.ext.2.dods.so
#       dist/addons/sourcemod/extensions/x64/dodhooks.ext.2.dods.so
# 归档：DODHooks-<ver>-sm1.12-linux.tar.gz
```

32 位需要 multilib：`apt-get install gcc-multilib g++-multilib libstdc++6:i386`

## Linux（Docker，最省事）

```bash
./build_linux_docker.sh
```

用 AlliedModders 官方构建镜像，宿主机不需要装工具链。

## Windows（本机编译，产出 x86 + x64 两个 .dll）

在 VS 命令行（或先跑 `vsvars32.bat`）里：

```bat
build.bat
REM 产物：dist\addons\sourcemod\extensions\dodhooks.ext.2.dods.dll
```

## 自动化发布（GitHub Actions）

`.github/workflows/master.yaml` 已配好三个 job：

- `windows` —— 构建 Windows x86 + x64
- `linux` —— 构建 Linux x86 + x64
- `release` —— 从 `smsdk_config.h` 读版本号，打包
  `DODHooks-<ver>-sm1.12-windows.zip`、`DODHooks-<ver>-sm1.12-linux.zip`
  和 `DODHooks-<ver>-source.zip` 并发布 Release

**要拿到 Linux 的 .so，最可靠的方式就是推到 master 让 Actions 跑 linux job**，
不需要本地准备 hl2sdk。

## 部署

把 `addons/` 整个目录拷进服务端根目录：

```
addons/sourcemod/extensions/dodhooks.ext.2.dods.so   (Linux 32 位)
addons/sourcemod/extensions/x64/dodhooks.ext.2.dods.so (Linux 64 位)
addons/sourcemod/gamedata/dodhooks.txt
addons/sourcemod/scripting/include/dodhooks.inc
```

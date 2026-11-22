# Building DODHooks

DODHooks builds **both 32-bit (x86) and 64-bit (x64)** binaries for Windows and Linux.
The build scripts produce a release-ready `dist/` folder and an archive.

## Requirements

- Python 3.8+ with AMBuild: `pip install ambuild` (or install from
  <https://github.com/alliedmodders/ambuild> source).
- Dependencies (place under `deps/`, or pass paths to the build scripts):
  - SourceMod (`1.12-dev` branch) → `deps/sourcemod`
  - Metamod:Source (`1.12-dev` branch) → `deps/mmsource`
  - HL2SDK for Day of Defeat: Source (**latest `dods` branch**) → `deps/hl2sdk-dods`

Fetch dependencies in one shot:

```bash
./setup_dependencies.sh
git clone --depth 1 -b dods https://github.com/alliedmodders/hl2sdk.git deps/hl2sdk-dods
```

The CI workflow always clones the **latest** `alliedmodders/hl2sdk` `dods` branch,
so GitHub Actions builds do not depend on a local SDK checkout.

## Windows (native, builds x86 + x64 .dll)

From a Visual Studio developer command prompt (or run `build.bat`, which locates
`vcvarsall.bat` automatically):

```bat
build.bat
REM output:
REM   dist\addons\sourcemod\extensions\dodhooks.ext.2.dods.dll          (32-bit)
REM   dist\addons\sourcemod\extensions\x64\dodhooks.ext.2.dods.dll      (64-bit)
REM   DODHooks-<ver>-sm1.12-windows.zip
```

## Linux (native, builds x86 + x64 .so)

```bash
./build.sh          # same as ./build_linux.sh
# output:
#   dist/addons/sourcemod/extensions/dodhooks.ext.2.dods.so          (32-bit)
#   dist/addons/sourcemod/extensions/x64/dodhooks.ext.2.dods.so      (64-bit)
#   DODHooks-<ver>-sm1.12-linux.tar.gz
```

32-bit Linux builds need multilib: `apt-get install gcc-multilib g++-multilib libstdc++6:i386`.

## Linux (Docker — easiest, no local toolchain)

```bash
./build_linux_docker.sh
```

Uses the official AlliedModders build container.

## GitHub Actions

`.github/workflows/master.yaml` defines three jobs:

- `windows` — builds Windows x86 + x64
- `linux` — builds Linux x86 + x64
- `release` — reads the version from `smsdk_config.h` and publishes
  `DODHooks-<ver>-sm1.12-windows.zip`, `DODHooks-<ver>-sm1.12-linux.zip`
  and `DODHooks-<ver>-source.zip` as a Release.

To get Linux `.so` files without a local SDK, push to `master` and let the
`linux` job run.

## Install / load

Copy the `addons/` folder from `dist/` into your game directory. The archive
already contains a zero-byte `dodhooks.autoload` marker at
`addons/sourcemod/extensions/`, so SourceMod loads DODHooks automatically on
both 32-bit and 64-bit servers — no plugin `#include` or manual
`sm exts load` is required. Verify with:

```
meta list          ; Metamod must be loaded (win64 / win32 variant)
sm exts list       ; DODHooks should show as "running"
sm dhookstest      ; prints detour count and forward status
```

See `README.md` for the full list of natives/forwards and
`TROUBLESHOOTING.md` if the extension does not auto-load.

_(中文文档见 [BUILD_zh.md](BUILD_zh.md))_

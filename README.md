<p align="center">
  <img src="https://shared.cdn.queniuqe.com/store_item_assets/steam/apps/300/header.jpg?t=1763407002" alt="Day of Defeat: Source" width="460" />
</p>

<h1 align="center">DODHooks</h1>

<p align="center">
  <strong>SourceMod Extension with Detours &amp; Natives for Day of Defeat: Source</strong>
</p>

<p align="center">
  <a href="https://github.com/kittenks/dodhooks/actions/workflows/master.yaml">
    <img src="https://github.com/kittenks/dodhooks/actions/workflows/master.yaml/badge.svg" alt="Build Status" />
  </a>
  <a href="https://github.com/kittenks/dodhooks/releases/latest">
    <img src="https://img.shields.io/github/v/release/kittenks/dodhooks?style=flat-square&label=Latest%20Release" alt="Latest Release" />
  </a>
  <a href="https://github.com/kittenks/dodhooks/releases">
    <img src="https://img.shields.io/github/downloads/kittenks/dodhooks/total?style=flat-square&label=Downloads" alt="Total Downloads" />
  </a>
  <a href="https://github.com/kittenks/dodhooks/releases/latest">
    <img src="https://img.shields.io/github/release-date/kittenks/dodhooks?style=flat-square&label=Released" alt="Release Date" />
  </a>
  <img src="https://img.shields.io/badge/SourceMod-1.12%20%2F%201.13-orange?style=flat-square" alt="SourceMod" />
  <img src="https://img.shields.io/badge/Platform-Windows%20%2F%20Linux-blue?style=flat-square" alt="Platform" />
  <img src="https://img.shields.io/badge/Arch-x86%20%2F%20x64-green?style=flat-square" alt="Architecture" />
</p>

---

## Download

<p align="center">
  <a href="https://github.com/kittenks/dodhooks/releases/latest">
    <img src="https://img.shields.io/github/v/release/kittenks/dodhooks?style=for-the-badge&label=Latest%20Stable%20Release&color=brightgreen" alt="Latest Stable Release" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/kittenks/dodhooks/releases/latest">
    <img src="https://img.shields.io/badge/🪟%20Download%20for%20Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Download for Windows" />
  </a>
  &nbsp;
  <a href="https://github.com/kittenks/dodhooks/releases/latest">
    <img src="https://img.shields.io/badge/🐧%20Download%20for%20Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" alt="Download for Linux" />
  </a>
  &nbsp;
  <a href="https://github.com/kittenks/dodhooks/releases/latest">
    <img src="https://img.shields.io/badge/📦%20Download%20Source-4285F4?style=for-the-badge&logo=github&logoColor=white" alt="Download Source" />
  </a>
</p>

<p align="center">
  <a href="https://github.com/kittenks/dodhooks/releases">
    <img src="https://img.shields.io/badge/📂%20View%20All%20Releases-gray?style=flat-square" alt="All Releases" />
  </a>
</p>

> Each release archive contains the 32-bit extension binary, an `x64/`
> subfolder with the 64-bit binary, `gamedata/dodhooks.txt`, and
> `scripting/include/dodhooks.inc`. The version number above is detected
> automatically from the latest GitHub release — no manual update needed.
---

## About

DODHooks is a SourceMod extension for **Day of Defeat: Source** that provides:

- **Detours** (hooks) for key game functions: voice commands, class joining, helmet popping, respawning, wave time, winning team, round state, player state, and bomb target state.
- **Natives** for controlling player classes, control point icons, round timers, and game rules from SourcePawn plugins.
- **Forwards** (hooks) that allow plugins to intercept and modify game events.

This version is a maintained fork that:

- Supports **SourceMod 1.12 and 1.13**
- Supports **Metamod:Source 1.12 and 2.0**
- Compiles for **both 32-bit (x86) and 64-bit (x86_64)** architectures
- Works on **Windows and Linux**
- Uses the **latest AMBuild 2.x** build system
- Fixes server crash issues present in older versions
- Uses modern C++17 compiler flags
- **Auto-loads via include file** — no manual `sm exts load` needed (fixed in v1.6.1)

## Requirements

| Dependency | Version | Notes |
|------------|---------|-------|
| SourceMod | 1.12 / 1.13 | Source code required for building |
| Metamod:Source | 1.12 / 2.0 | Source code required for building |
| AMBuild | 2.2+ | Python-based build system |
| Python | 3.8+ | Required for AMBuild |
| Compiler | GCC 9+ / Clang 10+ / MSVC 2019+ | C++17 support required |

## Build Dependencies

### Linux

```bash
# Debian/Ubuntu
sudo apt-get update
sudo apt-get install -y build-essential clang-22 python3 python3-pip git

# Install AMBuild
pip3 install --upgrade git+https://github.com/alliedmodders/ambuild.git
```

### Windows

```powershell
# Install Python 3.12+ from python.org
# Install Visual Studio 2019+ (Community Edition is fine)
# Install Git from git-scm.com
# Install AMBuild
python -m pip install --upgrade git+https://github.com/alliedmodders/ambuild.git
```

## Building

Both 32-bit (x86) and 64-bit (x64) binaries are produced in a **single run**
and staged into a release-ready `dist/` folder (the extension auto-loads via
its SourcePawn include file).

### Quick Start (Linux)

```bash
# Clone the repository
git clone https://github.com/kittenks/dodhooks.git
cd dodhooks

# Clone dependencies
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/metamod-source.git mmsource
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod

# Build BOTH 32-bit + 64-bit, then stage into dist/ and create a .tar.gz
./build.sh
```

The result is `dist/addons/sourcemod/extensions/` containing the 32-bit
`.so`, an `x64/` subfolder with the 64-bit `.so`, the bundled
`dodhooks.inc` include, and `dist/addons/sourcemod/gamedata/dodhooks.txt`.

### Windows Build

```powershell
# Open "Developer Command Prompt for VS" (or any terminal; the script
# locates vcvarsall.bat automatically).

git clone https://github.com/kittenks/dodhooks.git
cd dodhooks

git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/metamod-source.git mmsource
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod

# Build BOTH 32-bit + 64-bit, then stage into dist/ and create a .zip
build.bat
```

### Manual / Advanced (raw AMBuild)

If you prefer to build a single architecture by hand:

```bash
mkdir build && cd build
python3 ../configure.py \
    --sm-path ../sourcemod \
    --mms-path ../mmsource \
    --arch=x86 \
    --enable-optimize
ambuild
# 64-bit: use --arch=x64
```

> **Note:** the configure argument is `--arch=x86` / `--arch=x64`
> (not `--target`). The SDK selection is `--sdks=dods`.

### Generate Visual Studio Project (Windows)

```powershell
python ..\configure.py `
    --sm-path ..\sourcemod `
    --mms-path ..\mmsource `
    --arch=x86 `
    --enable-optimize `
    --gen=vs
```

## Docker Build

A Dockerfile and a one-command wrapper (`build_linux_docker.sh`) are provided
for easy, reproducible Linux builds (no host toolchain needed):

```bash
# Build inside the official AlliedModders container (clones deps into deps/):
./build_linux_docker.sh

# Or manually:
docker build -t dodhooks-builder .
docker run --rm -v $(pwd):/work/dodhooks -w /work/dodhooks dodhooks-builder \
    bash -c "pip3 install --upgrade ambuild; ./build.sh"
```

## Installation

After building (or downloading a release archive), copy the contents to your
game server's root directory:

```
addons/
└── sourcemod/
    ├── extensions/
    │   ├── dodhooks.ext.2.dods.dll        (Windows 32-bit)
    │   ├── dodhooks.ext.2.dods.so         (Linux 32-bit)
    │   └── x64/
    │       ├── dodhooks.ext.2.dods.dll    (Windows 64-bit)
    │       └── dodhooks.ext.2.dods.so     (Linux 64-bit)
    ├── gamedata/
    │   └── dodhooks.txt
    └── scripting/
        └── include/
            └── dodhooks.inc
```

### Loading the extension
**Warning** Urgently Needs Fixing Extension Loading Issue 
At present, there is no definitive solution to the issue of dodhooks failing to load properly; the temporary workaround is to enter the command sm exts load dodhooks.ext.2.dods in server.cfg.
The extension is **auto-loaded** automatically: any plugin that
`#include <dodhooks>` triggers SourceMod to load `dodhooks.ext` (resolved to
`dodhooks.ext.2.dods`) at runtime. This is wired up by the
`public Extension __ext_dodhooks` block inside `dodhooks.inc`, so **no**
`.autoload` marker file or manual command is needed.

> **Important (v1.6.1 fix):** The `file` field in `dodhooks.inc` must be
> `"dodhooks.ext"` (the base name). SourceMod automatically appends the game
> suffix `.2.dods` and platform extension. Do **not** write
> `"dodhooks.ext.2.dods"` — that breaks auto-load resolution.

To load it explicitly (e.g. for debugging), use:

```
sm exts load dodhooks
```

> Do **not** use `meta load` — that command is for Metamod:Source plugins and
> will report "File type not supported" for a `.dll`/`.so` extension.

## Available Natives

| Native | Description |
|--------|-------------|
| `DOD_GetPlayerClass(client)` | Get a player's current class |
| `DOD_SetPlayerClass(client, class)` | Set a player's current class |
| `DOD_GetDesiredPlayerClass(client)` | Get desired player class |
| `DOD_SetDesiredPlayerClass(client, class)` | Set desired player class |
| `DOD_PopHelmet(client, velocity[3], origin[3])` | Force a helmet to pop off |
| `DOD_SetNumControlPoints(num)` | Set number of control points |
| `DOD_PrecacheCPIcon(material)` | Precache a CP icon material |
| `DOD_SetCPIcons(index, ...)` | Set icons for a control point |
| `DOD_SetCPVisible(index, visible)` | Show/hide a control point |
| `DOD_PauseTimer(timer)` | Pause a round timer |
| `DOD_ResumeTimer(timer)` | Resume a round timer |
| `DOD_SetTimeRemaining(timer, seconds)` | Set timer remaining time |
| `DOD_GetTimeRemaining(timer)` | Get timer remaining time |
| `DOD_RespawnPlayer(client, useClass)` | Force respawn a player |
| `DOD_AddWaveTime(team, delay)` | Add wave time for a team |
| `DOD_SetWinningTeam(team)` | Set the winning team |
| `DOD_SetRoundState(state)` | Set the round state |
| `DOD_SetPlayerState(client, state)` | Set a player's state |
| `DOD_SetBombTargetState(entity, state)` | Set bomb target state |

## Available Forwards (Hooks)

| Forward | Description |
|---------|-------------|
| `OnVoiceCommand(client, &voiceCommand)` | Called when a voice command is used |
| `OnJoinClass(client, &playerClass)` | Called when a player joins a class |
| `OnPopHelmet(client, velocity[3], origin[3])` | Called when a helmet pops off |
| `OnPlayerRespawn(client)` | Called when a player is about to respawn |
| `OnAddWaveTime(team, &delay)` | Called when wave time is added |
| `OnSetWinningTeam(team)` | Called when the winning team is set |
| `OnEnterRoundState(&roundState)` | Called when round state changes |
| `OnEnterPlayerState(client, &playerState)` | Called when player state changes |
| `OnEnterBombTargetState(entity, &bombState)` | Called when bomb target state changes |

## Enumerations

The include file provides the following enumerations for type-safe development:

| Enum | Values |
|------|--------|
| `DODPlayerClass` | Random, None, Rifleman, Assault, Support, Sniper, Machinegunner, Rocket |
| `DODRoundState` | RoundInit, PreGame, StartGame, PreRound, RoundRunning, AlliesWin, AxisWin, Restart, GameOver |
| `DODPlayerState` | Active, Welcome, PickingTeam, PickingClass, DeathAnim, ObserverMode |
| `DODBombTargetState` | Inactive, Active, Armed |
| `DODVoiceCommand` | 39 voice command IDs (Attack, Hold, Move, Medic, Grenade, Sniper, etc.) |

## Changes from Original

- **SourceMod 1.12/1.13 compatibility** - Updated APIs and build system
- **64-bit support** - Compiles and runs on 64-bit servers
- **Modern C++17** - Updated compiler flags and standards
- **Fixed crashes** - Addressed several server crash scenarios:
  - NULL pointer checks in detour callbacks
  - Proper stack alignment for 64-bit ThisCall conventions
  - Safer gamedata signature resolution with better error messages
  - Protected against invalid entity references
- **Improved error handling** - Better error messages for missing gamedata or signatures
- **GitHub Actions CI** - Automated builds for 4 platforms (Win/Linux x86/x64)
- **Docker support** - Reproducible builds via containerization
- **Fixed include auto-load (v1.6.1)** - Corrected `file` field to base name `"dodhooks.ext"` so the extension auto-loads without manual `sm exts load`
- **Restored include features (v1.6.1)** - `MarkNativeAsOptional` fallback, `AUTOLOAD_EXTENSIONS`/`REQUIRE_EXTENSIONS` conditionals, all enums (RoundState/PlayerState/BombTargetState/VoiceCommand), `MAX_CONTROL_POINTS`, `IsPlayerClassValid()` stock
- **Stronger typing (v1.6.1)** - `SetRoundState`/`SetPlayerState`/`SetBombTargetState` use enum-typed parameters

## Changelog

See [CHANGELOG.md](CHANGELOG.md) for the full version history.

## License

GPL v2 - See [LICENSE](LICENSE) for details.

## Credits

- **Andersso** - Original author
- **ChesterSmitty** - Previous maintainer
- **Apfelwurm** - CI improvements
- **DNA-styx** - Gamedata file
- **Kittenks** - Current maintainer (1.12/1.13 updates, build & packaging) - https://github.com/kittenks/dodhooks
- **AlliedModders** - SourceMod, Metamod:Source, AMBuild

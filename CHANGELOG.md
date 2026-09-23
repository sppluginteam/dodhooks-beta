# Changelog

All notable changes to DODHooks are documented here.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.6.0/),
and this project follows SourceMod-style versioning where the extension is
built against SourceMod 1.12.

## [1.6.3] - 2026-09-24

### Verified (no change required)

- **Gamedata re-validated byte-for-byte against the current server binary**
  (`server.dll`, PE timestamp `0x6AAA1368`, 2026-09-16):
  - All 9 Windows x86 signatures match **exactly once** each — no ambiguity,
    no drift. `CreateStandardEntities`, `VoiceCommand`, `JoinClass`,
    `PopHelmet`, `SetWinningTeam`, `DODRespawn`, `AddWaveTime`, `RoundState`,
    `PlayerState`, `BombTargetState` are all correct.
  - The `g_pObjectiveResource` offset `67` was disassembled and confirmed: it
    lands on the 4-byte operand of the `mov [g_pObjectiveResource], eax`
    instruction belonging to the `dod_objective_resource` creation block
    inside `CreateStandardEntities`. (Offset 40 is `g_pPlayerManager` —
    do NOT "fix" 67 to 40.)
  - Conclusion: **signatures and offsets did not need updating.**

### Fixed

- **Critical: a single missing detour no longer aborts the whole extension.**
  `SetupDetours()` used to `return false` (failing `SDK_OnLoad`, i.e. the
  extension never appears in `sm exts list`) if *any one* of the 9 signatures
  failed to resolve. Failures are now non-fatal: the extension loads, prints
  `DODHooks: loaded - N/9 detours active`, and names every inactive detour.
- **Critical: round state changes were being swallowed.** `RoundState` kept a
  `static iPreviousRoundState` cache and, on a repeated value, returned
  *without* calling the original `CDODGameRules::State_Enter()`. That silently
  dropped legal transitions (and only updated the cache when a plugin returned
  `Plugin_Handled`), which can stall the round. The cache is gone; every state
  change now reaches the original code unless a plugin blocks it.
- **Critical: forward registration/teardown ordering.**
  - Forwards are now created in `SDK_OnLoad` **before** any detour is enabled
    (a detour callback dereferences its forward pointer, so enabling a detour
    before the forward exists is a null-deref waiting to happen).
  - `SDK_OnUnload` now tears down detours **before** releasing forwards
    (previously it released forwards first while detours were still live).
- **`SDK_OnAllLoaded` no longer aborts when bintools is unavailable.** It used
  to `return` early, which skipped offset resolution *and*
  `InitializeValveGlobals()`. A missing bintools now only disables the
  call-based natives.
- **Unresolved send-properties no longer corrupt memory.** `GetSendPropOffset`
  returned `-1` (0xFFFFFFFF) and natives happily wrote through it. It now
  returns `INVALID_SEND_PROP_OFFSET`, every native validates the offset first
  and returns a clean plugin error, and unresolved groups are reported in the
  console at load time.
- **`DOD_SetNumControlPoints` had no range check** and would write any value
  into the objective resource. Now validated against `0..MAX_CONTROL_POINTS`.
- **`DODPlayerClass` off-by-one in `dodhooks.inc`.** The include declared
  `DODClass_None = 0, DODClass_Rifleman = 1 ... DODClass_Rocket = 6`, but the
  game (and `extension.h`) use `None = -1, Rifleman = 0 ... Rocket = 5`
  (verified: `HandleCommand_JoinClass` tests the incoming class against `-2`
  for random). Every class was shifted by one. The include now matches the
  game, and both sides carry a comment warning against regressing it.

### Changed

- **Corrected: `bin/x64/server.dll` IS a real DoD:S server binary.** A previous
  note claimed DoD:S has no 64-bit server. That was wrong — the x64 DLL exports
  the same 8 symbols as the x86 one (`CDODBombDispenser`, `CAreaCapture`,
  `CWeaponDODBase`, `CreateInterface`, `cvar`), just with x64 calling
  conventions, and contains the same server-side strings
  (`dod_objective_resource`, `DODRoundState: entering`, `BOMB_TARGET_ACTIVE`).
  It is a complete, usable 64-bit server binary.
- **`windows64` signatures are now shipped** as a forward-looking measure, so
  the extension keeps working if/when Valve enables the 64-bit DoD:S server.
  Resolved and shipped: `CreateStandardEntities`, `DODRespawn`, `AddWaveTime`,
  `RoundState`, `PlayerState`, `BombTargetState`.
  `VoiceCommand`, `JoinClass`, `PopHelmet`, `SetWinningTeam` have multiple
  near-identical x64 candidates and could not be uniquely resolved — they are
  deliberately left out rather than risk hooking the wrong function.
- **`g_pObjectiveResource` resolution rewritten for x64.** x86 stores an
  absolute address (`mov [abs32], eax`), but x64 uses RIP-relative addressing
  (`mov [rip+disp32], rax`), so the 4 bytes at the offset are a *displacement*,
  not a pointer. The x64 offset now points at the displacement field and
  `vglobals.cpp` computes `target = pAddress + offset + 4 + disp32`.
  (`windows64` offset 75 = second store = objective resource; the first store,
  offset 39, is `g_pPlayerManager`.)

### Added

- **`linux64` keys** in `dodhooks.txt`. 64-bit Linux servers look up the
  `linux64` key first; without it the symbol lookups fail silently.
- **Diagnostic natives**: `DOD_IsAvailable()`, `DOD_GetDetourCount()`,
  `DOD_GetDetourTotal()`, `DOD_IsObjectiveResourceReady()`.
- **`dodhooks_test.sp` diagnostics**: `dodhooks_test` now prints detour
  health (`Detours active: 9/9`), objective-resource readiness, and a class
  enum sanity check, and tells you what each failure pattern means.
  `dodhooks_status` reports every forward's fire count and warns when detours
  are installed but nothing has fired.

## [1.6.1] - 2026-08-24

### Fixed

- **Critical: Extension auto-load was broken.** The `dodhooks.inc` header
  declared `file = "dodhooks.ext.2.dods"` in the `public Extension` block,
  but SourceMod's auto-loader expects the **base name** `file = "dodhooks.ext"`
  and appends the game suffix (`.2.dods`) and platform extension (`.so`/`.dll`)
  automatically. The over-specified filename caused resolution to fail, so the
  extension would not auto-load and required manual `sm exts load dodhooks`.
  Fixed by reverting to `file = "dodhooks.ext"` — matching the 2015 psychonic
  original and all standard SourceMod extensions (sdkhooks, dhooks, etc.).

### Added

- `__ext_dodhooks_SetNTVOptional()` — marks all 19 natives as optional when
  `REQUIRE_EXTENSIONS` is not defined, so plugins load gracefully even if the
  extension is unavailable (natives throw a runtime error when called instead
  of blocking plugin load). Restored from the 2015 psychonic include.
- Conditional `autoload` / `required` via `AUTOLOAD_EXTENSIONS` and
  `REQUIRE_EXTENSIONS` defines, allowing plugin authors to control extension
  loading behavior at compile time. Restored from the 2015 psychonic include.
- Enumerations restored from the 2015 psychonic include:
  - `DODRoundState` — round states (RoundInit, PreGame, StartGame, PreRound,
    RoundRunning, AlliesWin, AxisWin, Restart, GameOver)
  - `DODPlayerState` — player states (Active, Welcome, PickingTeam,
    PickingClass, DeathAnim, ObserverMode)
  - `DODBombTargetState` — bomb target states (Inactive, Active, Armed)
  - `DODVoiceCommand` — all 39 voice command IDs (Attack, Hold, Move, Medic,
    Grenade, Sniper, etc.)
- `#define MAX_CONTROL_POINTS 8` constant.
- `stock bool IsPlayerClassValid(DODPlayerClass playerClass)` utility function.

### Changed

- `DOD_SetRoundState`, `DOD_SetPlayerState`, and `DOD_SetBombTargetState`
  now use strongly-typed enum parameters (`DODRoundState`, `DODPlayerState`,
  `DODBombTargetState`) instead of raw `int`, providing compile-time type
  checking for plugin authors.

## [1.6.0] - 2026-08-23

### Added

- Auto-load via the SourcePawn include: `dodhooks.inc` now declares
  `public Extension __ext_dodhooks`, so any plugin that
  `#include <dodhooks>` makes SourceMod automatically load `dodhooks.ext`
  (`dodhooks.ext.2.dods`) at runtime. No `.autoload` marker file or manual
  `sm exts load` is required.
- Single-command builds for both architectures:
  - `build.bat` (Windows) builds x86 + x64 and produces
    `DODHooks-<ver>-sm1.12-windows.zip`.
  - `build.sh` (Linux) builds x86 + x64 and produces
    `DODHooks-<ver>-sm1.12-linux.tar.gz`.
  - `build_linux_docker.sh` runs the Linux build inside the official
    AlliedModders build container.
- Release packaging:
  - 32-bit binary in `extensions/` (default).
  - 64-bit binary in `extensions/x64/` subfolder.
  - GameData copied from repository `gamedata/dodhooks.txt`.
  - `dodhooks.inc` packaged into `scripting/include/`.
- GitHub Actions workflow that builds Windows (x86+x64) and Linux (x86+x64)
  and publishes three release archives on tags:
  - `DODHooks-<tag>-sm1.12-windows.zip`
  - `DODHooks-<tag>-sm1.12-linux.zip`
  - `DODHooks-<tag>-source.zip`
- Bilingual documentation (`README.md` / `README_zh.md`) and this changelog.

### Fixed

- Correct `configure.py` flags: `--arch=x86|x64` (was `--target`) and
  `--sdks=dods` (was `dod`) across all build scripts, `Dockerfile` and CI.
- `PackageScript` now copies GameData from the repository's
  `gamedata/dodhooks.txt` and the include from
  `sourcemod/scripting/include/dodhooks.inc` (previously referenced the wrong
  paths).
- Unified dependency versions: build scripts, CI, and setup script now all use
  Metamod:Source `1.12-dev` branch (previously Windows builds used `1.11-dev`).

### Changed

- Consolidated build scripts; removed the broken Windows→Linux cross-compile
  attempt and redundant `docker/`, `scripts/`, `build_linux.sh` files.
- GameData default section documented as targeting Day of Defeat: Source
  (game folder `dod`).

### Compatibility

- SourceMod 1.12 / 1.13
- Metamod:Source 1.12 / 2.0
- Windows and Linux, 32-bit and 64-bit

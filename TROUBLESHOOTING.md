# DODHooks Troubleshooting

> 中文版：[TROUBLESHOOTING_zh.md](TROUBLESHOOTING_zh.md)
>
> Project: <https://github.com/sppluginteam/dodhooks-beta>

## 0. 32-bit or 64-bit? Let's settle this first

**`bin/x64/server.dll` IS a real DoD:S server binary, not a leftover client DLL.**

Proof — it exports exactly the same 8 symbols as the x86 `bin/server.dll`,
just with x64 calling conventions:

```
x86: ?Touch@CDODBombDispenser@@UAEXPAVCBaseEntity@@@Z
x64: ?Touch@CDODBombDispenser@@UEAAXPEAVCBaseEntity@@@Z
     ?Smack@CWeaponDODBase@@QEAAXXZ
     ?AreaTouch@CAreaCapture@@AEAAXPEAVCBaseEntity@@@Z
     CreateInterface / cvar
```

`CDODBombDispenser`, `CAreaCapture` and `CWeaponDODBase` are all DoD:S
server-only classes. The x64 DLL also contains server-side strings such as
`dod_objective_resource`, `DODRoundState: entering`, `BOMB_TARGET_ACTIVE` and
`Player sent bad joinclass syntax`. It is a **complete, usable 64-bit server
binary**.

### So why are we still using 32-bit signatures?

- Current DoD:S deployments still run the **32-bit server** (there is no
  publicly usable 64-bit srcds launch path yet), so SourceMod loads
  `bin/server.dll`.
- Therefore the **`windows` column is the one actually in use** today.

**However, `windows64` signatures are shipped since 1.6.3** as a
forward-looking measure — if/when Valve enables the 64-bit server, they are
ready to go (assuming 64-bit SourceMod/Metamod builds exist by then).

### `g_pObjectiveResource` resolution differs on x64

This is an easy trap. On x86 the instruction stores an absolute address, so a
plain offset dereference works:

```
x86: A3 xx xx xx xx              mov [abs32], eax
```

On x64 the code uses RIP-relative addressing, so those 4 bytes are a
**relative displacement**, not a pointer:

```
x64: 48 89 05 71 52 3c 00        mov [rip+0x3c5271], rax
```

The x64 offset therefore points at the displacement field and the extension
computes:

```
target = pAddress + offset + 4 + disp32
```

In `dodhooks.txt`, `windows64` offset `75` is the displacement field of the
**second** store — the one saving the freshly created `dod_objective_resource`.
(The first store, offset `39`, is `g_pPlayerManager`.)

### Confidence level of the `windows64` signatures

Uniquely resolved via string anchors and shipped: `CreateStandardEntities`,
`DODRespawn`, `AddWaveTime`, `RoundState`, `PlayerState`, `BombTargetState`.

`VoiceCommand`, `JoinClass`, `PopHelmet` and `SetWinningTeam` have multiple
near-identical x64 candidates (e.g. the `dod_round_win` string is referenced by
6 different functions) and could **not** be uniquely resolved. They are
deliberately left out — better to report "detour inactive" than to hook the
wrong function and crash the server.

## 1. Extension fails to load / missing from `sm exts list`

Check in this order:

1. **`meta list`** — without Metamod:Source nothing else matters.
2. **File name** — must be
   `addons/sourcemod/extensions/dodhooks.ext.2.dods.dll`
   (`.so` on Linux; 64-bit Linux goes under `extensions/x64/`).
3. **`dodhooks.txt`** must be in `addons/sourcemod/gamedata/`.
4. **`sm exts load dodhooks`** — load manually and read the exact console error.
5. Check the newest error log in `addons/sourcemod/logs/`.

Since 1.6.3, **a single failing signature no longer aborts the whole
extension** — it loads anyway and prints:

```
DODHooks: loaded - 9/9 detours active
```

or

```
DODHooks: loaded - 7/9 detours active
DODHooks: 2 detour(s) inactive - check addons/sourcemod/gamedata/dodhooks.txt
DODHooks: Warning - detour "PopHelmet" not available (...)
```

**The `N/9` number is your fastest diagnostic:**

| Value | Meaning |
|---|---|
| `9/9` | All hooks installed — the problem is elsewhere (plugin / game logic). |
| `0/9` | Gamedata loaded but nothing matched — `server.dll` version mismatch (game updated, or wrong binary). |
| in between | Individual signatures broke — fix the keys named in the console. |

## 2. Extension loaded, but the plugin does nothing

Run `dodhooks_test`:

- `Detours active: 9/9` with all `Forward Fire Counts` at 0
  → hooks are installed but never triggered. Make sure you actually performed
  the triggering action (change class, use a voice command, respawn).
- `ObjectiveResource ready: NO` **after a map has loaded**
  → the `CreateStandardEntities` signature or the `g_pObjectiveResource`
  offset is broken; every control-point native will report
  "ObjectiveResource not available".

## 3. Player classes are off by one (Rifleman reported as Assault, etc.)

1.6.3 fixed an off-by-one in the `dodhooks.inc` class enum. Correct values:

**Random = -2, None = -1, Rifleman = 0, Assault = 1, Support = 2,
Sniper = 3, Machinegunner = 4, Rocket = 5**

`dodhooks_test` runs an enum self-check and prints
`!! Class enum is off by one` if the values are wrong.

## 4. Does the gamedata need updating?

**Not for the current binary (server.dll dated 2026-09-16).** Verified
byte-for-byte:

- All 9 Windows x86 signatures match **exactly once**.
- Offset 67 was disassembled and confirmed to land on the 4-byte operand of
  the `mov [g_pObjectiveResource], eax` instruction inside
  `CreateStandardEntities`.

Note that offset 40 is `g_pPlayerManager` — **do not** "fix" 67 to 40.

When Valve ships another server update, `dodhooks_test`'s
`Detours active: N/9` tells you immediately which signature broke.

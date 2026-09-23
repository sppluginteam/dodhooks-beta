# DODHooks 故障排查

> English version: [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
>
> 项目地址：<https://github.com/sppluginteam/dodhooks-beta>

## 0. 32 位还是 64 位？先把这件事说清楚

**`bin/x64/server.dll` 确实是 DoD:S 的服务端二进制，不是客户端残留。**

证据 —— 它导出的 8 个符号与 x86 版 `bin/server.dll` 完全相同，只是调用约定
换成了 x64 写法：

```
x86: ?Touch@CDODBombDispenser@@UAEXPAVCBaseEntity@@@Z
x64: ?Touch@CDODBombDispenser@@UEAAXPEAVCBaseEntity@@@Z
     ?Smack@CWeaponDODBase@@QEAAXXZ
     ?AreaTouch@CAreaCapture@@AEAAXPEAVCBaseEntity@@@Z
     CreateInterface / cvar
```

`CDODBombDispenser`、`CAreaCapture`、`CWeaponDODBase` 全是 DoD:S 服务端
专有的类。x64 版里同样包含 `dod_objective_resource`、`DODRoundState: entering`、
`BOMB_TARGET_ACTIVE`、`Player sent bad joinclass syntax` 等服务端字符串。
它是一个**完整可用的 64 位服务端二进制**。

### 那为什么现在还在用 32 位签名？

- 目前实际部署仍是 **32 位服务端**（还没有对外可用的 64 位 srcds 启动路径），
  SourceMod 加载的是 `bin/server.dll`。
- 所以 **`windows` 这一列才是当前真正生效的**。

**但 1.6.3 已预置 `windows64` 签名**，为将来做准备 —— 若 Valve 启用 64 位
服务端，这套签名可直接用（前提是届时 SourceMod/Metamod 也有 64 位构建）。

### x64 上 g_pObjectiveResource 的获取方式变了

这是个容易踩的坑。x86 那条指令存的是绝对地址，给个 offset 直接解引用就行：

```
x86: A3 xx xx xx xx              mov [abs32], eax
```

但 x64 用 RIP 相对寻址，那 4 字节是**相对位移**而不是指针：

```
x64: 48 89 05 71 52 3c 00        mov [rip+0x3c5271], rax
```

所以 x64 的 offset 指向位移字段，由扩展自己算：

```
target = pAddress + offset + 4 + disp32
```

`dodhooks.txt` 里 `windows64` 的 `75` 就是**第二条** store 的位移字段 —— 即
保存新建 `dod_objective_resource` 的那条。（第一条 `39` 是 `g_pPlayerManager`。）

### windows64 签名的置信度

已通过字符串锚点唯一确定并提供签名：`CreateStandardEntities`、`DODRespawn`、
`AddWaveTime`、`RoundState`、`PlayerState`、`BombTargetState`。

`VoiceCommand`、`JoinClass`、`PopHelmet`、`SetWinningTeam` 在 x64 中有多个
形态接近的候选函数（例如 `dod_round_win` 字符串同时被 6 个函数引用），
**无法唯一确定**。这几个故意不提供签名 —— 宁可让那一个 detour 报"未启用"，
也不冒险钩错函数把服务器搞崩。

## 1. 扩展加载失败 / `sm exts list` 里看不到

按这个顺序查：

1. **`meta list`** —— 没有 Metamod:Source 一切免谈。
2. **文件名**：必须是 `addons/sourcemod/extensions/dodhooks.ext.2.dods.dll`
   （Linux 是 `.so`，64 位 Linux 放 `extensions/x64/`）。
3. **`dodhooks.txt`** 必须在 `addons/sourcemod/gamedata/`。
4. **`sm exts load dodhooks`** 手动加载，看控制台回显的具体错误。
5. 看 `addons/sourcemod/logs/` 下最新的 error log。

从 1.6.3 起，**单条签名解析失败不再导致整个扩展加载不出来** —— 扩展会照常
加载并打印：

```
DODHooks: loaded - 9/9 detours active
```

或

```
DODHooks: loaded - 7/9 detours active
DODHooks: 2 detour(s) inactive - check addons/sourcemod/gamedata/dodhooks.txt
DODHooks: Warning - detour "PopHelmet" not available (...)
```

**`N/9` 这个数字就是最快的定位依据**：

| 数值 | 含义 |
|---|---|
| `9/9` | 全部钩子就位，问题在别处（插件、游戏逻辑）。 |
| `0/9` | gamedata 读到了但没一条匹配，说明 `server.dll` 版本对不上。 |
| 介于两者之间 | 个别签名失效，按控制台点名的 key 去修。 |

## 2. 扩展加载了，但插件没反应

跑 `dodhooks_test`：

- `Detours active: 9/9` + `Forward Fire Counts` 全是 0
  → 钩子装上了但没被触发，确认你确实做了触发动作（换兵种、语音、重生）。
- `ObjectiveResource ready: NO`（**地图已加载后**仍为 NO）
  → `CreateStandardEntities` 签名或 `g_pObjectiveResource` 偏移失效，
  控制点类 native 会全部报 "ObjectiveResource not available"。

## 3. 兵种值不对（步枪兵变成突击兵等）

1.6.3 修正了 `dodhooks.inc` 兵种枚举整体错位 1 的问题。正确值：

**Random = -2，None = -1，Rifleman = 0，Assault = 1，Support = 2，
Sniper = 3，Machinegunner = 4，Rocket = 5**

`dodhooks_test` 会做枚举自检，发现错位会打印 `!! Class enum is off by one`。

## 4. gamedata 到底要不要改？

**当前（2026-09-16 的 server.dll）不需要改。** 已逐字节复核：

- 9 条 Windows x86 签名全部唯一命中；
- 偏移 67 经反汇编确认落在 `CreateStandardEntities` 中
  `mov [g_pObjectiveResource], eax` 的操作数上。

注意偏移 40 是 `g_pPlayerManager`，**不要**把 67 改成 40。

哪天 Valve 再更新服务端，用 `dodhooks_test` 的 `Detours active: N/9`
就能立刻知道哪条签名失效了。

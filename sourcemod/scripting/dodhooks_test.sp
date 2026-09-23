/**
 * DODHooks Extension Test & Diagnostics Plugin
 *
 * Purpose:
 * - Verify that the DODHooks extension auto-loads correctly (no manual "sm exts load" needed)
 * - Report HOW MANY detours were actually installed vs. attempted
 * - Test that all natives are callable and return sane results
 * - Verify that forwards (detour hooks) fire correctly
 * - Provide console commands:
 *       dodhooks_test    - full diagnostic
 *       dodhooks_status  - forward firing statistics
 *
 * Usage:
 * 1. Place dodhooks.inc in addons/sourcemod/scripting/include/
 * 2. Compile this plugin
 * 3. Place .smx in addons/sourcemod/plugins/
 * 4. Restart the server (do NOT manually load the extension)
 * 5. Check server console / SourceMod logs for [DODHOOKS-TEST] messages
 * 6. In console, type: dodhooks_test
 *
 * Expected result on success:
 * [DODHOOKS-TEST] Extension auto-load: OK
 * [DODHOOKS-TEST] Detours active: 9/9
 * [DODHOOKS-TEST] All natives resolved: OK
 * ... and when a player joins / uses voice command / respawns,
 * forward callback messages should appear.
 *
 * Interpreting a failure:
 *   "Extension AUTO-LOAD: FAILED"      -> binary missing / wrong name / MM:Source issue
 *   "Detours active: 0/9"              -> gamedata resolved the file but no signature
 *                                         matched; check gamedata/dodhooks.txt and the
 *                                         server.dll build date
 *   "Detours active: 7/9"              -> some signatures matched; the console lists which
 *   "ObjectiveResource ready: NO"      -> normal before a map loads; after map load it
 *                                         means CreateStandardEntities / the offset failed
 */
#pragma semicolon 1
#pragma newdecls required

#include <sourcemod>
#include <dodhooks>

public Plugin myinfo =
{
	name = "DODHooks Extension Test",
	author = "Test",
	description = "Verifies DODHooks extension auto-load, natives, and forwards",
	version = "1.1",
	url = ""
};

/* Track whether extension is available */
bool g_bExtAvailable = false;

/* Counters for forward firing */
int g_iVoiceCmdCount = 0;
int g_iJoinClassCount = 0;
int g_iPopHelmetCount = 0;
int g_iRespawnCount = 0;
int g_iRoundStateCount = 0;
int g_iPlayerStateCount = 0;
int g_iAddWaveTimeCount = 0;
int g_iSetWinningTeamCount = 0;
int g_iBombTargetStateCount = 0;

public void OnPluginStart()
{
	/* Check if the extension loaded by testing if a native exists.
	 * GetPlayerClass is a safe read-only native that won't crash
	 * even if called with an invalid client (it throws a native error).
	 * We use GetFeatureStatus to check extension presence instead.
	 */
	g_bExtAvailable = (GetFeatureStatus(FeatureType_Native, "DOD_GetPlayerClass") == FeatureStatus_Available);

	if (g_bExtAvailable)
	{
		PrintToServer("[DODHOOKS-TEST] ========================================");
		PrintToServer("[DODHOOKS-TEST] DODHooks extension AUTO-LOAD: OK");
		PrintToServer("[DODHOOKS-TEST] Extension loaded automatically via include file.");
		PrintToServer("[DODHOOKS-TEST] No manual 'sm exts load' was needed.");
		PrintToServer("[DODHOOKS-TEST] ========================================");
		LogMessage("DODHooks extension auto-load: OK");
	}
	else
	{
		PrintToServer("[DODHOOKS-TEST] ========================================");
		PrintToServer("[DODHOOKS-TEST] DODHooks extension AUTO-LOAD: FAILED");
		PrintToServer("[DODHOOKS-TEST] Extension did NOT load automatically.");
		PrintToServer("[DODHOOKS-TEST] Check:");
		PrintToServer("[DODHOOKS-TEST] 1. dodhooks.ext.2.dods.dll is in addons/sourcemod/extensions/");
		PrintToServer("[DODHOOKS-TEST] 2. dodhooks.txt is in addons/sourcemod/gamedata/");
		PrintToServer("[DODHOOKS-TEST] 3. dodhooks.inc has file = \"dodhooks.ext\" (NOT \"dodhooks.ext.2.dods\")");
		PrintToServer("[DODHOOKS-TEST] 4. Metamod:Source is installed and loaded (meta list)");
		PrintToServer("[DODHOOKS-TEST] 5. Run 'sm exts list' to see if it appears");
		PrintToServer("[DODHOOKS-TEST] 6. Check addons/sourcemod/logs/ for load failures");
		PrintToServer("[DODHOOKS-TEST] ========================================");
		LogError("DODHooks extension auto-load: FAILED - natives not available");
	}

	/* Register diagnostic console command */
	RegConsoleCmd("dodhooks_test", Cmd_Test, "Run DODHooks extension diagnostic tests");
	RegConsoleCmd("dodhooks_status", Cmd_Status, "Show DODHooks forward firing statistics");
}

public void OnAllPluginsLoaded()
{
	if (g_bExtAvailable)
	{
		PrintToServer("[DODHOOKS-TEST] All plugins loaded. DODHooks forwards are active.");
		PrintToServer("[DODHOOKS-TEST] Trigger these in-game to verify forwards fire:");
		PrintToServer("[DODHOOKS-TEST] - Use a voice command (Z/X/C menus) -> OnVoiceCommand");
		PrintToServer("[DODHOOKS-TEST] - Change class -> OnJoinClass");
		PrintToServer("[DODHOOKS-TEST] - Get shot in the head -> OnPopHelmet");
		PrintToServer("[DODHOOKS-TEST] - Respawn -> OnPlayerRespawn");
		PrintToServer("[DODHOOKS-TEST] - Round starts/ends -> OnEnterRoundState");
	}
}

/**
 * Console command: run full diagnostic
 */
public Action Cmd_Test(int client, int args)
{
	PrintToServer("[DODHOOKS-TEST] ===== Running Diagnostic =====");
	PrintToServer("[DODHOOKS-TEST] Extension available: %s", g_bExtAvailable ? "YES" : "NO");

	if (!g_bExtAvailable)
	{
		PrintToServer("[DODHOOKS-TEST] ABORT: Extension not loaded, cannot test natives.");
		return Plugin_Handled;
	}

	/* Test 0: detour health - the single most useful number when the
	 * extension "loads" but nothing happens. */
	PrintToServer("[DODHOOKS-TEST] --- Detour Status ---");
	int active = DOD_GetDetourCount();
	int total = DOD_GetDetourTotal();
	PrintToServer("[DODHOOKS-TEST] Detours active: %d/%d", active, total);
	if (total > 0 && active == 0)
	{
		PrintToServer("[DODHOOKS-TEST] !! No detour could be installed - gamedata signatures");
		PrintToServer("[DODHOOKS-TEST] !! do not match this server.dll. Check gamedata/dodhooks.txt.");
	}
	else if (active < total)
	{
		PrintToServer("[DODHOOKS-TEST] !! %d detour(s) failed - see console lines starting with", total - active);
		PrintToServer("[DODHOOKS-TEST] !! 'DODHooks: Warning - detour' above.");
	}

	/* Test 0b: objective resource */
	PrintToServer("[DODHOOKS-TEST] ObjectiveResource ready: %s",
		DOD_IsObjectiveResourceReady() ? "YES" : "NO (expected before a map loads)");

	/* Test 0c: class enum sanity (guards against the off-by-one regression) */
	PrintToServer("[DODHOOKS-TEST] --- Enum Sanity (must match the game) ---");
	PrintToServer("[DODHOOKS-TEST] DODClass_Random=%d DODClass_None=%d DODClass_Rifleman=%d DODClass_Rocket=%d",
		DODClass_Random, DODClass_None, DODClass_Rifleman, DODClass_Rocket);
	if (view_as<int>(DODClass_Rifleman) != 0 || view_as<int>(DODClass_None) != -1)
	{
		PrintToServer("[DODHOOKS-TEST] !! Class enum is off by one - classes will be shifted.");
	}

	/* Test 1: Native existence checks */
	PrintToServer("[DODHOOKS-TEST] --- Native Availability ---");
	TestNative("DOD_GetPlayerClass");
	TestNative("DOD_SetPlayerClass");
	TestNative("DOD_GetDesiredPlayerClass");
	TestNative("DOD_SetDesiredPlayerClass");
	TestNative("DOD_PopHelmet");
	TestNative("DOD_SetNumControlPoints");
	TestNative("DOD_PrecacheCPIcon");
	TestNative("DOD_SetCPIcons");
	TestNative("DOD_SetCPVisible");
	TestNative("DOD_PauseTimer");
	TestNative("DOD_ResumeTimer");
	TestNative("DOD_SetTimeRemaining");
	TestNative("DOD_GetTimeRemaining");
	TestNative("DOD_RespawnPlayer");
	TestNative("DOD_AddWaveTime");
	TestNative("DOD_SetWinningTeam");
	TestNative("DOD_SetRoundState");
	TestNative("DOD_SetPlayerState");
	TestNative("DOD_SetBombTargetState");
	TestNative("DOD_IsAvailable");
	TestNative("DOD_GetDetourCount");
	TestNative("DOD_GetDetourTotal");
	TestNative("DOD_IsObjectiveResourceReady");

	/* Test 2: Read-only native on a real player (if any) */
	PrintToServer("[DODHOOKS-TEST] --- Runtime Native Test ---");
	int testClient = FindTestClient();
	if (testClient > 0)
	{
		int cls = DOD_GetPlayerClass(testClient);
		int desired = DOD_GetDesiredPlayerClass(testClient);
		PrintToServer("[DODHOOKS-TEST] Player %N: class=%d, desiredClass=%d", testClient, cls, desired);

		if (cls >= 0 && cls < view_as<int>(DODClass_Size))
		{
			PrintToServer("[DODHOOKS-TEST] DOD_GetPlayerClass: OK (class=%d is in range)", cls);
		}
		else if (cls == view_as<int>(DODClass_None))
		{
			PrintToServer("[DODHOOKS-TEST] DOD_GetPlayerClass: OK (player has no class yet)");
		}
		else
		{
			PrintToServer("[DODHOOKS-TEST] !! DOD_GetPlayerClass returned %d - out of range,", cls);
			PrintToServer("[DODHOOKS-TEST] !! the class enum is probably shifted.");
		}
	}
	else
	{
		PrintToServer("[DODHOOKS-TEST] No players in game, skipping runtime native test.");
		PrintToServer("[DODHOOKS-TEST] (Connect a player and run 'dodhooks_test' again)");
	}

	/* Test 3: PrecacheCPIcon (safe, read-only-ish) */
	int matIdx = DOD_PrecacheCPIcon("sprites/obj_icons/icon_obj_allies.vmt");
	PrintToServer("[DODHOOKS-TEST] DOD_PrecacheCPIcon(icon_obj_allies.vmt) returned: %d", matIdx);
	if (matIdx > 0)
	{
		PrintToServer("[DODHOOKS-TEST] DOD_PrecacheCPIcon: OK");
	}
	else
	{
		PrintToServer("[DODHOOKS-TEST] DOD_PrecacheCPIcon: returned 0 (material may not exist,");
		PrintToServer("[DODHOOKS-TEST] or the Materials string table is not ready yet)");
	}

	PrintToServer("[DODHOOKS-TEST] ===== Diagnostic Complete =====");
	return Plugin_Handled;
}

/**
 * Console command: show forward statistics
 */
public Action Cmd_Status(int client, int args)
{
	PrintToServer("[DODHOOKS-TEST] ===== Forward Fire Counts =====");
	PrintToServer("[DODHOOKS-TEST] Detours active    : %d/%d", DOD_GetDetourCount(), DOD_GetDetourTotal());
	PrintToServer("[DODHOOKS-TEST] OnVoiceCommand    : %d", g_iVoiceCmdCount);
	PrintToServer("[DODHOOKS-TEST] OnJoinClass       : %d", g_iJoinClassCount);
	PrintToServer("[DODHOOKS-TEST] OnPopHelmet       : %d", g_iPopHelmetCount);
	PrintToServer("[DODHOOKS-TEST] OnPlayerRespawn   : %d", g_iRespawnCount);
	PrintToServer("[DODHOOKS-TEST] OnEnterRoundState : %d", g_iRoundStateCount);
	PrintToServer("[DODHOOKS-TEST] OnEnterPlayerState: %d", g_iPlayerStateCount);
	PrintToServer("[DODHOOKS-TEST] OnAddWaveTime     : %d", g_iAddWaveTimeCount);
	PrintToServer("[DODHOOKS-TEST] OnSetWinningTeam  : %d", g_iSetWinningTeamCount);
	PrintToServer("[DODHOOKS-TEST] OnEnterBombTargetState: %d", g_iBombTargetStateCount);

	int total = g_iVoiceCmdCount + g_iJoinClassCount + g_iPopHelmetCount + g_iRespawnCount
		+ g_iRoundStateCount + g_iPlayerStateCount + g_iAddWaveTimeCount
		+ g_iSetWinningTeamCount + g_iBombTargetStateCount;

	if (DOD_GetDetourCount() > 0 && total == 0)
	{
		PrintToServer("[DODHOOKS-TEST] No forward has fired yet - detours are installed but");
		PrintToServer("[DODHOOKS-TEST] nothing triggered them, or the hooks target the wrong functions.");
	}
	PrintToServer("[DODHOOKS-TEST] =============================");
	return Plugin_Handled;
}

/**
 * Helper: check if a native exists and print result
 */
void TestNative(const char[] name)
{
	FeatureStatus status = GetFeatureStatus(FeatureType_Native, name);

	if (status == FeatureStatus_Available)
	{
		PrintToServer("[DODHOOKS-TEST] %-32s OK", name);
	}
	else if (status == FeatureStatus_Unknown)
	{
		PrintToServer("[DODHOOKS-TEST] %-32s UNKNOWN", name);
	}
	else if (status == FeatureStatus_Unavailable)
	{
		PrintToServer("[DODHOOKS-TEST] %-32s NOT AVAILABLE", name);
	}
	else
	{
		PrintToServer("[DODHOOKS-TEST] %-32s ERROR", name);
	}
}

/**
 * Helper: find a human player for runtime testing
 */
int FindTestClient()
{
	for (int i = 1; i <= MaxClients; i++)
	{
		if (IsClientInGame(i) && !IsFakeClient(i))
		{
			return i;
		}
	}
	return 0;
}

/* ============================================================
 * FORWARD IMPLEMENTATIONS (detour hooks)
 * These fire when the game calls the detoured functions.
 * If they fire, the extension's detours are working correctly.
 * ============================================================
 */

public Action OnVoiceCommand(int client, int &voiceCommand)
{
	g_iVoiceCmdCount++;
	if (g_iVoiceCmdCount <= 5)
	{
		PrintToServer("[DODHOOKS-TEST] OnVoiceCommand: client=%d, cmd=%d", client, voiceCommand);
	}
	return Plugin_Continue;
}

public Action OnJoinClass(int client, int &playerClass)
{
	g_iJoinClassCount++;
	if (g_iJoinClassCount <= 10)
	{
		PrintToServer("[DODHOOKS-TEST] OnJoinClass: client=%d, class=%d", client, playerClass);
	}
	return Plugin_Continue;
}

public Action OnPopHelmet(int client, float velocity[3], float origin[3])
{
	g_iPopHelmetCount++;
	if (g_iPopHelmetCount <= 5)
	{
		PrintToServer("[DODHOOKS-TEST] OnPopHelmet: client=%d, vel=[%.1f %.1f %.1f], origin=[%.1f %.1f %.1f]",
			client, velocity[0], velocity[1], velocity[2], origin[0], origin[1], origin[2]);
	}
	return Plugin_Continue;
}

public Action OnPlayerRespawn(int client)
{
	g_iRespawnCount++;
	if (g_iRespawnCount <= 10)
	{
		PrintToServer("[DODHOOKS-TEST] OnPlayerRespawn: client=%d", client);
	}
	return Plugin_Continue;
}

public Action OnAddWaveTime(int team, float &delay)
{
	g_iAddWaveTimeCount++;
	if (g_iAddWaveTimeCount <= 5)
	{
		PrintToServer("[DODHOOKS-TEST] OnAddWaveTime: team=%d, delay=%.1f", team, delay);
	}
	return Plugin_Continue;
}

public Action OnSetWinningTeam(int team)
{
	g_iSetWinningTeamCount++;
	PrintToServer("[DODHOOKS-TEST] OnSetWinningTeam: team=%d", team);
	return Plugin_Continue;
}

public Action OnEnterRoundState(int &roundState)
{
	g_iRoundStateCount++;
	PrintToServer("[DODHOOKS-TEST] OnEnterRoundState: state=%d", roundState);
	return Plugin_Continue;
}

public Action OnEnterPlayerState(int client, int &playerState)
{
	g_iPlayerStateCount++;
	if (g_iPlayerStateCount <= 10)
	{
		PrintToServer("[DODHOOKS-TEST] OnEnterPlayerState: client=%d, state=%d", client, playerState);
	}
	return Plugin_Continue;
}

public Action OnEnterBombTargetState(int entity, int &bombState)
{
	g_iBombTargetStateCount++;
	if (g_iBombTargetStateCount <= 10)
	{
		PrintToServer("[DODHOOKS-TEST] OnEnterBombTargetState: entity=%d, state=%d", entity, bombState);
	}
	return Plugin_Continue;
}

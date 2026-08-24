#if defined _dodhooks_included
 #endinput
#endif
#define _dodhooks_included

/**
 * DODHooks - SourcePawn Include File
 *
 * Provides natives and forwards for the DODHooks extension.
 * Compatible with SourceMod 1.12 / 1.13
 *
 * Based on the original 2015 (psychonic) include, updated for
 * the modern DOD_ prefixed native API and new SourcePawn syntax.
 */

#define MAX_CONTROL_POINTS 8

/**
 * Auto-loads the DODHooks extension whenever a plugin includes this file.
 * autoload is hardcoded to 1 — no AUTOLOAD_EXTENSIONS define needed.
 *
 * IMPORTANT: The "file" field must be the BASE name "dodhooks.ext" —
 * SourceMod automatically appends the game-specific suffix (.2.dods)
 * and platform extension (.so/.dll) when resolving the binary.
 * Do NOT write "dodhooks.ext.2.dods" here or auto-load will fail.
 */
public Extension __ext_dodhooks =
{
    name = "DODHooks",
    file = "dodhooks.ext",
    autoload = 1,
#if defined REQUIRE_EXTENSIONS
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_EXTENSIONS
public __ext_dodhooks_SetNTVOptional()
{
    MarkNativeAsOptional("DOD_GetPlayerClass");
    MarkNativeAsOptional("DOD_SetPlayerClass");
    MarkNativeAsOptional("DOD_GetDesiredPlayerClass");
    MarkNativeAsOptional("DOD_SetDesiredPlayerClass");
    MarkNativeAsOptional("DOD_PopHelmet");
    MarkNativeAsOptional("DOD_SetNumControlPoints");
    MarkNativeAsOptional("DOD_PrecacheCPIcon");
    MarkNativeAsOptional("DOD_SetCPIcons");
    MarkNativeAsOptional("DOD_SetCPVisible");
    MarkNativeAsOptional("DOD_PauseTimer");
    MarkNativeAsOptional("DOD_ResumeTimer");
    MarkNativeAsOptional("DOD_SetTimeRemaining");
    MarkNativeAsOptional("DOD_GetTimeRemaining");
    MarkNativeAsOptional("DOD_RespawnPlayer");
    MarkNativeAsOptional("DOD_AddWaveTime");
    MarkNativeAsOptional("DOD_SetWinningTeam");
    MarkNativeAsOptional("DOD_SetRoundState");
    MarkNativeAsOptional("DOD_SetPlayerState");
    MarkNativeAsOptional("DOD_SetBombTargetState");
}
#endif

/**
 * ============================================================
 * ENUMS
 * ============================================================
 */

/**
 * Player class constants
 */
enum DODPlayerClass
{
    DODClass_Random       = -2,
    DODClass_None         = 0,
    DODClass_Rifleman     = 1,
    DODClass_Assault      = 2,
    DODClass_Support      = 3,
    DODClass_Sniper       = 4,
    DODClass_Machinegunner = 5,
    DODClass_Rocket       = 6,
};

/**
 * Team constants
 */
#define DODTeam_Unassigned  0
#define DODTeam_Spectator   1
#define DODTeam_Allies      2
#define DODTeam_Axis        3

/**
 * Round state constants
 */
enum DODRoundState
{
    DoDRoundState_RoundInit = 0,
    DoDRoundState_PreGame,
    DoDRoundState_StartGame,
    DoDRoundState_PreRound,
    DoDRoundState_RoundRunning,
    DoDRoundState_AlliesWin,
    DoDRoundState_AxisWin,
    DoDRoundState_Restart,
    DoDRoundState_GameOver,
    DoDRoundState_Size
};

/**
 * Player state constants
 */
enum DODPlayerState
{
    PlayerState_Active = 0,
    PlayerState_Welcome = 1,
    PlayerState_PickingTeam = 2,
    PlayerState_PickingClass = 3,
    PlayerState_DeathAnim = 4,
    PlayerState_ObserverMode = 6,
    PlayerState_Size
};

/**
 * Bomb target state constants
 */
enum DODBombTargetState
{
    BombTargetState_Inactive = 0,
    BombTargetState_Active,
    BombTargetState_Armed,
    BombTargetState_Size
};

/**
 * Voice command constants
 */
enum DODVoiceCommand
{
    Voice_Attack = 0,
    Voice_Hold,
    Voice_Left,
    Voice_Right,
    Voice_StickTogether,
    Voice_Cover,
    Voice_UseSmoke,
    Voice_UseGrens,
    Voice_CeaseFire,
    Voice_YesSir,
    Voice_Negative,
    Voice_Backup,
    Voice_FireInHole,
    Voice_Grenade,
    Voice_Sniper,
    Voice_NiceShot,
    Voice_Thanks,
    Voice_AreaClear,
    Voice_DropWeapons,
    Voice_Displace,
    Voice_MgAhead,
    Voice_EnemyBehind,
    Voice_WeGotHim,
    Voice_MoveUpMg,
    Voice_NeedAmmo,
    Voice_UseBazooka,
    Voice_BazookaSpotted,
    Voice_GoGoGo,
    Voice_Medic,
    Voice_CoverFlanks,
    Voice_Tank,
    Voice_TakeAmmo,
    Voice_MoveWithtank,
    Voice_WTF,
    Voice_FireLeft,
    Voice_FireRight,
    Voice_EnemyAhead = 37,
    Voice_FallBack
};

/**
 * ============================================================
 * STOCKS
 * ============================================================
 */

/**
 * Checks if a player class is valid (not None).
 *
 * @param playerClass   The player class value.
 * @return              True if valid, false otherwise.
 */
stock bool IsPlayerClassValid(DODPlayerClass playerClass)
{
    return playerClass != DODClass_None;
}

/**
 * ============================================================
 * NATIVES
 * ============================================================
 */

/**
 * Gets a player's current class.
 *
 * @param client    Client index.
 * @return          Current DODPlayerClass value.
 * @error           Invalid client index.
 */
native int DOD_GetPlayerClass(int client);

/**
 * Sets a player's current class.
 *
 * @param client        Client index.
 * @param playerclass   DODPlayerClass value to set.
 * @return              true on success, false on failure.
 * @error               Invalid client or class value.
 */
native bool DOD_SetPlayerClass(int client, DODPlayerClass playerclass);

/**
 * Gets a player's desired class (the class they will spawn as).
 *
 * @param client    Client index.
 * @return          Desired DODPlayerClass value.
 * @error           Invalid client index.
 */
native int DOD_GetDesiredPlayerClass(int client);

/**
 * Sets a player's desired class.
 *
 * @param client        Client index.
 * @param playerclass   DODPlayerClass value to set.
 * @return              true on success, false on failure.
 * @error               Invalid client or class value.
 */
native bool DOD_SetDesiredPlayerClass(int client, DODPlayerClass playerclass);

/**
 * Forces a player's helmet to pop off.
 *
 * @param client        Client index.
 * @param velocity      Vector array (3 floats) for helmet velocity.
 * @param origin        Vector array (3 floats) for helmet origin.
 * @return              true on success.
 * @error               Invalid client index.
 */
native bool DOD_PopHelmet(int client, const float velocity[3], const float origin[3]);

/**
 * Sets the number of control points on the map.
 *
 * @param numCP     Number of control points.
 * @return          true on success.
 */
native bool DOD_SetNumControlPoints(int numCP);

/**
 * Precaches a control point icon material and returns its index.
 *
 * @param material  Material name to precache (e.g. "sprites/obj_icons/icon_obj_allies.vmt").
 * @return          Material index, or 0 on failure.
 */
native int DOD_PrecacheCPIcon(const char[] material);

/**
 * Sets the icons for a control point.
 *
 * @param cpIndex       Control point index.
 * @param alliesIcon    Allies icon material index (0 = no change).
 * @param axisIcon      Axis icon material index (0 = no change).
 * @param neutralIcon   Neutral icon material index (0 = no change).
 * @param timerCapIcon  Timer capture icon material index (0 = no change).
 * @param bombedIcon    Bombed icon material index (0 = no change).
 * @return              true on success.
 */
native bool DOD_SetCPIcons(int cpIndex, int alliesIcon = 0, int axisIcon = 0, int neutralIcon = 0, int timerCapIcon = 0, int bombedIcon = 0);

/**
 * Sets whether a control point is visible.
 *
 * @param cpIndex   Control point index.
 * @param visible   true to show, false to hide.
 * @return          true on success.
 */
native bool DOD_SetCPVisible(int cpIndex, bool visible);

/**
 * Pauses a round timer.
 *
 * @param timerEnt  Entity index of the dod_round_timer.
 * @return          true on success.
 * @error           Invalid timer entity.
 */
native bool DOD_PauseTimer(int timerEnt);

/**
 * Resumes a round timer.
 *
 * @param timerEnt  Entity index of the dod_round_timer.
 * @return          true on success.
 * @error           Invalid timer entity.
 */
native bool DOD_ResumeTimer(int timerEnt);

/**
 * Sets the time remaining on a round timer.
 *
 * @param timerEnt  Entity index of the dod_round_timer.
 * @param seconds   Time remaining in seconds.
 * @return          true on success.
 * @error           Invalid timer entity.
 */
native bool DOD_SetTimeRemaining(int timerEnt, float seconds);

/**
 * Gets the time remaining on a round timer.
 *
 * @param timerEnt  Entity index of the dod_round_timer.
 * @return          Time remaining in seconds (0.0 if paused and expired).
 * @error           Invalid timer entity.
 */
native float DOD_GetTimeRemaining(int timerEnt);

/**
 * Forces a player to respawn.
 *
 * @param client        Client index.
 * @param useClass      If true, uses the player's desired class (prevents crashes).
 * @return              true on success.
 * @error               Invalid client index.
 */
native bool DOD_RespawnPlayer(int client, bool useClass = true);

/**
 * Adds wave time for a team.
 *
 * @param team      DODTeam_Allies or DODTeam_Axis.
 * @param delay     Additional time in seconds.
 * @return          true on success.
 * @error           Invalid team.
 */
native bool DOD_AddWaveTime(int team, float delay);

/**
 * Sets the winning team for the current round.
 *
 * @param team      DODTeam_Allies or DODTeam_Axis.
 * @return          true on success.
 * @error           Invalid team.
 */
native bool DOD_SetWinningTeam(int team);

/**
 * Sets the round state.
 *
 * @param state     DODRoundState value.
 * @return          true on success.
 */
native bool DOD_SetRoundState(DODRoundState state);

/**
 * Sets a player's state.
 *
 * @param client    Client index.
 * @param state     DODPlayerState value.
 * @return          true on success.
 * @error           Invalid client index.
 */
native bool DOD_SetPlayerState(int client, DODPlayerState state);

/**
 * Sets a bomb target's state.
 *
 * @param entity    Entity index of the dod_bomb_target.
 * @param state     DODBombTargetState value.
 * @return          true on success.
 * @error           Invalid entity index.
 */
native bool DOD_SetBombTargetState(int entity, DODBombTargetState state);

/**
 * ============================================================
 * FORWARDS (Hooks)
 * ============================================================
 */

/**
 * Called when a player uses a voice command.
 *
 * @param client        Client index.
 * @param voiceCommand  Voice command id (can be modified).
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnVoiceCommand(int client, int &voiceCommand);

/**
 * Called when a player joins a class.
 *
 * @param client        Client index.
 * @param playerClass   Class being joined (can be modified).
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnJoinClass(int client, int &playerClass);

/**
 * Called when a helmet is popped off a player.
 *
 * @param client        Client index.
 * @param velocity      Helmet velocity vector (can be modified).
 * @param origin        Helmet origin vector (can be modified).
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnPopHelmet(int client, float velocity[3], float origin[3]);

/**
 * Called when a player is about to respawn.
 *
 * @param client        Client index.
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnPlayerRespawn(int client);

/**
 * Called when wave time is added for a team.
 *
 * @param team          Team index.
 * @param delay         Time being added (can be modified).
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnAddWaveTime(int team, float &delay);

/**
 * Called when the winning team is set.
 *
 * @param team          Winning team.
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnSetWinningTeam(int team);

/**
 * Called when the round state changes.
 *
 * @param roundState    New round state (can be modified).
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnEnterRoundState(int &roundState);

/**
 * Called when a player's state changes.
 *
 * @param client        Client index.
 * @param playerState   New player state (can be modified).
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnEnterPlayerState(int client, int &playerState);

/**
 * Called when a bomb target's state changes.
 *
 * @param entity        Bomb target entity index.
 * @param bombState     New bomb target state (can be modified).
 * @return              Plugin_Handled to block, Plugin_Continue to allow.
 */
forward Action OnEnterBombTargetState(int entity, int &bombState);

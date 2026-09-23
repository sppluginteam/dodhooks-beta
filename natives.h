/**
 * vim: set ts=4 :
 * ======================================================
 * DODHooks - Native function declarations
 * ======================================================
 *
 * GPLv2 License
 */

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_

#include "extension.h"

/**
 * @brief Registers all natives with SourceMod.
 * Called from SDK_OnAllLoaded.
 */

/* Player class natives */
cell_t Native_GetPlayerClass(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetPlayerClass(IPluginContext *pContext, const cell_t *Params);
cell_t Native_GetDesiredPlayerClass(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetDesiredPlayerClass(IPluginContext *pContext, const cell_t *Params);

/* Helmet natives */
cell_t Native_PopHelmet(IPluginContext *pContext, const cell_t *Params);

/* Control point / objective natives */
cell_t Native_SetNumControlPoints(IPluginContext *pContext, const cell_t *Params);
cell_t Native_PrecacheCPIcon(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetCPIcons(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetCPVisible(IPluginContext *pContext, const cell_t *Params);

/* Timer natives */
cell_t Native_PauseTimer(IPluginContext *pContext, const cell_t *Params);
cell_t Native_ResumeTimer(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetTimeRemaining(IPluginContext *pContext, const cell_t *Params);
cell_t Native_GetTimeRemaining(IPluginContext *pContext, const cell_t *Params);

/* Diagnostic natives (help track down "extension won't load" reports) */
cell_t Native_GetDetourCount(IPluginContext *pContext, const cell_t *Params);
cell_t Native_GetDetourTotal(IPluginContext *pContext, const cell_t *Params);
cell_t Native_IsObjectiveResourceReady(IPluginContext *pContext, const cell_t *Params);
cell_t Native_IsAvailable(IPluginContext *pContext, const cell_t *Params);

/* Game rules natives */
cell_t Native_RespawnPlayer(IPluginContext *pContext, const cell_t *Params);
cell_t Native_AddWaveTime(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetWinningTeam(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetRoundState(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetPlayerState(IPluginContext *pContext, const cell_t *Params);
cell_t Native_SetBombTargetState(IPluginContext *pContext, const cell_t *Params);

/**
 * Native function table - must match dodhooks.inc
 */
extern const sp_nativeinfo_t g_Natives[];

#endif // _INCLUDE_SOURCEMOD_EXTENSION_NATIVES_H_

/**
 * vim: set ts=4 :
 * ======================================================
 * DODHooks - Detours & Natives for Day of Defeat: Source
 * ======================================================
 *
 * GPLv2 License
 */

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include <sourcemod_version.h>
#include <ISDKTools.h>
#include <IBinTools.h>
#include <IForwardSys.h>
#include <IPlayerHelpers.h>
#include <IGameHelpers.h>
#include <IEngineTrace.h>
#include <iconvar.h>
#include <iserver.h>
#include <convar.h>
#include <sourcemod.h>
#include <smsdk_ext.h>

/* HL2SDK public headers providing types used by this extension. */
#include <networkstringtabledefs.h>
#include <edict.h>

/**
 * @brief Helper to read/write an entity member at a byte offset.
 * Produces an lvalue so it can be used on both the left and right hand side.
 */
#define OFFSET(type, base, offset) (*(type *)((unsigned char *)(base) + (offset)))

/**
 * Convenience macros - new style uses the global interfaces directly
 * where available, with fallbacks for older SM versions.
 */
#if SM_VERSION_MINOR >= 12
// SourceMod 1.12+ provides these globals directly
#define g_pGameHelpers gamehelpers
#define g_pEngine     engine
#define g_pGameConfs  gameconfs
#else
// Older SM - need to use the interface manager
#define g_pGameHelpers gamehelpers
#define g_pEngine     engine
#define g_pGameConfs  gameconfs
#endif

extern IGameConfig *g_pGameConf;

extern CGlobalVars *g_pGlobals;

extern IBinTools *g_pBinTools;
extern ISDKTools *g_pSDKTools;

extern IServerGameEnts *g_pGameEnts;

extern CSharedEdictChangeInfo *g_pSharedChangeInfo;

extern INetworkStringTableContainer *netstringtables;

/**
 * Detour bookkeeping. A missing signature/symbol must no longer abort the
 * whole extension load - the extension stays loaded and simply reports which
 * hooks are inactive (see SDK_OnLoad / SetupDetours).
 */
extern uint32 g_iActiveDetours;
extern uint32 g_iFailedDetours;

/**
 * Value returned by GetSendPropOffset() when a send property could not be
 * resolved. Natives must check for this before touching entity memory.
 */
#define INVALID_SEND_PROP_OFFSET 0xFFFFFFFF

extern uint32 g_iOffset_PlayerClass;
extern uint32 g_iOffset_DesiredPlayerClass;

extern uint32 g_iOffset_NumControlPoints;
extern uint32 g_iOffset_AlliesIcons;
extern uint32 g_iOffset_AxisIcons;
extern uint32 g_iOffset_NeutralIcons;
extern uint32 g_iOffset_TimerCapIcons;
extern uint32 g_iOffset_BombedIcons;
extern uint32 g_iOffset_CPIsVisible;

extern uint32 g_iOffset_TimerPaused;
extern uint32 g_iOffset_TimeRemaining;
extern uint32 g_iOffset_TimerEndTime;

extern void *g_pEntList;

extern void **g_pObjectiveResource;

#define DOD_MAXPLAYERS 33

#define MAX_CONTROL_POINTS 8

/**
 * Macro to simplify detour creation with proper error reporting.
 * Compatible with both 32-bit and 64-bit builds.
 *
 * IMPORTANT: A single missing signature/symbol is no longer fatal. On 64-bit
 * servers (windows64 / linux64) or after a game update, individual detours may
 * be unavailable; the extension keeps loading so that `sm exts list` and the
 * remaining hooks/natives still work, and the console shows exactly which
 * detour failed.
 */
#define CREATE_DETOUR(detour, name, gamedata) \
	do { \
		detour = DETOUR_CREATE_MEMBER(name, gamedata); \
		if (detour != NULL) \
		{ \
			detour->EnableDetour(); \
			g_iActiveDetours++; \
		} \
		else \
		{ \
			g_iFailedDetours++; \
			META_CONPRINTF("DODHooks: Warning - detour \"%s\" not available (gamedata key missing, signature not found, or platform mismatch)\n", gamedata); \
		} \
	} while (0)

#define REMOVE_DETOUR(detour) \
	do { \
		if (detour != NULL) \
		{ \
			detour->Destroy(); \
			detour = NULL; \
		} \
	} while (0)

/**
 * Player class enum - matches DoD:S internal values
 */
/**
 * These values are the REAL game values (verified against the current
 * server binary: HandleCommand_JoinClass() compares the incoming class
 * against -2 for "random").
 *
 *   Random = -2, None = -1, Rifleman = 0 ... Rocket = 5, Size = 6
 *
 * WARNING: dodhooks.inc MUST use exactly the same numbering. An earlier
 * revision of the include file declared DODClass_None = 0 / Rifleman = 1,
 * which is off by one and silently shifts every class.
 */
enum DODPlayerClass
{
    PlayerClass_Random = -2,
    PlayerClass_None,
    PlayerClass_Rifleman,
    PlayerClass_Assault,
    PlayerClass_Support,
    PlayerClass_Sniper,
    PlayerClass_Machinegunner,
    PlayerClass_Rocket,

    PlayerClass_Size
};

/**
 * Team enum
 */
enum DODTeam
{
    Team_Unassigned = 0,
    Team_Spectator = 1,
    Team_Allies    = 2,
    Team_Axis      = 3,
};

/**
 * Helper: get send property offset safely.
 * Returns INVALID_SEND_PROP_OFFSET when the property cannot be resolved -
 * never write through the result without checking it first.
 */
inline uint32 GetSendPropOffset(const char *szNetClass, const char *szPropName)
{
    sm_sendprop_info_t SendPropInfo;

    if (!g_pGameHelpers->FindSendPropInfo(szNetClass, szPropName, &SendPropInfo))
    {
        META_CONPRINTF("DODHooks: Warning - unable to get offset: %s::%s!\n", szNetClass, szPropName);
        return INVALID_SEND_PROP_OFFSET;
    }

    return SendPropInfo.actual_offset;
}

/**
 * Helper: true when a send property offset was resolved successfully.
 */
inline bool IsValidOffset(uint32 iOffset)
{
    return (iOffset != INVALID_SEND_PROP_OFFSET);
}

/**
 * Native guards. Every native that touches entity memory through a send
 * property offset must validate that offset first - if the game or a plugin
 * renames/removes the property, writing through 0xFFFFFFFF would corrupt
 * memory instead of returning a clean error to the plugin.
 */
#define CHECK_OFFSET(off) \
    do { \
        if (!IsValidOffset(off)) \
        { \
            return pContext->ThrowNativeError("Send property %s is unavailable on this server", #off); \
        } \
    } while (0)

#define CHECK_TIMER_OFFSETS() \
    do { \
        if (!IsValidOffset(g_iOffset_TimerPaused) || \
            !IsValidOffset(g_iOffset_TimeRemaining) || \
            !IsValidOffset(g_iOffset_TimerEndTime)) \
        { \
            return pContext->ThrowNativeError("Round timer props are unavailable on this server"); \
        } \
    } while (0)

/**
 * Guards for natives that call into game code through bintools / sdktools.
 */
#define CHECK_BINTOOLS() \
    do { \
        if (!g_pBinTools) \
        { \
            return pContext->ThrowNativeError("bintools extension is not available"); \
        } \
    } while (0)

#define CHECK_GAMERULES() \
    do { \
        if (!g_pSDKTools) \
        { \
            return pContext->ThrowNativeError("sdktools extension is not available"); \
        } \
    } while (0)

/**
 * @brief Main extension class
 */
class CDODHooks : public SDKExtension
{
public:
    /**
     * @brief Called when the command client is set.
     */
    void OnSetCommandClient(int client);

public:
    /**
     * @brief Called after the initial loading sequence.
     */
    virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late) override;

    /**
     * @brief Called right before the extension is unloaded.
     */
    virtual void SDK_OnUnload() override;

    /**
     * @brief Called once all known extensions have been loaded.
     */
    virtual void SDK_OnAllLoaded() override;

#if defined SMEXT_CONF_METAMOD
    /**
     * @brief Called when Metamod is attached.
     */
    virtual bool SDK_OnMetamodLoad(SourceMM::ISmmAPI *ismm, char *error, size_t maxlength, bool late) override;
#endif

private:
    /**
     * @brief Helper to register all forwards.
     */
    void RegisterForwards();

    /**
     * @brief Helper to unregister all forwards.
     */
    void UnregisterForwards();

    /**
     * @brief Helper to setup all detours.
     */
    bool SetupDetours(char *error, size_t maxlength);

    /**
     * @brief Helper to teardown all detours.
     */
    void TeardownDetours();
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

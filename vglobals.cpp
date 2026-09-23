/**
 * vim: set ts=4 :
 * ======================================================
 * DODHooks - Valve Globals implementation
 * ======================================================
 *
 * GPLv2 License
 */

#include "vglobals.h"
#include "extension.h"

void *g_pEntList = NULL;
void **g_pObjectiveResource = NULL;

void InitializeValveGlobals()
{
    g_pEntList = g_pGameHelpers->GetGlobalEntityList();

    uint8 *pAddress = NULL;

#ifdef PLATFORM_WINDOWS
    int iOffset;

    if (!g_pGameConf->GetMemSig("CreateStandardEntities", (void **)&pAddress) || !pAddress)
    {
        META_CONPRINTF("DODHooks: Warning - Failed to find CreateStandardEntities signature\n");
        return;
    }

    if (!g_pGameConf->GetOffset("g_pObjectiveResource", &iOffset) || !iOffset)
    {
        META_CONPRINTF("DODHooks: Warning - Failed to find g_pObjectiveResource offset\n");
        return;
    }

#if defined PLATFORM_X64 || defined _WIN64 || defined(__x86_64__)
    /* x64 uses RIP-relative addressing, so the byte at pAddress+iOffset is
     * no longer an absolute pointer - it is the 4-byte displacement inside
     *   mov [rip+disp32], rax      (48 89 05 xx xx xx xx)
     * The instruction ends 4 bytes after the displacement field, so the
     * global lives at:  pAddress + iOffset + 4 + disp32.
     *
     * gamedata offset 75 = displacement of the SECOND store, which is the
     * one that saves the freshly created dod_objective_resource. (The first
     * store, offset 39, is g_pPlayerManager.) */
    {
        int32 iDisp = *reinterpret_cast<int32 *>(reinterpret_cast<uint8 *>(pAddress) + iOffset);
        uint8 *pTarget = reinterpret_cast<uint8 *>(pAddress) + iOffset + 4 + iDisp;

        g_pObjectiveResource = reinterpret_cast<void **>(pTarget);
    }
#else
    /* x86: 'mov [abs32], eax' stores the absolute address of the global. */
    g_pObjectiveResource = *reinterpret_cast<void ***>(pAddress + iOffset);
#endif

#elif defined PLATFORM_LINUX
    if (!g_pGameConf->GetMemSig("g_pObjectiveResource", (void **)&pAddress) || !pAddress)
    {
        META_CONPRINTF("DODHooks: Warning - Failed to find g_pObjectiveResource signature\n");
        return;
    }

    /* On Linux, the symbol directly gives us the address of the global. */
    g_pObjectiveResource = reinterpret_cast<void **>(pAddress);
#endif

    if (g_pObjectiveResource)
    {
        META_CONPRINTF("DODHooks: g_pObjectiveResource initialized (%p)\n", (void *)g_pObjectiveResource);
    }
    else
    {
        META_CONPRINTF("DODHooks: Warning - g_pObjectiveResource is NULL after init\n");
    }
}

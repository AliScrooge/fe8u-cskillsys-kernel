.macro SET_DATA name, value
    .global \name
    .type \name, object
    .set \name, \value
.endm

SET_DATA FreeRamSpaceTop,    0x02026E30
SET_DATA FreeRamSpaceBottom, 0x02028E58
SET_DATA UsedFreeRamSpaceTop, FreeRamSpaceBottom

.macro _kernel_malloc name, size
    .set UsedFreeRamSpaceTop, UsedFreeRamSpaceTop - \size
    SET_DATA \name, UsedFreeRamSpaceTop
.endm

SET_DATA FreeRamSpace2Top,    0x0203AAA4
SET_DATA FreeRamSpace2Bottom, 0x0203DDE0
SET_DATA UsedFreeRamSpace2Top, FreeRamSpace2Bottom

.macro _kernel_malloc2 name, size
    .set UsedFreeRamSpace2Top, UsedFreeRamSpace2Top - \size
    SET_DATA \name, UsedFreeRamSpace2Top
.endm

SET_DATA EwramOverlay0_FreeRamSpaceTop,    0x201F200
SET_DATA EwramOverlay0_FreeRamSpaceBottom, 0x2020188
SET_DATA EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_FreeRamSpaceBottom

.macro _kernel_malloc_overlay0 name, size
    .set EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_UsedFreeRamSpaceTop - \size
    SET_DATA \name, EwramOverlay0_UsedFreeRamSpaceTop
.endm

/* From the bottom to the top */
_kernel_malloc sSkillList, 0x40 * 3
_kernel_malloc sStatScreenPage1BarMax, 4
_kernel_malloc sLearnedSkillPLists, 0x46 * 0x20
_kernel_malloc sEfxSkillRoundData, 8 * 0x21
_kernel_malloc sEfxCombatArtRoundData,  0x30
_kernel_malloc sEfxSkillQueue, 32
_kernel_malloc gBattleTemporaryFlag, 8
_kernel_malloc gBattleActorGlobalFlag, 8
_kernel_malloc gBattleTargetGlobalFlag, 8
_kernel_malloc gCombatArtStatus, 0x10
_kernel_malloc sCombatArtList, 0x20
_kernel_malloc sSelectedComatArtIndex, 4
_kernel_malloc sCombatArtBKSELfxTimer, 4
_kernel_malloc sHelpBoxType, 4
_kernel_malloc sRandSeedsC, 8
_kernel_malloc sRandBackup, 8
_kernel_malloc gPostActionGaleforceFlag, 4
_kernel_malloc gBanimSyncHandler, 8
_kernel_malloc gComboAtkList, 0x14
_kernel_malloc sKTutorialBits, 0xC
_kernel_malloc sKTutorialBitsHistory, 0xC
_kernel_malloc gBattleTargetPositionBackup, 0x4
_kernel_malloc gActionDataExpa, 0x10
_kernel_malloc gComboMapAnimBattleUnit, 0x80
_kernel_malloc sStatDebuffStatusAlly, 51 * 4
_kernel_malloc sStatDebuffStatusEnemy, 50 * 4
_kernel_malloc sStatDebuffStatusNpc, 8 * 4
_kernel_malloc sStatDebuffMsgBuf, 0x1C * 6
_kernel_malloc sStatDebuffMsgBufNext, 4
_kernel_malloc sPrepareNextBattleRoundTimer, 4

/**
 * These part of space is allocated from `ewram_overlay_0`
 * For more space, place refer to decomp ldscript.
 *
 * Note that it is risky to use this area rashly
 */
_kernel_malloc_overlay0 sPrepEquipSkillList, 0x120

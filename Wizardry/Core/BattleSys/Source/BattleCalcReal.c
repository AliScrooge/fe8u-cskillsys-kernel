#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

STATIC_DECLAR void BattleCalcReal_ModifyBattleStatusSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    /**
     * Here we need to put some calculation at the end of the pre-battle calc.
     * Thus the main part of calc should be positioned at berfore.
     */

#if (defined(SID_CatchingUp) && (COMMON_SKILL_VALID(SID_CatchingUp)))
        if (SkillTester(&attacker->unit, SID_CatchingUp))
        {
            /**
             * Check if the enemy unit doubles the skill holder
             * if there's any additional speed above the doubling threshold
             * add that to the skillholder's attack
             */
            if ((defender->battleSpeed - attacker->battleSpeed) > BATTLE_FOLLOWUP_SPEED_THRESHOLD)
                attacker->battleAttack += (defender->battleSpeed - attacker->battleSpeed);
        }
#endif

    if (attacker->battleAttack > defender->battleAttack)
    {
#if (defined(SID_HeavyBlade) && (COMMON_SKILL_VALID(SID_HeavyBlade)))
        if (SkillTester(&attacker->unit, SID_HeavyBlade))
            attacker->battleCritRate += 15;
#endif

#if (defined(SID_HeavyBladePlus) && (COMMON_SKILL_VALID(SID_HeavyBladePlus)))
        if (SkillTester(&attacker->unit, SID_HeavyBladePlus))
            attacker->battleCritRate += 25;
#endif
    }

    if (attacker->unit.conBonus < defender->unit.conBonus)
    {
#if (defined(SID_DancingBlade) && (COMMON_SKILL_VALID(SID_DancingBlade)))
        if (SkillTester(&attacker->unit, SID_DancingBlade))
        {
            attacker->battleSpeed += 4;
            attacker->battleDefense += 2;
        }
#endif
    }

    if (attacker->battleSpeed > defender->battleSpeed)
    {
#if (defined(SID_FlashingBlade) && (COMMON_SKILL_VALID(SID_FlashingBlade)))
        if (SkillTester(&attacker->unit, SID_FlashingBlade))
            attacker->battleCritRate += 15;
#endif

#if (defined(SID_FlashingBladePlus) && (COMMON_SKILL_VALID(SID_FlashingBladePlus)))
        if (SkillTester(&attacker->unit, SID_FlashingBladePlus))
            attacker->battleCritRate += 25;
#endif

#if (defined(SID_Puissance) && (COMMON_SKILL_VALID(SID_Puissance)))
        if (SkillTester(&attacker->unit, SID_Puissance))
            attacker->battleAttack += 3;
#endif
    }
}

/* LynJump */
void ComputeBattleUnitSpecialWeaponStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker->weaponAttributes & IA_MAGICDAMAGE)
    {
        switch (GetItemIndex(attacker->weapon)) {
        case ITEM_SWORD_LIGHTBRAND:
        case ITEM_SWORD_RUNESWORD:
        case ITEM_SWORD_WINDSWORD:
            attacker->battleAttack -= UNIT_MAG(&attacker->unit) / 2;
            attacker->battleCritRate = 0;
            attacker->battleEffectiveCritRate = 0;
            break;
        }
    }
    else
    {
        if (GetItemWeaponEffect(attacker->weaponBefore) == WPN_EFFECT_HPHALVE)
        {
            attacker->battleAttack = (defender->unit.curHP + 1) >> 1;

            if (attacker->battleAttack == 0)
                attacker->battleAttack = 1;

            defender->battleDefense = 0;
            attacker->battleCritRate = 0;
            attacker->battleEffectiveCritRate = 0;
        }

        if (attacker->weaponAttributes & IA_NEGATE_DEFENSE)
            defender->battleDefense = 0;

#ifdef CHAX
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_PETRIFY || GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_13)
#else
        if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13)
#endif
        {
            attacker->battleEffectiveHitRate = 100;
            attacker->battleEffectiveCritRate += 30;

            if (attacker->battleEffectiveCritRate > 100)
                attacker->battleEffectiveCritRate = 100;
        }
    }
}

STATIC_DECLAR void BattleCalcReal_ComputSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if (defined(SID_Hawkeye) && (COMMON_SKILL_VALID(SID_Hawkeye)))
    if (SkillTester(&attacker->unit, SID_Hawkeye))
        attacker->battleEffectiveHitRate = 100;
#endif

#if (defined(SID_Merciless) && (COMMON_SKILL_VALID(SID_Merciless)))
    if (SkillTester(&attacker->unit, SID_Merciless))
    {
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_POISON)
            attacker->battleEffectiveCritRate = 100;
    }
#endif

#if (defined(SID_WonderGuard) && (COMMON_SKILL_VALID(SID_WonderGuard)))
    if (SkillTester(&defender->unit, SID_WonderGuard))
    {
        if (defender->weaponType == attacker->weaponType) 
            attacker->battleAttack = 0;
    }
#endif

#if (defined(SID_NoGuard) && (COMMON_SKILL_VALID(SID_NoGuard)))
    if (SkillTester(&attacker->unit, SID_NoGuard) || SkillTester(&defender->unit, SID_NoGuard))
        attacker->battleEffectiveHitRate = 100;
#endif
}

/* LynJump */
void ComputeBattleUnitSilencerRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    return;
}

/* LynJump */
void ComputeBattleUnitEffectiveHitRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    attacker->battleEffectiveHitRate = attacker->battleHitRate - defender->battleAvoidRate;

    /* For non-ballista combat, Distance +2, hit rate -20% for actor */
    if (gBattleStats.range > 2 && attacker == &gBattleActor && !(gBattleStats.config & BATTLE_CONFIG_BALLISTA) && !(gBattleStats.config & BATTLE_CONFIG_ARENA))
    {
        attacker->battleEffectiveHitRate -= Div(gBattleStats.range, 2) * 20;

        if (gBattleStats.config & BATTLE_CONFIG_REAL)
            TriggerKtutorial(KTUTORIAL_RANGED_FAILOFF);
    }

    if (attacker->battleEffectiveHitRate > 100)
        attacker->battleEffectiveHitRate = 100;

    if (attacker->battleEffectiveHitRate < 0)
        attacker->battleEffectiveHitRate = 0;
}

/* LynJump */
void ComputeBattleUnitEffectiveStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
#if CHAX
    BattleCalcReal_ModifyBattleStatusSkills(attacker, defender);
#endif

    ComputeBattleUnitEffectiveHitRate(attacker, defender);
    ComputeBattleUnitEffectiveCritRate(attacker, defender);
    ComputeBattleUnitSilencerRate(attacker, defender);
    ComputeBattleUnitSpecialWeaponStats(attacker, defender);

#if CHAX
    BattleCalcReal_ComputSkills(attacker, defender);
#endif
}

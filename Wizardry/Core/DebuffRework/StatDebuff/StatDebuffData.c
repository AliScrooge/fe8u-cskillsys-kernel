#include "common-chax.h"
#include "debuff.h"

const struct DebuffInfo gStatDebuffInfos[UNIT_STAT_DEBUFF_MAX] = {
    [UNIT_STAT_DEBUFF_POW] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .pow = -4 },
    },
    [UNIT_STAT_DEBUFF_MAG] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .mag = -4 },
    },
    [UNIT_STAT_DEBUFF_SKL] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .skl = -4 },
    },
    [UNIT_STAT_DEBUFF_SPD] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .spd = -4 },
    },
    [UNIT_STAT_DEBUFF_LCK] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .lck = -4 },
    },
    [UNIT_STAT_DEBUFF_DEF] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .def = -4 },
    },
    [UNIT_STAT_DEBUFF_RES] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .res = -4 },
    },
    [UNIT_STAT_DEBUFF_MOV] = {
        .type = STATUS_INFO_TYPE_DEBUFF,
        .unit_status = { .mov = -4 },
    },
};
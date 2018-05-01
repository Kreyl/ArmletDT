#pragma once

#include "qhsm.h"

enum PlayerSignals {
    TIME_TICK_1M_SIG = Q_USER_SIG + 1,
    TIME_TICK_1S_SIG,
    INFLUENCE_AT_DOGAN_SIG,
    BTN_ENABLE_LONG_SIG,
    BTN_NEXT_PICTURE_SIG,
    BTN_UP_SIG,
    BTN_DOWN_SIG,
    BTN_PRESSED_SIG,
    BTN_POWER_LONG_SIG,
    BTN_DOGAN_SIG,
    BATTERY_LOW_SIG,
    BTN_DEATH_LONG_SIG,
    BTN_HYPNOSIS_LONG_SIG,
    BATTERY_INFO_SIG,
    BASE_SIG
};

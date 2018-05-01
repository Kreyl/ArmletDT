/*****************************************************************************
* Model: screen.qm
* File:  ./screen.cpp
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${.::screen.cpp} .........................................................*/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdint>

#include "qhsm.h"
#include "signals.h"
#include "dispatcher.h"
#include "reasons.h"
#include "bsp.h"
#include "screen.h"

//Q_DEFINE_THIS_FILE


/* global-scope definitions -----------------------------------------*/

/*${SMs::Screen_ctor} ......................................................*/
void Screen_ctor(Screen* me, Dispatcher* dispatcher) {
    me->timer = 0;
    me->ChargePercent = 100;
    me->dispatcher = dispatcher;
    QHsm_ctor(&me->super, Q_STATE_CAST(&Screen_initial));
}

/*${SMs::Screen} ...........................................................*/
/*${SMs::Screen::SM} .......................................................*/
QState Screen_initial(Screen * const me, QEvt const * const e) {
    /* ${SMs::Screen::SM::initial} */
    return Q_TRAN(&Screen_active);
}
/*${SMs::Screen::SM::global} ...............................................*/
QState Screen_global(Screen * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {

#ifdef DESKTOP
        /* ${SMs::Screen::SM::global::TERMINATE} */
        case TERMINATE_SIG: {
            status_ = Q_TRAN(&Screen_final);
            break;
        }
#endif /* def DESKTOP */

        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${SMs::Screen::SM::global::ScreenButtons} ................................*/
QState Screen_ScreenButtons(Screen * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Screen::SM::global::ScreenButtons} */
        case Q_ENTRY_SIG: {
            ScreenShowPicture("DarkTower.bmp");
                me->timer = 0;
                me->ChargePercent = 100;
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::BTN_DOGAN} */
        case BTN_DOGAN_SIG: {
            DISPATCH_ONESHOT(SHOW_DOGAN_STATE);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::BTN_POWER_LONG} */
        case BTN_POWER_LONG_SIG: {
            PowerOff();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::BTN_PRESSED} */
        case BTN_PRESSED_SIG: {
            me->timer = 0;
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::BATTERY_INFO} */
        case BATTERY_INFO_SIG: {
            me->ChargePercent = ((const ScreenQEvt*)e)->ChargePercent;
                if (((const ScreenQEvt*)e)->Connected == true) {
                    if (((const ScreenQEvt*)e)->Charging == true) {
                        ScreenShowPicture("Charging.bmp");
                    } else {
                        if (((const ScreenQEvt*)e)->ChargePercent >= 95) {
                            ScreenShowPicture("Charged.bmp");
                        } else {
                            ScreenShowPicture("NotCharging.bmp");
                       }
                    }
                }
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::TIME_TICK_1M} */
        case TIME_TICK_1M_SIG: {
            if (GetBMPQueueLength()>0) {
                    Vibro(SHORT_VIBRO);
                }
                if (me->ChargePercent <= 20) {
                    ScreenShowPicture("BatteryLow.bmp");
                }
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Screen_global);
            break;
        }
    }
    return status_;
}
/*${SMs::Screen::SM::global::ScreenButtons::active} ........................*/
QState Screen_active(Screen * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Screen::SM::global::ScreenButtons::active} */
        case Q_ENTRY_SIG: {
            ScreenShowPicture("Unlocked.bmp");
                SleepDisable();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::active::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            PlayerVolumeDown();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::active::BTN_UP} */
        case BTN_UP_SIG: {
            PlayerVolumeUp();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::active::BTN_NEXT_PICTURE} */
        case BTN_NEXT_PICTURE_SIG: {
            ScreenShowNextBMP();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::active::BTN_HYPNOSIS_LONG} */
        case BTN_HYPNOSIS_LONG_SIG: {
            DISPATCH_ONESHOT(HYPNOSIS);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::active::BTN_DEATH_LONG} */
        case BTN_DEATH_LONG_SIG: {
            DISPATCH_ONESHOT(DEAD);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::active::TIME_TICK_1S} */
        case TIME_TICK_1S_SIG: {
            /* ${SMs::Screen::SM::global::ScreenButtons::active::TIME_TICK_1S::[me->timer>LOCK_LEVEL]} */
            if (me->timer>LOCK_LEVEL) {
                status_ = Q_TRAN(&Screen_locked);
            }
            /* ${SMs::Screen::SM::global::ScreenButtons::active::TIME_TICK_1S::[else]} */
            else {
                me->timer++;
                status_ = Q_HANDLED();
            }
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::active::BTN_ENABLE_LONG} */
        case BTN_ENABLE_LONG_SIG: {
            status_ = Q_TRAN(&Screen_locked);
            break;
        }
        default: {
            status_ = Q_SUPER(&Screen_ScreenButtons);
            break;
        }
    }
    return status_;
}
/*${SMs::Screen::SM::global::ScreenButtons::locked} ........................*/
QState Screen_locked(Screen * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Screen::SM::global::ScreenButtons::locked} */
        case Q_ENTRY_SIG: {
            ScreenShowPicture("Locked.bmp");
                SleepEnable();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Screen::SM::global::ScreenButtons::locked::BTN_ENABLE_LONG} */
        case BTN_ENABLE_LONG_SIG: {
            status_ = Q_TRAN(&Screen_active);
            break;
        }
        default: {
            status_ = Q_SUPER(&Screen_ScreenButtons);
            break;
        }
    }
    return status_;
}

#ifdef DESKTOP
/*${SMs::Screen::SM::final} ................................................*/
QState Screen_final(Screen * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Screen::SM::final} */
        case Q_ENTRY_SIG: {
            printf("Bye! Bye!"); exit(0);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
#endif /* def DESKTOP */






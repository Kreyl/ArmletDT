/*****************************************************************************
* Model: Character.qm
* File:  ./character.cpp
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
/*${.::character.cpp} ......................................................*/
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
#include "character.h"

//Q_DEFINE_THIS_FILE


/* global-scope definitions -----------------------------------------*/

/*${SMs::Character_ctor} ...................................................*/
void Character_ctor(
    Character* me,
    Dispatcher* dispatcher,
    int Dogan,
    bool Manni,
    bool Corrupted,
    bool Dead)
{
    me->Todash = false;
    me->DoganScale = 0;
    me->Manni = Manni;
    if (Dead) {
        switch (Dogan) {
            case CRIMSON: {
                me->StartState = (QStateHandler)&Character_crimson_dead;
                break;
            }
            case NEUTRAL: {
                me->StartState = (QStateHandler)&Character_neutral_dead;
                break;
            }
            case WHITE: {
                me->StartState = (QStateHandler)&Character_white_dead;
                break;
           }
          default:
               me->StartState = (QStateHandler)&Character_neutral_dead;
               break;
       }
    } else {
        if (Corrupted) {
            me->StartState = (QStateHandler)&Character_corrupted;
        } else {
            switch (Dogan) {
                case CRIMSON: {
                    me->StartState = (QStateHandler)&Character_crimson;
                    break;
                }
                case CRIMSONISH: {
                    me->StartState = (QStateHandler)&Character_crimsonish;
                    break;
                }
                case NEUTRAL: {
                    me->StartState = (QStateHandler)&Character_neutral;
                    break;
                }
                case WHITISH: {
                    me->StartState = (QStateHandler)&Character_whitish;
                    break;
                }
                case WHITE: {
                    me->StartState = (QStateHandler)&Character_white;
                    break;
                }
                default:
                    me->StartState = (QStateHandler)&Character_neutral;
                    break;
             }
        }
    }
    me->dispatcher = dispatcher;
    QHsm_ctor(&me->super, Q_STATE_CAST(&Character_initial));
}


/*${SMs::Character} ........................................................*/
/*${SMs::Character::SM} ....................................................*/
QState Character_initial(Character * const me, QEvt const * const e) {
    /* ${SMs::Character::SM::initial} */
    return Q_TRAN(me->StartState);
    return Q_TRAN(&Character_neutral);
}
/*${SMs::Character::SM::global} ............................................*/
QState Character_global(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {

#ifdef DESKTOP
        /* ${SMs::Character::SM::global::TERMINATE} */
        case TERMINATE_SIG: {
            status_ = Q_TRAN(&Character_final);
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
/*${SMs::Character::SM::global::character} .................................*/
QState Character_character(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::BEGIN(DOOR)+BASE} */
        case BEGIN(DOOR)+BASE_SIG: {
            if (me->Todash == false) {
                    me->Todash = true;
                    DISPATCH_ONESHOT(DOOR_VOICE);
                } else {
                    me->Todash = false;
                }
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::BEGIN(DOOR_NEAR)+BASE} */
        case BEGIN(DOOR_NEAR)+BASE_SIG: {
            if (me->Manni == true) {
                    DISPATCH_BEGIN(DOOR_NEAR_MANNI);
                }
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::BEGIN(DOOR_FAR)+BASE} */
        case BEGIN(DOOR_FAR)+BASE_SIG: {
            if (me->Manni == true) {
                    DISPATCH_END(DOOR_FAR_MANNI);
                }
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(DOOR_FAR)+BASE} */
        case END(DOOR_FAR)+BASE_SIG: {
            DISPATCH_BEGIN(DOOR_FAR_MANNI);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(DOOR_NEAR)+BASE} */
        case END(DOOR_NEAR)+BASE_SIG: {
            DISPATCH_BEGIN(DOOR_NEAR_MANNI);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(CRIMSON_FEELING)+BASE} */
        case END(CRIMSON_FEELING)+BASE_SIG: {
            DISPATCH_END(CRIMSON_LAUGH);
                DISPATCH_END(WHITE_SORROW);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(WHITE_FEELING)+BASE} */
        case END(WHITE_FEELING)+BASE_SIG: {
            DISPATCH_END(CRIMSON_GRIEVE);
                DISPATCH_END(WHITE_JOY);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(CRIMSON_BROADCAST1)+BASE} */
        case END(CRIMSON_BROADCAST1)+BASE_SIG: {
            DISPATCH_END(CRIMSON_BROADCAST1);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(CRIMSON_BROADCAST2)+BASE} */
        case END(CRIMSON_BROADCAST2)+BASE_SIG: {
            DISPATCH_END(CRIMSON_BROADCAST2);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(CRIMSON_BROADCAST3)+BASE} */
        case END(CRIMSON_BROADCAST3)+BASE_SIG: {
            DISPATCH_END(CRIMSON_BROADCAST3);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::END(CRIMSON_BROADCAST4)+BASE} */
        case END(CRIMSON_BROADCAST4)+BASE_SIG: {
            DISPATCH_END(CRIMSON_BROADCAST4);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_global);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive} ..........................*/
QState Character_alive(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        default: {
            status_ = Q_SUPER(&Character_character);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::neutral} .................*/
QState Character_neutral(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::neutral} */
        case Q_ENTRY_SIG: {
            Vibro(LONG_VIBRO);
                DISPATCH_ONESHOT(BECOME_NEUTRAL);
                ScreenAddBMPToQueue("Neutral.bmp");
                SaveState(NEUTRAL, false, false);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::neutral::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("Neutral.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::neutral::BEGIN(MASTER_WHITE)+BASE} */
        case BEGIN(MASTER_WHITE)+BASE_SIG: {
            status_ = Q_TRAN(&Character_whitish);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::neutral::BEGIN(MASTER_CRIMSON)+BASE} */
        case BEGIN(MASTER_CRIMSON)+BASE_SIG: {
            status_ = Q_TRAN(&Character_crimsonish);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::neutral::INFLUENCE_AT_DOGAN} */
        case INFLUENCE_AT_DOGAN_SIG: {
            me->DoganScale += ((const CharacterQEvt*)e)->amount;
            /* ${SMs::Character::SM::global::character::alive::neutral::INFLUENCE_AT_DOG~::[me->DoganScale>=WHITE_THRESHOLD~} */
            if (me->DoganScale>=WHITE_THRESHOLD) {
                status_ = Q_TRAN(&Character_whitish);
            }
            /* ${SMs::Character::SM::global::character::alive::neutral::INFLUENCE_AT_DOG~::[me->DoganScale<=CRIMSON_THRESHO~} */
            else if (me->DoganScale<=CRIMSON_THRESHOLD) {
                status_ = Q_TRAN(&Character_crimsonish);
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::neutral::BEGIN(DEATH)+BASE} */
        case BEGIN(DEATH)+BASE_SIG: {
            status_ = Q_TRAN(&Character_neutral_dead);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_alive);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::whiten} ..................*/
QState Character_whiten(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::whiten::BEGIN(WHITE_FEELING)+BASE} */
        case BEGIN(WHITE_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(WHITE_JOY);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::BEGIN(CRIMSON_FEELING)+BASE} */
        case BEGIN(CRIMSON_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(WHITE_SORROW);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::BEGIN(DEATH)+BASE} */
        case BEGIN(DEATH)+BASE_SIG: {
            status_ = Q_TRAN(&Character_white_dead);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_alive);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::whiten::white} ...........*/
QState Character_white(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::whiten::white} */
        case Q_ENTRY_SIG: {
            Vibro(LONG_VIBRO); ScreenShowPicture("White.bmp");
                DISPATCH_ONESHOT(BECOME_WHITE);
                ScreenAddBMPToQueue("White.bmp");
                DISPATCH_ONESHOT(DESTROY_KATET);
                SaveState(WHITE, false, false);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::white::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("White.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::white::BEGIN(MASTER_CRIMSON)+BASE} */
        case BEGIN(MASTER_CRIMSON)+BASE_SIG: {
            status_ = Q_TRAN(&Character_whitish);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_whiten);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::whiten::whitish} .........*/
QState Character_whitish(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::whiten::whitish} */
        case Q_ENTRY_SIG: {
            Vibro(LONG_VIBRO);
                DISPATCH_ONESHOT(BECOME_WHITISH);
                ScreenAddBMPToQueue("BecomeWhiter.bmp");
                SaveState(WHITISH, false, false);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::whitish::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("Whitish.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::whitish::} */
        case BEGIN(NEUTRALIZE)+BASE_SIG: {
            status_ = Q_TRAN(&Character_neutral);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::whitish::BEGIN(BECOME_WHITE_MESSENGER)+BA~} */
        case BEGIN(BECOME_WHITE_MESSENGER)+BASE_SIG: {
            status_ = Q_TRAN(&Character_white);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::whitish::INFLUENCE_AT_DOGAN} */
        case INFLUENCE_AT_DOGAN_SIG: {
            /* ${SMs::Character::SM::global::character::alive::whiten::whitish::INFLUENCE_AT_DOG~::[me->DoganScale<=0]} */
            if (me->DoganScale<=0) {
                status_ = Q_TRAN(&Character_neutral);
            }
            /* ${SMs::Character::SM::global::character::alive::whiten::whitish::INFLUENCE_AT_DOG~::[else]} */
            else {
                me->DoganScale += ((const CharacterQEvt*)e)->amount;
                status_ = Q_HANDLED();
            }
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::whitish::BEGIN(MASTER_WHITE)+BASE} */
        case BEGIN(MASTER_WHITE)+BASE_SIG: {
            status_ = Q_TRAN(&Character_white);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::whiten::whitish::BEGIN(MASTER_CRIMSON)+BASE} */
        case BEGIN(MASTER_CRIMSON)+BASE_SIG: {
            status_ = Q_TRAN(&Character_neutral);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_whiten);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::red} .....................*/
QState Character_red(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::red::BEGIN(WHITE_FEELING)+BASE} */
        case BEGIN(WHITE_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_GRIEVE);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::BEGIN(CRIMSON_FEELING)+BASE} */
        case BEGIN(CRIMSON_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_LAUGH);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::BEGIN(CRIMSON_BROADCAST1)+BASE} */
        case BEGIN(CRIMSON_BROADCAST1)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST1);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::BEGIN(CRIMSON_BROADCAST2)+BASE} */
        case BEGIN(CRIMSON_BROADCAST2)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST2);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::BEGIN(CRIMSON_BROADCAST3)+BASE} */
        case BEGIN(CRIMSON_BROADCAST3)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST3);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::BEGIN(CRIMSON_BROADCAST4)+BASE} */
        case BEGIN(CRIMSON_BROADCAST4)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST4);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::BEGIN(DEATH)+BASE} */
        case BEGIN(DEATH)+BASE_SIG: {
            status_ = Q_TRAN(&Character_crimson_dead);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_alive);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::red::crimsonish} .........*/
QState Character_crimsonish(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish} */
        case Q_ENTRY_SIG: {
            Vibro(LONG_VIBRO);
                DISPATCH_ONESHOT(BECOME_CRIMSONISH);
                ScreenAddBMPToQueue("Crimsonish.bmp");
                SaveState(CRIMSONISH, false, false);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("Crimsonish.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish::BEGIN(NEUTRALIZE)+BASE} */
        case BEGIN(NEUTRALIZE)+BASE_SIG: {
            status_ = Q_TRAN(&Character_neutral);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish::BEGIN(BECOME_CRIMSON_SERVANT)+BA~} */
        case BEGIN(BECOME_CRIMSON_SERVANT)+BASE_SIG: {
            status_ = Q_TRAN(&Character_crimson);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish::BEGIN(MASTER_WHITE)+BASE} */
        case BEGIN(MASTER_WHITE)+BASE_SIG: {
            status_ = Q_TRAN(&Character_neutral);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish::BEGIN(MASTER_CRIMSON)+BASE} */
        case BEGIN(MASTER_CRIMSON)+BASE_SIG: {
            status_ = Q_TRAN(&Character_crimson);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish::BEGIN(CRIMSON_KING_SONG)+BASE} */
        case BEGIN(CRIMSON_KING_SONG)+BASE_SIG: {
            status_ = Q_TRAN(&Character_corrupted);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimsonish::INFLUENCE_AT_DOGAN} */
        case INFLUENCE_AT_DOGAN_SIG: {
            /* ${SMs::Character::SM::global::character::alive::red::crimsonish::INFLUENCE_AT_DOG~::[me->DoganScale>=0]} */
            if (me->DoganScale >= 0) {
                status_ = Q_TRAN(&Character_neutral);
            }
            /* ${SMs::Character::SM::global::character::alive::red::crimsonish::INFLUENCE_AT_DOG~::[else]} */
            else {
                me->DoganScale += ((const CharacterQEvt*)e)->amount;
                status_ = Q_HANDLED();
            }
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_red);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::red::crimson} ............*/
QState Character_crimson(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::red::crimson} */
        case Q_ENTRY_SIG: {
            Vibro(LONG_VIBRO);
                DISPATCH_ONESHOT(BECOME_CRIMSON);
                ScreenAddBMPToQueue("Crimson.bmp");
                DISPATCH_ONESHOT(DESTROY_KATET);
                SaveState(CRIMSON, false, true);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimson::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("Crimson.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::crimson::BEGIN(MASTER_WHITE)+BASE} */
        case BEGIN(MASTER_WHITE)+BASE_SIG: {
            status_ = Q_TRAN(&Character_crimsonish);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_red);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::alive::red::corrupted} ..........*/
QState Character_corrupted(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::alive::red::corrupted} */
        case Q_ENTRY_SIG: {
            Vibro(LONG_VIBRO);
                DISPATCH_ONESHOT(BECOME_CORRUPTED);
                ScreenAddBMPToQueue("Corrupted.bmp");
                SaveState(CRIMSONISH, false, true);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::corrupted::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("Crimsonish.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::corrupted::BEGIN(MASTER_CANCEL_CORRUPT)+BAS~} */
        case BEGIN(MASTER_CANCEL_CORRUPT)+BASE_SIG: {
            status_ = Q_TRAN(&Character_crimsonish);
            break;
        }
        /* ${SMs::Character::SM::global::character::alive::red::corrupted::BEGIN(BECOME_CRIMSON_SERVANT)+BA~} */
        case BEGIN(BECOME_CRIMSON_SERVANT)+BASE_SIG: {
            status_ = Q_TRAN(&Character_crimson);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_red);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::dead} ...........................*/
QState Character_dead(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::dead} */
        case Q_ENTRY_SIG: {
            DISPATCH_ONESHOT(DEATH);
                DISPATCH_ONESHOT(DESTROY_KATET);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::BEGIN(MASTER_ALIVE)+BASE} */
        case BEGIN(MASTER_ALIVE)+BASE_SIG: {
            status_ = Q_TRAN(&Character_neutral);
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_character);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::dead::neutral_dead} .............*/
QState Character_neutral_dead(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::dead::neutral_dead} */
        case Q_ENTRY_SIG: {
            SaveState(NEUTRAL, true, false);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::neutral_dead::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("Neutral.bmp");
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_dead);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::dead::white_dead} ...............*/
QState Character_white_dead(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::dead::white_dead} */
        case Q_ENTRY_SIG: {
            SaveState(WHITE, true, false);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::white_dead::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("White.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::white_dead::BEGIN(WHITE_FEELING)+BASE} */
        case BEGIN(WHITE_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(WHITE_JOY);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::white_dead::BEGIN(CRIMSON_FEELING)+BASE} */
        case BEGIN(CRIMSON_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(WHITE_SORROW);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_dead);
            break;
        }
    }
    return status_;
}
/*${SMs::Character::SM::global::character::dead::crimson_dead} .............*/
QState Character_crimson_dead(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::global::character::dead::crimson_dead} */
        case Q_ENTRY_SIG: {
            SaveState(CRIMSON, true, true);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::crimson_dead::BEGIN(SHOW_DOGAN_STATE)+BASE} */
        case BEGIN(SHOW_DOGAN_STATE)+BASE_SIG: {
            ScreenShowPicture("Crimson.bmp");
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::crimson_dead::BEGIN(WHITE_FEELING)+BASE} */
        case BEGIN(WHITE_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_GRIEVE);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::crimson_dead::BEGIN(CRIMSON_FEELING)+BASE} */
        case BEGIN(CRIMSON_FEELING)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_LAUGH);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::crimson_dead::BEGIN(CRIMSON_BROADCAST1)+BASE} */
        case BEGIN(CRIMSON_BROADCAST1)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST1);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::crimson_dead::BEGIN(CRIMSON_BROADCAST2)+BASE} */
        case BEGIN(CRIMSON_BROADCAST2)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST2);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::crimson_dead::BEGIN(CRIMSON_BROADCAST3)+BASE} */
        case BEGIN(CRIMSON_BROADCAST3)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST3);
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Character::SM::global::character::dead::crimson_dead::BEGIN(CRIMSON_BROADCAST4)+BASE} */
        case BEGIN(CRIMSON_BROADCAST4)+BASE_SIG: {
            DISPATCH_BEGIN(CRIMSON_BROADCAST4);
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Character_dead);
            break;
        }
    }
    return status_;
}

#ifdef DESKTOP
/*${SMs::Character::SM::final} .............................................*/
QState Character_final(Character * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Character::SM::final} */
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






/*****************************************************************************
* Model: Character.qm
* File:  ./character.h
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
/*${.::character.h} ........................................................*/
#ifndef character_h
#define character_h
#ifdef __cplusplus
extern "C" {
#endif

#include "qhsm.h"    /* include own framework */
#include "localcharacter.h"

#define NEUTRAL 0
#define CRIMSON -2
#define CRIMSONISH -1
#define WHITISH 1
#define WHITE 2
#define WHITE_THRESHOLD 120 // 3600
#define CRIMSON_THRESHOLD -150 // -4000

class Dispatcher;

/*${SMs::Character} ........................................................*/
typedef struct {
/* protected: */
    QHsm super;

/* public: */
    Dispatcher* dispatcher;
    QStateHandler StartState;
    int DoganScale;
    bool Todash;
    bool Manni;
} Character;

/* protected: */
QState Character_initial(Character * const me, QEvt const * const e);
QState Character_global(Character * const me, QEvt const * const e);
QState Character_character(Character * const me, QEvt const * const e);
QState Character_alive(Character * const me, QEvt const * const e);
QState Character_neutral(Character * const me, QEvt const * const e);
QState Character_whiten(Character * const me, QEvt const * const e);
QState Character_white(Character * const me, QEvt const * const e);
QState Character_whitish(Character * const me, QEvt const * const e);
QState Character_red(Character * const me, QEvt const * const e);
QState Character_crimsonish(Character * const me, QEvt const * const e);
QState Character_crimson(Character * const me, QEvt const * const e);
QState Character_corrupted(Character * const me, QEvt const * const e);
QState Character_dead(Character * const me, QEvt const * const e);
QState Character_neutral_dead(Character * const me, QEvt const * const e);
QState Character_white_dead(Character * const me, QEvt const * const e);
QState Character_crimson_dead(Character * const me, QEvt const * const e);

#ifdef DESKTOP
QState Character_final(Character * const me, QEvt const * const e);
#endif /* def DESKTOP */





typedef struct CharacterQEvt {
    QEvt super;
    int amount;
} CharacterQEvt;


/*${SMs::Character_ctor} ...................................................*/
void Character_ctor(
    Character* me,
    Dispatcher* dispatcher,
    int Dogan,
    bool Manni,
    bool Corrupted,
    bool Dead);

#ifdef __cplusplus
}
#endif
#endif /* character_h */

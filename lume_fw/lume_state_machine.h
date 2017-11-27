/*****************************************************************************
* Model: lume_state_machine.qm
* File:  ./lume_state_machine.h
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
/*${.::lume_state_machine.h} ...............................................*/
    #ifndef lume_state_machine_h
    #define lume_state_machine_h
    #ifdef __cplusplus
    extern "C" {
    #endif

    #include "qpc.h"    /* include own framework */

    #define BUTTON_OFFSET 7

typedef struct LumeQEvt{
    QEvt super;
    void* Ptr;
    int32_t Value;
 } LumeQEvt;

enum PlayerSignals {
TICK_SEC_SIG = Q_USER_SIG,

SHELL_COMMAND_SIG,
LUM_CHANGED_SIG,
BTN_UP_SIG,
BTN_DOWN_SIG,
BTN_PLUS_SIG,
BTN_MINUS_SIG,
TIMEOUT_SIG,
BUTTON_PRESSED_SIG,

LAST_USER_SIG
};

    extern QHsm * const the_lume_state_machine; /* opaque pointer to the lume_state_machine HSM */

/*${SMs::Lume_state_machine_ctor} ..........................................*/
void Lume_state_machine_ctor(void);


    #ifdef __cplusplus
    }
    #endif
    #endif /* lume_state_machine_h */

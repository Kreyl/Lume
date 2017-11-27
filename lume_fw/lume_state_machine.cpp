/*****************************************************************************
* Model: lume_state_machine.qm
* File:  ./lume_state_machine.cpp
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
/*${.::lume_state_machine.cpp} .............................................*/
#include <EventsHandlers.h>
#include "qpc.h"
#include "lume_state_machine.h"
#include "kl_lib.h"
#include "lcd1200.h"
#include "kl_time.h"
#include "interface.h"
#include "Mirilli.h"
#include "kl_adc.h"
#include "lume_state_machine.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

//Q_DEFINE_THIS_FILE



/*${SMs::Lume_state_machine} ...............................................*/
typedef struct {
/* protected: */
    QHsm super;
} Lume_state_machine;

/* protected: */
static QState Lume_state_machine_initial(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_global(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_Lume(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_idle(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_Menu(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_second_screen(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_hours(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_brightness_high(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_brightness_low(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_minutes(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_threshold(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_background(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_day(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_color_hour(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_color_minute(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_month(Lume_state_machine * const me, QEvt const * const e);
static QState Lume_state_machine_year(Lume_state_machine * const me, QEvt const * const e);

#ifdef DESKTOP
static QState Lume_state_machine_final(Lume_state_machine * const me, QEvt const * const e);
#endif /* def DESKTOP */



static Lume_state_machine lume_state_machine; /* the only instance of the Lume_state_machine class */



/* global-scope definitions -----------------------------------------*/
QHsm * const the_lume_state_machine = (QHsm *) &lume_state_machine; /* the opaque pointer */

/*${SMs::Lume_state_machine_ctor} ..........................................*/
void Lume_state_machine_ctor(void) {
    Lume_state_machine *me = &lume_state_machine;
     QHsm_ctor(&me->super, Q_STATE_CAST(&Lume_state_machine_initial));
}
/*${SMs::Lume_state_machine} ...............................................*/
/*${SMs::Lume_state_machi~::SM} ............................................*/
static QState Lume_state_machine_initial(Lume_state_machine * const me, QEvt const * const e) {
    /* ${SMs::Lume_state_machi~::SM::initial} */
    return Q_TRAN(&Lume_state_machine_idle);
}
/*${SMs::Lume_state_machi~::SM::global} ....................................*/
static QState Lume_state_machine_global(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {

#ifdef DESKTOP
        /* ${SMs::Lume_state_machi~::SM::global::TERMINATE} */
        case TERMINATE_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_final);
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
/*${SMs::Lume_state_machi~::SM::global::Lume} ..............................*/
static QState Lume_state_machine_Lume(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::SHELL_COMMAND} */
        case SHELL_COMMAND_SIG: {
            OnCmd((Shell_t*)((LumeQEvt const*)e)->Ptr);
                  ((Shell_t*)((LumeQEvt const*)e)->Ptr)->SignalCmdProcessed();
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_global);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::idle} ........................*/
static QState Lume_state_machine_idle(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::idle} */
        case Q_ENTRY_SIG: {
            //ShowMenuElements
                Interface.Reset();
                Interface.ResetFirstScreen();
                Interface.DisplayDateTime();
                 //SaveSettings
                RTC->BKP1R = Settings.R1;
                RTC->BKP2R = Settings.R2;
                RTC->BKP3R = Settings.R3;
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::idle::TICK_SEC} */
        case TICK_SEC_SIG: {
            Time.GetDateTime();
                 Interface.DisplayDateTime();
                 IndicateNewSecond();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::idle::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            Lcd.Backlight(100);
               TmrMenu.StartOrRestart();
            status_ = Q_TRAN(&Lume_state_machine_hours);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::idle::BTN_UP} */
        case BTN_UP_SIG: {
            Lcd.Backlight(100);
               TmrMenu.StartOrRestart();
                Interface.Reset();
                Interface.ResetSecondScreen();
               Interface.DisplaySecondScreen(CurrentLum);
            status_ = Q_TRAN(&Lume_state_machine_brightness_low);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Lume);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu} ........................*/
static QState Lume_state_machine_Menu(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::TIMEOUT} */
        case TIMEOUT_SIG: {
            ResetColors(ClrH, ClrM, Settings.BckGrnd);
                 Time.SetDateTime();
                 IndicateNewSecond();
                 Lcd.Backlight(0);
                 TmrMenu.Stop();
            status_ = Q_TRAN(&Lume_state_machine_idle);
            break;
        }
        case BUTTON_PRESSED_SIG: {
        	  TmrMenu.StartOrRestart();
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Lume);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::second_screen} ..............*/
static QState Lume_state_machine_second_screen(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case LUM_CHANGED_SIG: {
            	CurrentLum = ((LumeQEvt const*)e)->Value / 2;
            	    if(CurrentLum > 99) CurrentLum = 99;
            	    Interface.DisplayLum(CurrentLum);
                     status_ = Q_HANDLED();
                 break;
                    }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Menu);
            break;
        }

    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::hours} .................*/
static QState Lume_state_machine_hours(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::hours} */
        case Q_ENTRY_SIG: {
            Interface.DisplayHoursInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::hours} */
        case Q_EXIT_SIG: {
            Interface.DisplayHours();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::hours::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            Time.Curr.IncH();
                   Interface.DisplayHoursInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::hours::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            Time.Curr.DecH();
                  Interface.DisplayHoursInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::hours::BTN_UP} */
        case BTN_UP_SIG: {
            TmrMenu.Stop();
                  ResetColors(ClrH, ClrM, Settings.BckGrnd);
                 Time.SetDateTime();
                 IndicateNewSecond();
                 Lcd.Backlight(0);
            status_ = Q_TRAN(&Lume_state_machine_idle);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::hours::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_minutes);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Menu);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_high} .......*/
static QState Lume_state_machine_brightness_high(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_high} */
        case Q_ENTRY_SIG: {
            Interface.DisplayBrHighInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_high} */
        case Q_EXIT_SIG: {
            Interface.DisplayBrHigh();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_high::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            if(Settings.BrtHi == TOP_BRIGHTNESS) Settings.BrtHi = 0;
                                else Settings.BrtHi++;
                 Interface.DisplayBrHighInverted();;
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_high::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            if(Settings.BrtHi == 0) Settings.BrtHi = TOP_BRIGHTNESS;
                                else Settings.BrtHi--;
                 Interface.DisplayBrHighInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_high::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_brightness_low);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_high::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_threshold);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_second_screen);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_low} ........*/
static QState Lume_state_machine_brightness_low(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_low} */
        case Q_ENTRY_SIG: {
            Interface.DisplayBrLowInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_low} */
        case Q_EXIT_SIG: {
            Interface.DisplayBrLow();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_low::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            if(Settings.BrtLo == TOP_BRIGHTNESS) Settings.BrtLo = 0;
                                else Settings.BrtLo++;
                    Interface.DisplayBrLowInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_low::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            if(Settings.BrtLo == 0) Settings.BrtLo = TOP_BRIGHTNESS;
                                else Settings.BrtLo--;
                    Interface.DisplayBrLowInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_low::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            TmrMenu.Stop();
                 Lcd.Backlight(0);
            status_ = Q_TRAN(&Lume_state_machine_idle);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::brightness_low::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_brightness_high);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_second_screen);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::minutes} ...............*/
static QState Lume_state_machine_minutes(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::minutes} */
        case Q_ENTRY_SIG: {
            Interface.DisplayMinutesInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::minutes} */
        case Q_EXIT_SIG: {
            Interface.DisplayMinutes();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::minutes::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            Time.Curr.IncM(); Time.Curr.S = 0;
                Interface.DisplayMinutesInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::minutes::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            Time.Curr.DecM(); Time.Curr.S = 0;
                Interface.DisplayMinutesInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::minutes::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_day);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::minutes::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_hours);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Menu);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::threshold} .............*/
static QState Lume_state_machine_threshold(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::threshold} */
        case Q_ENTRY_SIG: {
            Interface.DisplayThresholdInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::threshold} */
        case Q_EXIT_SIG: {
            Interface.DisplayThreshold();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::threshold::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            if(Settings.Threshold == 99) Settings.Threshold = 0;
                 else Settings.Threshold++;
                 Interface.DisplayThresholdInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::threshold::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            if(Settings.Threshold == 0) Settings.Threshold = 99;
                                else Settings.Threshold--;
                  Interface.DisplayThresholdInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::threshold::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_background);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::threshold::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_brightness_high);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_second_screen);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::background} ............*/
static QState Lume_state_machine_background(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::background} */
        case Q_ENTRY_SIG: {
            Interface.DisplayBackgroundInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::background} */
        case Q_EXIT_SIG: {
            Interface.DisplayBackground();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::background::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            if(Settings.BckGrnd == 99) Settings.BckGrnd = 0;
                 else Settings.BckGrnd++;
            Interface.DisplayBackgroundInverted();
            IndicateNewSecond();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::background::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            if(Settings.BckGrnd == 0) Settings.BckGrnd = 99;
                                else Settings.BckGrnd--;
                  Interface.DisplayBackgroundInverted();
            IndicateNewSecond();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::background::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_color_minute);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::background::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_threshold);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_second_screen);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::day} ...................*/
static QState Lume_state_machine_day(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::day} */
        case Q_ENTRY_SIG: {
            Interface.DisplayDayInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::day} */
        case Q_EXIT_SIG: {
            Interface.DisplayDay();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::day::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            Time.Curr.IncDay();
               Interface.DisplayDayInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::day::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            Time.Curr.DecDay();
                Interface.DisplayDayInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::day::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_month);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::day::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_minutes);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Menu);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_hour} ............*/
static QState Lume_state_machine_color_hour(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_hour} */
        case Q_ENTRY_SIG: {
            Interface.DisplayColorHourInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_hour::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            if(Settings.ClrIdH == 360) Settings.ClrIdH = 0;
                                else Settings.ClrIdH++;
                                ClrH.H = Settings.ClrIdH;
                                IndicateNewSecond();
                  Interface.DisplayColorHourInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_hour::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            if(Settings.ClrIdH == 0) Settings.ClrIdH = 360;
                                else Settings.ClrIdH--;
                                ClrH.H = Settings.ClrIdH;
                                IndicateNewSecond();
                  Interface.DisplayColorHourInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_hour::BTN_UP} */
        case BTN_UP_SIG: {
            Interface.Reset();
            Interface.ResetFirstScreen();
                Interface.DisplayDateTime();
            status_ = Q_TRAN(&Lume_state_machine_year);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_hour::BTN_DOWN} */
        case BTN_DOWN_SIG: {
        	Interface.DisplayColorHour();
        	status_ = Q_TRAN(&Lume_state_machine_color_minute);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_second_screen);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_minute} ..........*/
static QState Lume_state_machine_color_minute(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_minute} */
        case Q_ENTRY_SIG: {
            Interface.DisplayColorMinInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_minute} */
        case Q_EXIT_SIG: {
            Interface.DisplayColorMin();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_minute::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            if(Settings.ClrIdM == 360) Settings.ClrIdM = 0;
                                else Settings.ClrIdM++;
                                ClrM.H = Settings.ClrIdM;
                                IndicateNewSecond();
                 Interface.DisplayColorMinInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_minute::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            if(Settings.ClrIdM == 0) Settings.ClrIdM = 360;
                                else Settings.ClrIdM--;
                                ClrM.H = Settings.ClrIdM;
                                IndicateNewSecond();
                 Interface.DisplayColorMinInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_minute::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_color_hour);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::color_minute::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_background);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_second_screen);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::month} .................*/
static QState Lume_state_machine_month(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::month} */
        case Q_ENTRY_SIG: {
            Interface.DisplayMonthInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::month} */
        case Q_EXIT_SIG: {
            Interface.DisplayMonth();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::month::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            Time.Curr.IncMonth();
                  Interface.DisplayMonthInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::month::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            Time.Curr.DecMonth();
                  Interface.DisplayMonthInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::month::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_year);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::month::BTN_UP} */
        case BTN_UP_SIG: {
            status_ = Q_TRAN(&Lume_state_machine_day);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Menu);
            break;
        }
    }
    return status_;
}
/*${SMs::Lume_state_machi~::SM::global::Lume::Menu::year} ..................*/
static QState Lume_state_machine_year(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::year} */
        case Q_ENTRY_SIG: {
            Interface.DisplayYearInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::year::BTN_PLUS} */
        case BTN_PLUS_SIG: {
            Time.Curr.IncYear();
                Interface.DisplayYearInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::year::BTN_MINUS} */
        case BTN_MINUS_SIG: {
            Time.Curr.DecYear();
                 Interface.DisplayYearInverted();
            status_ = Q_HANDLED();
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::year::BTN_UP} */
        case BTN_UP_SIG: {
        	Interface.DisplayYear();
            status_ = Q_TRAN(&Lume_state_machine_month);
            break;
        }
        /* ${SMs::Lume_state_machi~::SM::global::Lume::Menu::year::BTN_DOWN} */
        case BTN_DOWN_SIG: {
            ResetColors(ClrH, ClrM, Settings.BckGrnd);
                 Time.SetDateTime();
                 IndicateNewSecond();
                 Interface.Reset();
                Interface.ResetSecondScreen();
                Interface.DisplaySecondScreen(CurrentLum);
            status_ = Q_TRAN(&Lume_state_machine_color_hour);
            break;
        }
        default: {
            status_ = Q_SUPER(&Lume_state_machine_Menu);
            break;
        }
    }
    return status_;
}

#ifdef DESKTOP
/*${SMs::Lume_state_machi~::SM::final} .....................................*/
static QState Lume_state_machine_final(Lume_state_machine * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /* ${SMs::Lume_state_machi~::SM::final} */
        case Q_ENTRY_SIG: {
            printf(" Bye! Bye!"); exit(0);
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






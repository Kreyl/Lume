/*
 * Mirilli.cpp
 *
 *  Created on: 30 сент. 2017 г.
 *      Author: Kreyl
 */

#include "kl_lib.h"
#include "Mirilli.h"
//#include "ws2812b.h"
#include "sk6812.h"

Neopixel_t Leds;
static thread_reference_t PThd = nullptr;

// Target colors
static Color_t ITargetClr[LED_CNT];
static bool ColorsDone = true;

static uint32_t ICalcDelayN(uint32_t n, uint32_t SmoothValue) {
    return Leds.ICurrentClr[n].DelayToNextAdj(ITargetClr[n], SmoothValue);
}

static THD_WORKING_AREA(waMirilli, 128);
__noreturn
static void MirilliThread(void *arg) {
    chRegSetThreadName("Mirilli");
    while(true) {
        if(ColorsDone) {
            chSysLock();
            chThdSuspendS(&PThd);
            chSysUnlock();
        }
        else {
            uint32_t Delay = 0;
            for(int32_t i=0; i<LED_CNT; i++) {
                uint32_t tmp = ICalcDelayN(i, SMOOTH_VALUE);  // }
                if(tmp > Delay) Delay = tmp;                  // } Calculate Delay
                Leds.ICurrentClr[i].Adjust(ITargetClr[i]);    // Adjust current color
            } // for
            Leds.ISetCurrentColors();
            if (Delay == 0) ColorsDone = true;  // Setup completed
            else chThdSleepMilliseconds(Delay);
        }
    } // while true
}

void InitMirilli() {
    Leds.Init();
    chThdCreateStatic(waMirilli, sizeof(waMirilli), HIGHPRIO, (tfunc_t)MirilliThread, NULL);
}

void SetTargetClrH(uint32_t H, Color_t Clr) {
    if(H >= MIRILLI_H_CNT) return;
    uint32_t Indx = H2LedN[H];
    ITargetClr[Indx] = Clr;
}
void SetTargetClrM(uint32_t M, Color_t Clr) {
    if(M >= MIRILLI_M_CNT) return;
    uint32_t Indx = M2LedN[M];
    ITargetClr[Indx] = Clr;
}

void WakeMirilli() {
    ColorsDone = false;
    if(PThd == nullptr) return;
    chSysLock();
    chThdResumeS(&PThd, MSG_OK);
    chSysUnlock();
}

void ResetColorsToOffState(Color_t ClrH, Color_t ClrM) {
    Color_t TargetClrH;
    Color_t TargetClrM;
    TargetClrH.SetRGBWBrightness(ClrH, OFF_LUMINOCITY);
    TargetClrM.SetRGBWBrightness(ClrM, OFF_LUMINOCITY);
    for(int32_t i=0; i<12; i++) {
        SetTargetClrH(i, TargetClrH);
        SetTargetClrM(i, TargetClrM);
    }
    SetTargetClrM(12, TargetClrM);  // Extra miril at 12
}

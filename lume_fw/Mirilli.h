/*
 * Mirilli.h
 *
 *  Created on: 30 сент. 2017 г.
 *      Author: Kreyl
 */

#pragma once

#include "color.h"

#define SMOOTH_VALUE    360
#define OFF_LUMINOCITY  1       // Set to 0 if "backlight all" is not required

#define MIRILLI_H_CNT   12
#define MIRILLI_M_CNT   13

// Tables of accordance between hours/minutes and LED indxs
static const uint32_t H2LedN[MIRILLI_H_CNT] = { 0, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
static const uint32_t M2LedN[MIRILLI_M_CNT] = { 12, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13};

// Do not touch
#define LED_CNT         (MIRILLI_H_CNT + MIRILLI_M_CNT)   // Number of WS2812 LEDs

void InitMirilli();
void SetTargetClrH(uint32_t H, Color_t Clr);
void SetTargetClrM(uint32_t M, Color_t Clr);
void WakeMirilli();
void ResetColorsToOffState(Color_t ClrH, Color_t ClrM);

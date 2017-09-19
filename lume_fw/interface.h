/*
 * interface.h
 *
 *  Created on: 22 ����� 2015 �.
 *      Author: Kreyl
 */

#pragma once

#include "lcd1200.h"
#include "kl_time.h"
#include "main.h"

class Interface_t {
public:
    void Reset() {
        Lcd.Printf(0, 0, "�����:  00:00:00");
        Lcd.Printf(0, 1, "����: 0000/00/00");
        Lcd.Printf(0, 2, "������������: 00");
        Lcd.Printf(0, 3, "����� ���-��: 00");
        Lcd.Printf(0, 4, "������� ����: 00");
        Lcd.Printf(0, 5, "������� ����: 00");
        Lcd.Printf(0, 6, "���� �����: 000");
        Lcd.Printf(0, 7, "���� �����: 000");
    }

    void DisplayDateTime(DateTime_t *pDT) {
        if(State == stHours) Lcd.PrintfInverted(8,  0, "%02u", pDT->H);
        else Lcd.Printf(8,  0, "%02u", pDT->H);
        if(State == stMinutes) Lcd.PrintfInverted(11, 0, "%02u", pDT->M);
        else Lcd.Printf(11, 0, "%02u", pDT->M);
        Lcd.Printf(14, 0, "%02u", pDT->S);  // do not touch seconds
        if(State == stYear) Lcd.PrintfInverted(6,  1, "%04u", pDT->Year);
        else Lcd.Printf(6,  1, "%04u", pDT->Year);
        if(State == stMonth) Lcd.PrintfInverted(11, 1, "%02u", pDT->Month);
        else Lcd.Printf(11, 1, "%02u", pDT->Month);
        if(State == stDay) Lcd.PrintfInverted(14, 1, "%02u", pDT->Day);
        else Lcd.Printf(14, 1, "%02u", pDT->Day);
    }

    void DisplayLum(uint32_t Lum) {
        Lcd.Printf(14, 2, "%02u", Lum);
    }

    void DisplayClrH(uint16_t ClrId) {
        if(State == stClrH) Lcd.PrintfInverted(12, 6, "%03u", ClrId);
        else Lcd.Printf(12, 6, "%03u", ClrId);
    }
    void DisplayClrM(uint16_t ClrId) {
        if(State == stClrM) Lcd.PrintfInverted(12, 7, "%03u", ClrId);
        else Lcd.Printf(12, 7, "%03u", ClrId);
    }

    void DisplayBrightness() {
//        if(App.Selected == selBrightness) Lcd.PrintfInverted(9, 7, "%u%%  ", App.BrightnessPercent);
//        else Lcd.Printf(9, 7, "%u%%    ", App.BrightnessPercent);
    }

    void Error(const char* msg) { Lcd.PrintfInverted(0, 0, "%S", msg); }
};

extern Interface_t Interface;

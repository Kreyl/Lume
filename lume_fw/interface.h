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
//        Lcd.Printf(0, 6, "���� �����: 000");
//        Lcd.Printf(0, 7, "���� �����: 000");
    }

    void DisplayDateTime() {
        if(State == stHours) Lcd.PrintfInverted(8,  0, "%02u", Time.Curr.H);
        else Lcd.Printf(8,  0, "%02u", Time.Curr.H);
        if(State == stMinutes) Lcd.PrintfInverted(11, 0, "%02u", Time.Curr.M);
        else Lcd.Printf(11, 0, "%02u", Time.Curr.M);
        Lcd.Printf(14, 0, "%02u", Time.Curr.S);  // do not touch seconds
        if(State == stYear) Lcd.PrintfInverted(6,  1, "%04u", Time.Curr.Year);
        else Lcd.Printf(6,  1, "%04u", Time.Curr.Year);
        if(State == stMonth) Lcd.PrintfInverted(11, 1, "%02u", Time.Curr.Month);
        else Lcd.Printf(11, 1, "%02u", Time.Curr.Month);
        if(State == stDay) Lcd.PrintfInverted(14, 1, "%02u", Time.Curr.Day);
        else Lcd.Printf(14, 1, "%02u", Time.Curr.Day);
    }

    void DisplayLum(uint32_t Lum) {
        Lcd.Printf(14, 2, "%02u", Lum);
    }

    void DisplayThreshold() {
        if(State == stThreshold) Lcd.PrintfInverted(14, 3, "%02u", Settings.Threshold);
        else Lcd.Printf(14, 3, "%02u", Settings.Threshold);
    }
    void DisplayBrtHi() {
        if(State == stBrtHi) Lcd.PrintfInverted(14, 4, "%02u", Settings.BrtHi);
        else Lcd.Printf(14, 4, "%02u", Settings.BrtHi);
    }
    void DisplayBrtLo() {
        if(State == stBrtLo) Lcd.PrintfInverted(14, 5, "%02u", Settings.BrtLo);
        else Lcd.Printf(14, 5, "%02u", Settings.BrtLo);
    }

//    void DisplayClrH() {
//        if(State == stClrH) Lcd.PrintfInverted(12, 6, "%03u", Settings.ClrIdH);
//        else Lcd.Printf(12, 6, "%03u", Settings.ClrIdH);
//    }
//    void DisplayClrM() {
//        if(State == stClrM) Lcd.PrintfInverted(12, 7, "%03u", Settings.ClrIdM);
//        else Lcd.Printf(12, 7, "%03u", Settings.ClrIdM);
//    }

    void Error(const char* msg) { Lcd.PrintfInverted(0, 0, "%S", msg); }
};

extern Interface_t Interface;

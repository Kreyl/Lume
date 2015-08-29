/*
 * interface.h
 *
 *  Created on: 22 ����� 2015 �.
 *      Author: Kreyl
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "lcd1200.h"
#include "main.h"
#include "kl_time.h"
#include "stones.h"

class Interface_t {
public:
    void Reset() {
        Lcd.Printf(11, 0, APP_VERSION);
        Lcd.Printf(0, 2, "�����: 00:00:00");
        Lcd.Printf(0, 3, "���:");
        Lcd.Printf(0, 4, "�����:");
        Lcd.Printf(0, 5, "����:");
        Lcd.Printf(0, 7, "�������:");
    }
    void DisplayTime() {
        if(App.Selected == selH) Lcd.PrintfInverted(7, 2, "%02u", App.dtNow.H);
        else Lcd.Printf(7, 2, "%02u", App.dtNow.H);
        if(App.Selected == selM) Lcd.PrintfInverted(10, 2, "%02u", App.dtNow.M);
        else Lcd.Printf(10, 2, "%02u", App.dtNow.M);
        if(App.Selected == selS) Lcd.PrintfInverted(13, 2, "%02u", App.dtNow.S);
        else Lcd.Printf(13, 2, "%02u", App.dtNow.S);

        if(App.Selected == selYear) Lcd.PrintfInverted(7, 3, "%04u", App.dtNow.Year);
        else Lcd.Printf(7, 3, "%04u", App.dtNow.Year);
        if(App.Selected == selMonth) Lcd.PrintfInverted(7, 4, "%02u", App.dtNow.Month);
        else Lcd.Printf(7, 4, "%02u", App.dtNow.Month);
        if(App.Selected == selDay) Lcd.PrintfInverted(7, 5, "%02u", App.dtNow.Day);
        else Lcd.Printf(7, 5, "%02u", App.dtNow.Day);
    }

    void DisplayBrightness() {
        if(App.Selected == selBrightness) Lcd.PrintfInverted(9, 7, "%u%%  ", App.BrightnessPercent);
        else Lcd.Printf(9, 7, "%u%%    ", App.BrightnessPercent);
    }

    void Error(const char* msg) { Lcd.PrintfInverted(0, 2, "%S", msg); }
};

extern Interface_t Interface;

#endif /* INTERFACE_H_ */

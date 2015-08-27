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

class Interface_t {
public:
    void Reset() {
        Lcd.Printf(11, 0, APP_VERSION);
        Lcd.Printf(0, 2, "�����:");
        Lcd.Printf(0, 4, "���:");
        Lcd.Printf(0, 5, "�����:");
        Lcd.Printf(0, 6, "����:");
//        Lcd.Symbols(0, 5, LineHorizDouble, 6, LineHorizDoubleUp, 1, LineHorizDouble, 9, 0);
//        for(int i=0; i<5; i++) Lcd.Symbols(6, i, ((i == 2)? LineVertDoubleLeft : LineVertDouble), 1,0);
//        Lcd.Symbols(0, 2, LineHorizDouble, 6, 0);
    }

    void Error(const char* msg) { Lcd.PrintfInverted(0, 2, "%S", msg); }
};

extern Interface_t Interface;

#endif /* INTERFACE_H_ */

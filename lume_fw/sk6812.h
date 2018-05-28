/*
 * sk6812.h
 *
 *  Created on: 19-07-2017 ã.
 *      Author: Kreyl
 */

#pragma once

/*
 * ========== SK2812 control module ==========
 * Only basic command "SetCurrentColors" is implemented, all other is up to
 * higher level software.
 * SPI input frequency should be 8 MHz (which results in 4MHz bitrate)
 */


#include "ch.h"
#include "hal.h"
#include "kl_lib.h"
#include "color.h"
#include "uart.h"
#include "Mirilli.h"

#define SEQ_LEN             4L   // SPI bits per single bit of LED data
#define RST_W_CNT           8L   // zero words before and after data to produce reset

// SPI16 Buffer (no tuning required)
#define DATA_BIT_CNT        (LED_CNT * 4 * 8 * SEQ_LEN) // Each led has 4 channels 8 bit each
#define DATA_W_CNT          ((DATA_BIT_CNT + 15) / 16)  // Data len in 16-bit words
#define TOTAL_W_CNT         (DATA_W_CNT + RST_W_CNT)

class Neopixel_t {
private:
    Spi_t ISpi {LEDWS_SPI};
    uint16_t IBuf[TOTAL_W_CNT];
    uint16_t *PBuf;
    void AppendBitsMadeOfByte(uint8_t Byte);
public:
    void Init();
    bool AreOff() {
        for(uint8_t i=0; i<LED_CNT; i++) {
            if(ICurrentClr[i] != clRGBWBlack) return false;
        }
        return true;
    }
    // Inner use
    Color_t ICurrentClr[LED_CNT];
    void ISetCurrentColors();
    void ITmrHandlerI();
};

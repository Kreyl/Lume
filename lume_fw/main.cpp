#include <EventHandlers.h>
#include "hal.h"
#include "MsgQ.h"
#include "shell.h"
#include "kl_lib.h"
#include "SimpleSensors.h"
#include "buttons.h"
#include "lcd1200.h"
#include "led.h"
#include "kl_time.h"
#include "interface.h"
#include "main.h"
#include "ws2812b.h"
#include "Mirilli.h"
#include "kl_adc.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
extern CmdUart_t Uart;
void ITask();


Interface_t Interface;


#endif

int main(void) {
    // ==== Init Clock system ====
    Clk.UpdateFreqValues();

    // === Init OS ===
    halInit();
    chSysInit();

    // ==== Init hardware ====
    EvtQMain.Init();
    Uart.Init(115200);
    Printf("\r%S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
    Clk.PrintFreqs();

    SimpleSensors::Init();
    chThdSleepMilliseconds(180); // Let power to stabilize
    // LEDs: must be set up before LCD to allow DMA irq
    InitMirilli();

    PinSetupOut(LCD_PWR, omPushPull);
    PinSetHi(LCD_PWR);
    chThdSleepMilliseconds(18);
    Lcd.Init();

    // Time and backup space
    BackupSpc::EnableAccess();
    InitSettings();
    Interface.Reset();
    Time.Init();

    // Adc
    PinSetupAnalog(LUM_MEAS_PIN);
    Adc.Init();
    Adc.EnableVRef();

    // Main cycle
    ITask();
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
//        Printf("%u\r", Msg.ID);
        switch(Msg.ID) {
            case evtIdShellCmd:
                break;

            case evtIdEverySecond:
                break;

            case evtIdAdcRslt:
                break;

            case evtIdButtons:
                // Individual buttons
                switch(Msg.BtnEvtInfo.BtnID) {
                    default: break;
                }
                break;


        } // switch
    } // while true
}

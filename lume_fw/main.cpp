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
#include "lume_state_machine.h"
#include "qpc.h"

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
    //Enable state machine
    Lume_state_machine_ctor();
    QMSM_INIT(the_lume_state_machine, (QEvt *)0);
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        Printf("%u\r", Msg.ID);
        LumeQEvt e;
        e.super.sig = 0;
        switch(Msg.ID) {
            case evtIdShellCmd:
                e.super.sig = SHELL_COMMAND_SIG;
                e.Ptr = Msg.Ptr;
                break;

            case evtIdEverySecond: e.super.sig = TICK_SEC_SIG; break;

            case evtIdAdcRslt:
                e.super.sig = LUM_CHANGED_SIG;
                e.Value = Msg.Value;
                break;

            case evtIdButtons:
                // Individual buttons
                switch(Msg.BtnEvtInfo.BtnID) {
                    case 0: e.super.sig = BTN_UP_SIG; break;
                    case 1: e.super.sig = BTN_DOWN_SIG; break;
                    case 2: e.super.sig = BTN_PLUS_SIG; break;
                    case 3: e.super.sig = BTN_MINUS_SIG; break;
                    default: break;
                }
                QMSM_DISPATCH(the_lume_state_machine, &(e.super));
                // Common event
                e.super.sig = BUTTON_PRESSED_SIG;
                break;


        } // switch

//        e.super.sig = Msg.ID;
//        e.Ptr = Msg.Ptr;
//        e.Value = Msg.Value;
        if(e.super.sig != 0) {
            QMSM_DISPATCH(the_lume_state_machine,  &(e.super));
        }
    }
}

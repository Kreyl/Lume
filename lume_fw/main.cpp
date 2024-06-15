#include "hal.h"
#include "MsgQ.h"
#include "shell.h"
#include "kl_lib.h"
#include "SimpleSensors.h"
#include "buttons.h"
#include "lcd1200.h"
#include "led.h"
#include "interface.h"
#include "kl_servo.h"

#if 1 // ======================== Variables and defines ========================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
static const UartParams_t CmdUartParams(115200, CMD_UART_PARAMS);
CmdUart_t Uart{CmdUartParams};
void OnCmd(Shell_t *PShell);
void ITask();

//Interface_t Interface;
Servo_t srv{SERVO_PIN, 360, 2280};
bool move_it = false;
int32_t angle1 = 45, angle2 = 120, curr_angle;;
TmrKL_t tmr_move(TIME_MS2I(999), evtIdMoveIt, tktPeriodic);

uint32_t press_cnt;
#endif

void DisplayCnt() {
    Lcd.PrintfFont(Times_New_Roman18x16, 0, 3, "%u    ", press_cnt);
    Printf("Cnt: %u\r", press_cnt);
}

void main(void) {
    // ==== Init Clock system ====
    Clk.EnablePrefetch();
    Clk.UpdateFreqValues();

    // === Init OS ===
    halInit();
    chSysInit();

    // ==== Init hardware ====
    EvtQMain.Init();
    Uart.Init();
    Printf("\r%S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
    Clk.PrintFreqs();

    chThdSleepMilliseconds(180); // Let power to stabilize
    SimpleSensors::Init();

    // LCD
    PinSetupOut(LCD_PWR, omPushPull);
    PinSetHi(LCD_PWR);
    chThdSleepMilliseconds(18);
    Lcd.Init();
    Lcd.BacklightOn();

    // Time and backup space
    BackupSpc::EnableAccess();
    press_cnt = BackupSpc::ReadRegister(0);
    DisplayCnt();
    Lcd.Print(0, 0, "Start");
    Lcd.Print(0, 7, "Reset");

    // Servo
    srv.Init();
    tmr_move.StartOrRestart();

    // Main cycle
    ITask();
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
        switch(Msg.ID) {
            case evtIdShellCmdRcvd:
                while(((CmdUart_t*)Msg.Ptr)->TryParseRxBuff() == retvOk) OnCmd((Shell_t*)((CmdUart_t*)Msg.Ptr));
                break;

            case evtIdButtons:
                Printf("Btn %u %u\r", Msg.BtnEvtInfo.BtnID, Msg.BtnEvtInfo.Type);
                switch(Msg.BtnEvtInfo.BtnID) {
                    case 0:
                        move_it = !move_it;
                        Lcd.Print(0, 0, move_it? "Stop ": "Start");
                        break;
                    case 1:
                        press_cnt = 0;
                        BackupSpc::WriteRegister(0, press_cnt);
                        DisplayCnt();
                        break;
//                    case 2: e.super.sig = BTN_PLUS_SIG; break;
//                    case 3: e.super.sig = BTN_MINUS_SIG; break;
                    default: break;
                }
                break;

            case evtIdMoveIt:
                if(move_it) {
                    curr_angle = (curr_angle == angle1)? angle2 : angle1;
                    srv.SetAngle_dg(curr_angle);
                }
                break;
        } // switch
    }
}


void OnCmd(Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
//    Printf("%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("Ping")) PShell->Ok();
    else if(PCmd->NameIs("Version")) PShell->Print("Version: %S %S\r", APP_NAME, XSTRINGIFY(BUILD_TIME));
    else if(PCmd->NameIs("mem")) PrintMemoryInfo();

    else if(PCmd->NameIs("Set")) {
        int32_t v;
        if(PCmd->GetNext(&v) == retvOk) {
            srv.SetAngle_dg(v);
            PShell->Ok();
        }
        else PShell->BadParam();
    }
    else if(PCmd->NameIs("Setus")) {
        int32_t v;
        if(PCmd->GetNext(&v) == retvOk) {
            srv.Set_us(v);
            PShell->Ok();
        }
        else PShell->BadParam();
    }

    else if(PCmd->NameIs("MoveIt")) {
        int32_t a1, a2;
        if(PCmd->GetParams<int32_t>(2, &angle1, &angle2) == retvOk) {
            move_it = true;
            PShell->Ok();
        }
    }
    else if(PCmd->NameIs("Stop")) {
        move_it = false;
        PShell->Ok();
    }

    else PShell->CmdUnknown();
}

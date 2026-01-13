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
#endif

#if 1 // ==== Non-volatile data ====
static const uint32_t knv_data_sz = 5, kaddr_cnt = 0;
union NVData {
    uint32_t arr[knv_data_sz];
    struct {
        uint32_t cnt;
        uint32_t up_pos_us, down_pos_us;
        uint32_t up_period_ms, down_period_ms;
    };
    void Load() {
        for(uint32_t i=0; i<knv_data_sz; i++) arr[i] = BackupSpc::ReadRegister(i);
        if(up_period_ms == 0 or down_period_ms == 0) {
            up_period_ms = 999;
            down_period_ms = 270;
        }
    }
    void Save() { for(uint32_t i=0; i<knv_data_sz; i++) BackupSpc::WriteRegister(i, arr[i]); }
    void SaveCnt() { BackupSpc::WriteRegister(kaddr_cnt, cnt); }
} nv_data;
#endif

#if 1 // ==== State machine ====
TmrKL_t tmr_move(TIME_MS2I(999), evtIdMoveIt, tktOneShot);
TmrKL_t tmr_show_act(TIME_MS2I(306), evtIdShowActivity, tktPeriodic);
static bool move_it = false;
#endif

#if 1 // ==== Interface ====
void DisplayAndSaveCnt() {
    nv_data.SaveCnt();
    Lcd.FillArea(0, 3, 96, 2, 0); // x, y, width, height, filler
    Lcd.PrintfFont(Times_New_Roman18x16, 0, 3, "N=%u", nv_data.cnt);
//    Printf("Cnt: %u\r", nv_data.cnt);
}


const uint32_t kblinker_x = 0, kblinker_y = 1;
void DisplayStartOrStop() {
    Lcd.Print(0, 0, move_it? "Stop ": "Start");
    if(!move_it) Lcd.Print(kblinker_x, kblinker_y, "     ");
}

void DisplayActivity() {
    static uint32_t sta = 0;
    switch(sta) {
        case 0: Lcd.Print(kblinker_x, kblinker_y, ".    "); break;
        case 1: Lcd.Print(kblinker_x, kblinker_y, "..   "); break;
        case 2: Lcd.Print(kblinker_x, kblinker_y, "...  "); break;
        case 3: Lcd.Print(kblinker_x, kblinker_y, ".... "); break;
        case 4: Lcd.Print(kblinker_x, kblinker_y, "....."); break;
        default: break;
    }
    sta++;
    if(sta > 4) sta = 0;
}

void DisplayShowSettings() {
    const uint32_t kx = 6;
    // Clean it up
    Lcd.FillArea(kx*6, 0, 96 - kx*6, 2, 0); // x, y, width, height, filler
    Lcd.FillArea(kx*6, 6, 96 - kx*6, 2, 0); // x, y, width, height, filler
    // Print
    Lcd.Print(kx, 0, "msU:%u",   nv_data.up_period_ms);
    Lcd.Print(kx, 1, "msD:%u", nv_data.down_period_ms);
    Lcd.Print(kx, 6, "usUp:%u",   nv_data.up_pos_us);
    Lcd.Print(kx, 7, "usDown:%u", nv_data.down_pos_us);
}
#endif

static class PressSrv : private Servo_t {
public:
    enum class Position {Up, Down} curr_pos = Position::Up;
    PressSrv() : Servo_t(SERVO_PIN, 333, 760, 2280) {}
    void Init() { Servo_t::Init(0); }
    void Set_us(uint32_t us) { Servo_t::Set_us(us); }
    void MoveUp() {
        Set_us(nv_data.up_pos_us);
        curr_pos = Position::Up;
    }
    void MoveDown() {
        Set_us(nv_data.down_pos_us);
        curr_pos = Position::Down;
    }
} srv;



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

    BackupSpc::EnableAccess(); // Time and backup space
    nv_data.Load();
    DisplayShowSettings();
    DisplayAndSaveCnt();
    DisplayStartOrStop();
    Lcd.Print(0, 7, "Reset");

    srv.Init();  // Servo
    srv.MoveUp();

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
//                Printf("Btn %u %u\r", Msg.BtnEvtInfo.BtnID, Msg.BtnEvtInfo.Type);
                switch(Msg.BtnEvtInfo.BtnID) {
                    case 0:
                        srv.MoveUp(); // Always
                        move_it = !move_it;
                        DisplayStartOrStop();
                        if(move_it) {
                            tmr_move.StartOrRestart(TIME_MS2I(nv_data.up_period_ms));
                            tmr_show_act.StartOrRestart();
                        }
                        else {
                            tmr_move.Stop();
                            tmr_show_act.Stop();
                        }
                        break;
                    case 1:
                        nv_data.cnt = 0;
                        DisplayAndSaveCnt();
                        break;
                    default: break;
                }
                break;

            case evtIdMoveIt:
                if(move_it) {
                    if(srv.curr_pos == srv.Position::Up) {
                        srv.MoveDown();
                        tmr_move.StartOrRestart(TIME_MS2I(nv_data.down_period_ms));
                        nv_data.cnt++;
                        DisplayAndSaveCnt();
                    }
                    else { // Down position
                        srv.MoveUp();
                        tmr_move.StartOrRestart(TIME_MS2I(nv_data.up_period_ms));
                    }
                }
                break;

            case evtIdShowActivity: DisplayActivity(); break;
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

    else if(PCmd->NameIs("Setus")) {
        int32_t v;
        if(PCmd->GetNext(&v) == retvOk) {
            srv.Set_us(v);
            PShell->Ok();
        }
        else PShell->BadParam();
    }

    else if(PCmd->NameIs("MoveIt")) {
        tmr_move.Stop();
        srv.MoveUp();
        if(PCmd->GetNext<uint32_t>(&nv_data.up_pos_us) == retvOk and
           PCmd->GetNext<uint32_t>(&nv_data.down_pos_us) == retvOk and
           PCmd->GetNext<uint32_t>(&nv_data.up_period_ms) == retvOk and
           PCmd->GetNext<uint32_t>(&nv_data.down_period_ms) == retvOk
        ) {
            nv_data.Save();
            DisplayShowSettings();
            tmr_move.StartOrRestart(TIME_MS2I(nv_data.up_period_ms));
            move_it = true;
            DisplayStartOrStop();
            PShell->Ok();
        }
        else PShell->BadParam();
    }
    else if(PCmd->NameIs("Stop")) {
        move_it = false;
        tmr_move.Stop();
        tmr_show_act.Stop();
        srv.MoveUp();
        DisplayStartOrStop();
        PShell->Ok();
    }

    else PShell->CmdUnknown();
}

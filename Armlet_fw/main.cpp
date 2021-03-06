/*
 * File:   main.cpp
 *
 * Created on May 27, 2016, 6:37 PM
 */

#include "usb_msdcdc.h"
#include "hal.h"
#include "MsgQ.h"
#include "kl_lib.h"
#include "Sequences.h"
#include "shell.h"
#include "radio_lvl1.h"
#include "lcd2630.h"
#include "kl_sd.h"
#include "DrawBmp.h"
#include "kl_fs_utils.h"
#include "beeper.h"
#include "vibro.h"
#include "SlotPlayer.h"
#include "kl_adc.h"
#include "pill.h"
#include "pill_mgr.h"
#include "dispatcher.h"
#include "bsp.h"
#include "battery_consts.h"

#define LOGIC_EN

#if 1 // =============== Low level ================
// Forever
EvtMsgQ_t<EvtMsg_t, MAIN_EVT_Q_LEN> EvtQMain;
extern CmdUart_t Uart;
void OnCmd(Shell_t *PShell);
void ITask();

uint16_t ID = 7;
uint8_t Influence = 16;//ID + 128;

Beeper_t Beeper(BEEPER_PIN);
Vibro_t Vibra(VIBRO_PIN);

static TmrKL_t TmrOneSecond {MS2ST(999), evtIdEverySecond, tktPeriodic}; // Measure battery periodically

class Power_t {
public:
    bool ExternalPwrOn() { return  PinIsHi(USB_DETECT_PIN); }
    bool IsCharging()    { return !PinIsHi(IS_CHARGING_PIN); }
    void Init() {
        // Battery
        PinSetupAnalog(BAT_MEAS_PIN);
        Adc.Init();
        // Charging
        PinSetupInput(IS_CHARGING_PIN, pudPullUp);
    }
};
static Power_t Power;
#endif

#if 1 // ===================== Logic ======================
// For csv parser
size_t ReadCallback(void *file_context, uint8_t *buffer, size_t length);

Dispatcher dispatcher;
CsvTable csvTable {ReadCallback};
EmotionTable emoTable;
InfluenceTable infTable;
CharacterTable charTable;
LocalCharacter localChar;
__unused static int8_t RadioTrs[4] = { 127, -45, -72, -81 }; // First is "reach never"
__unused static bool IsLogicInitOk = true;
extern uint8_t ChargePercent;
extern bool IsCharging;
uint8_t PrevChargePercent = 110;
void DrawBattery();

#define DISCH_BAT_VIBR_PERIOD_S     600
systime_t WhenDischargedBatteryIndicated = 0;

char SelfName[36];
#endif

#define BTN_TIMER_REG_NUM   1

int main() {
    // Check if IWDG reset occured => power-off occured
    if(Iwdg::ResetOccured()) {
        PinSetupInput(BTN_PWRON, pudPullUp);    // Setup key input
        BackupSpc::EnableAccess();
        if(PinIsHi(BTN_PWRON)) {    // if btn is not pressed, go back to sleep
            BackupSpc::WriteRegister(BTN_TIMER_REG_NUM, 0); // Reset timer
            PowerOff();
        }
        else { // Btn is pressed, check timeout
            uint32_t cnt = BackupSpc::ReadRegister(BTN_TIMER_REG_NUM);
            if(cnt < 2) {
                cnt++;
                BackupSpc::WriteRegister(BTN_TIMER_REG_NUM, cnt);
                PowerOff();
            }
        }
        // Time to wakeup
        BackupSpc::WriteRegister(BTN_TIMER_REG_NUM, 0); // Reset timer
        BackupSpc::DisableAccess();
    }

#if 1 // Low level init
    // ==== Setup clock ====
    Clk.SetCoreClk(cclk24MHz);
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    EvtQMain.Init();
    Uart.Init(115200);
    Printf("\r%S %S\r\n", APP_NAME, BUILD_TIME);
    Clk.PrintFreqs();

    Lcd.Init();
    Lcd.Cls(clBlack);

    SD.Init();

    SimpleSensors::Init();
    Power.Init();

    i2c2.Init();
    PillMgr.Init();

//    Beeper.Init();
//    Beeper.StartOrRestart(bsqBeepBeep);
    Vibra.Init(VIBRO_TIM_FREQ);

    SlotPlayer::Init();
    UsbMsdCdc.Init();

    TmrOneSecond.StartOrRestart();

#endif

#ifdef LOGIC_EN // ==== Logic init ====
    // Load Radio tresholds
    RadioTrs[0] = 127;  // Reach it never
    if(csv::OpenFile("Tresholds.csv") == retvOk) {
        while(csv::ReadNextLine() == retvOk) {
            char *Name;
            if(csv::GetNextToken(&Name) != retvOk) continue;
            csv::TryLoadParam<int8_t>(Name, "Near", &RadioTrs[1]);
            csv::TryLoadParam<int8_t>(Name, "Medium", &RadioTrs[2]);
            csv::TryLoadParam<int8_t>(Name, "Far", &RadioTrs[3]);
        }
        csv::CloseFile();
        Printf("Radio Tresholds: %d; %d; %d\r", RadioTrs[1], RadioTrs[2], RadioTrs[3]);
    }
    else {
        IsLogicInitOk = false;
        Lcd.Printf(0, 0, clGreen, clBlack, "No Radio tresholds");
    }

    // Open Emotions
    if(TryOpenFileRead("Emotions.csv", &CommonFile) == retvOk) {
        emoTable.init(&CommonFile, &csvTable);
        CloseFile(&CommonFile);
    }
    else {
        IsLogicInitOk = false;
        Lcd.Printf(0, 0, clGreen, clBlack, "No Emotions");
    }

    // Open Influence
    if(TryOpenFileRead("Reasons.csv", &CommonFile) == retvOk) {
        infTable.init(&CommonFile, &csvTable, &emoTable, RadioTrs);
        CloseFile(&CommonFile);
    }
    else {
        IsLogicInitOk = false;
        Lcd.Printf(0, 0, clGreen, clBlack, "No Reasons");
    }

    // Get Self name
    if(csv::OpenFile("SelfName.csv") == retvOk) {
        if(csv::ReadNextLine() != retvOk) chSysHalt("Bad Name File1");
        char *Name;
        if(csv::GetNextToken(&Name) != retvOk) chSysHalt("Bad Name File2");
        if(csv::GetNextCellString(SelfName) != retvOk) chSysHalt("Bad Name File3");
        Printf("Self name: %S\r", SelfName);
        csv::CloseFile();
    }
    else {
        IsLogicInitOk = false;
        Lcd.Printf(0, 0, clGreen, clBlack, "No Name");
    }

    // Character table
    if(TryOpenFileRead("Characters.csv", &CommonFile) == retvOk) {
        charTable.init(&CommonFile, &csvTable, SelfName, &localChar);
        CloseFile(&CommonFile);
    }
    else {
        IsLogicInitOk = false;
        Lcd.Printf(0, 0, clGreen, clBlack, "No Characters");
    }

    // Get ID
    ID = localChar.id;
    Printf("ID: %u\r", ID);
    Influence = localChar.id + CharacterTable::FIRST_CHARACTER;

    // Load State: Dogan, Dead, Corrupted
    if(csv::OpenFile("State.csv") == retvOk) {
        while(csv::ReadNextLine() == retvOk) {
            char *Name;
            if(csv::GetNextToken(&Name) != retvOk) continue;
            csv::TryLoadParam<int>(Name, "Dogan", &localChar.dogan);
            csv::TryLoadParam<bool>(Name, "Dead", &localChar.dead);
            csv::TryLoadParam<bool>(Name, "Corrupted", &localChar.corrupted);
        }
        csv::CloseFile();
        Printf("Dogan: %d; Dead: %u; Corrupted: %u\r", localChar.dogan, localChar.dead, 0);
    }

    // Load KatetLinks
    if(csv::OpenFile("KatetLinks.csv") == retvOk) {
        while(csv::ReadNextLine() == retvOk) {
            char *Name, *p;
            bool Value;
            if(csv::GetNextToken(&Name) != retvOk) continue;
            uint32_t i = strtoul(Name, &p, 0);
            if(*p == '\0') {    // Conversion to number succeded, get value
                if(csv::GetNextCell<bool>(&Value) == retvOk) {
                    localChar.ka_tet_links.set(i, Value);
//                    Printf("%u = %u\r", i, Value);
                }
            }
        } // while
        csv::CloseFile();
    }

    // Load counters
    if(csv::OpenFile("Counters.csv") == retvOk) {
        while(csv::ReadNextLine() == retvOk) {
            char *Name, *p;
            uint16_t Value;
            if(csv::GetNextToken(&Name) != retvOk) continue;
            uint32_t i = strtoul(Name, &p, 0);
            if(*p == '\0') {    // Conversion to number succeded, get value
                if(csv::GetNextCell<uint16_t>(&Value) == retvOk) {
                    localChar.ka_tet_counters[i] = Value;
                }
            }
        } // while
        csv::CloseFile();
    }

    if(IsLogicInitOk) {
        Vibra.StartOrRestart(vsqBrrBrr);
        DrawBmpFile(0, 0, "Images/Splash.bmp", &CommonFile);
        chThdSleepMilliseconds(720);
        dispatcher.init(&infTable, &emoTable, &charTable, &localChar);
        Radio.Init();
    }
#endif

    // ==== Main cycle ====
    ITask();
}

__noreturn
void ITask() {
    while(true) {
        EvtMsg_t Msg = EvtQMain.Fetch(TIME_INFINITE);
//        Printf("Msg.ID %u\r", Msg.ID);
        switch(Msg.ID) {
            case evtIdShellCmd:
                OnCmd((Shell_t*)Msg.Ptr);
                ((Shell_t*)Msg.Ptr)->SignalCmdProcessed();
                break;

            case evtIdButtons:
//                Printf("Btn: %u %u\r", Msg.BtnEvtInfo.BtnID, Msg.BtnEvtInfo.Type);
                if(Msg.BtnEvtInfo.BtnID == 2 and Msg.BtnEvtInfo.Type == beLongPress) {
                    PowerOff();
                }
#ifdef LOGIC_EN
                if(IsLogicInitOk) dispatcher.handle_button(Msg.BtnEvtInfo.BtnID, (Msg.BtnEvtInfo.Type == beLongPress));
#endif
                break;

            case evtIdEverySecond:
//                Printf("Second\r");
#ifdef LOGIC_EN
                if(IsLogicInitOk) dispatcher.tick();
#endif
                break;

            case evtIdAdcRslt:
//                Printf("Adc: %u; ExtPwr: %u; Charging: %u\r", Msg.Value, Power.ExternalPwrOn(), Power.IsCharging());
                ChargePercent = mV2PercentLiIon(Msg.Value);
                IsCharging = Power.IsCharging();
                // Force redraw if percent changed
                if(PrevChargePercent != ChargePercent and !IsSleeping()) {
                    PrevChargePercent = ChargePercent;
                    DrawBattery();
                }
                // Vibrate if discharged
                if(ChargePercent < 10 and !IsCharging) {
                    if(chVTTimeElapsedSinceX(WhenDischargedBatteryIndicated) >= S2ST(DISCH_BAT_VIBR_PERIOD_S)) {
                        WhenDischargedBatteryIndicated = chVTGetSystemTimeX();
                        Vibra.StartOrRestart(vsqBrrBrr);
                    }
                }
#ifdef LOGIC_EN
//                if(IsLogicInitOk) dispatcher.handle_battery_status(mV2PercentLiIon(Msg.Value), Power.IsCharging(), Power.ExternalPwrOn());
#endif
                break;

            case evtIdNewRPkt:
//                Printf("RPkt: Inf=%u; Par=%u; RSSI=%d\r", Msg.b[0], Msg.b[1], (int8_t)Msg.b[2]);
//                Lcd.Printf(0, 45, clBlue, clWhite, "Rssi %d; Inf: %u  ", (int8_t)Msg.b[2], Msg.b[0]);
#ifdef LOGIC_EN
                if(IsLogicInitOk) dispatcher.handle_radio_packet(Msg.b[0], Msg.b[1], (int8_t)Msg.b[2]);
#endif
                break;

            case evtIdSoundFileEnd:
//                Printf("SoundFile end: %u\r", Msg.Value);
#ifdef LOGIC_EN
                if(IsLogicInitOk) dispatcher.handle_track_end(Msg.Value);
#endif
                break;

#if 1 // ======= Pill ======
            case evtIdPillConnected:
                Printf("Pill: %d\r", ((Pill_t*)Msg.Ptr)->TypeInt32);
                Vibra.StartOrRestart(vsqBrrBrr);
#ifdef LOGIC_EN
                if(IsLogicInitOk) dispatcher.handle_nfc_packet((uint8_t)((Pill_t*)Msg.Ptr)->TypeInt32);
#endif
                break;
            case evtIdPillDisconnected:
                Printf("Pill Discon\r");
                break;
#endif

#if 1 // ======= USB =======
            case evtIdUsbConnect:
                Printf("USB connect\r");
                Clk.SetupFlashLatency(48);
                Clk.SetupBusDividers(ahbDiv1, apbDiv2, apbDiv2); // 48 MHz AHB, 24 MHz APB1, 24 MHz APB2
                Clk.UpdateFreqValues();
                Uart.OnClkChange();
                Clk.PrintFreqs();
                chThdSleepMilliseconds(270);
                UsbMsdCdc.Connect();
                break;
            case evtIdUsbDisconnect:
                UsbMsdCdc.Disconnect();
                Clk.SetupBusDividers(ahbDiv2, apbDiv1, apbDiv1); // 24 MHz AHB, 24 MHz APB1, 24 MHz APB2
                Clk.UpdateFreqValues();
                Clk.SetupFlashLatency(Clk.AHBFreqHz/1000000);
                Uart.OnClkChange();
                Printf("USB disconnect\r");
                Clk.PrintFreqs();
                break;
            case evtIdUsbReady:
                Printf("USB ready\r");
                break;
#endif

            default: break;
        } // switch
    } // while true
}

void ProcessUsbDetect(PinSnsState_t *PState, uint32_t Len) {
    if(*PState == pssRising) EvtQMain.SendNowOrExit(EvtMsg_t(evtIdUsbConnect));
    else if(*PState == pssFalling) EvtQMain.SendNowOrExit(EvtMsg_t(evtIdUsbDisconnect));
}

#if 1 // ======================= Command processing ============================
void OnCmd(Shell_t *PShell) {
    Cmd_t *PCmd = &PShell->Cmd;
//    Printf("%S  ", PCmd->Name);

    // Handle command
    if(PCmd->NameIs("Ping")) PShell->Ack(retvOk);
    else if(PCmd->NameIs("Version")) PShell->Printf("%S %S\r", APP_NAME, BUILD_TIME);

//    else if(PCmd->NameIs("GetBat")) { PShell->Printf("Battery: %u\r", Audio.GetBatteryVmv()); }

    else if(PCmd->NameIs("Start")) {
        uint8_t Slot;
        uint16_t Volume;
        char *S;
        if(PCmd->GetNext<uint8_t>(&Slot) != retvOk)    { PShell->Ack(retvCmdError); return; }
        if(PCmd->GetNext<uint16_t>(&Volume) != retvOk) { PShell->Ack(retvCmdError); return; }
        if(PCmd->GetNextString(&S) != retvOk) { PShell->Ack(retvCmdError); return; }
        SlotPlayer::Start(Slot, Volume, S, false);
    }
    else if(PCmd->NameIs("Vol")) {
        uint8_t Slot;
        uint16_t Volume;
        if(PCmd->GetNext<uint8_t>(&Slot) != retvOk)    { PShell->Ack(retvCmdError); return; }
        if(PCmd->GetNext<uint16_t>(&Volume) != retvOk) { PShell->Ack(retvCmdError); return; }
        SlotPlayer::SetVolume(Slot, Volume);
    }
    else if(PCmd->NameIs("Stop")) {
        uint8_t Slot;
        if(PCmd->GetNext<uint8_t>(&Slot) != retvOk)    { PShell->Ack(retvCmdError); return; }
        SlotPlayer::Stop(Slot);
    }

    else if(PCmd->NameIs("GetC")) {
        for(uint32_t i=0; i<KaTetCounters::SIZE; i++)
            Printf("cnt %u = %u\r", i, localChar.ka_tet_counters[i]);
    }

    else if(PCmd->NameIs("GetID")) PShell->Reply("ID", ID);
    else if(PCmd->NameIs("GetInf")) PShell->Reply("Inf", Influence);

#if PILL_ENABLED // ==== Pills ====
    else if(PCmd->NameIs("PillRead32")) {
        int32_t Cnt = 0, dw32;
        if(PCmd->GetNext<int32_t>(&Cnt) != retvOk) { PShell->Ack(retvCmdError); return; }
        uint8_t MemAddr = 0, b = retvOk;
        PShell->Printf("PillData32 ");
        for(int32_t i=0; i<Cnt; i++) {
            b = PillMgr.Read(MemAddr, &dw32, 4);
            if(b != retvOk) break;
            PShell->Printf("%d ", dw32);
            MemAddr += 4;
        }
        PShell->Printf("\r\n");
        PShell->Ack(b);
    }

    else if(PCmd->NameIs("PillWrite32")) {
        uint8_t b = retvCmdError;
        uint8_t MemAddr = 0;
        int32_t dw32;
        while(true) {
            if(PCmd->GetNext<int32_t>(&dw32) != retvOk) break;
            b = PillMgr.Write(MemAddr, &dw32, 4);
            if(b != retvOk) break;
            MemAddr += 4;
        } // while
        PShell->Ack(b);
    }
#endif

    else PShell->Ack(retvCmdUnknown);
}
#endif

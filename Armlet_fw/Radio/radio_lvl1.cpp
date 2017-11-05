/*
 * radio_lvl1.cpp
 *
 *  Created on: Nov 17, 2013
 *      Author: kreyl
 */

#include "radio_lvl1.h"
#include "cc1101.h"
#include "MsgQ.h"
#include "led.h"
#include "Sequences.h"

cc1101_t CC(CC_Setup0);

#define DBG_PINS

#ifdef DBG_PINS
#define DBG_GPIO1   GPIOD
#define DBG_PIN1    14
#define DBG1_SET()  PinSetHi(DBG_GPIO1, DBG_PIN1)
#define DBG1_CLR()  PinSetLo(DBG_GPIO1, DBG_PIN1)
//#define DBG_GPIO2   GPIOB
//#define DBG_PIN2    9
//#define DBG2_SET()  PinSet(DBG_GPIO2, DBG_PIN2)
//#define DBG2_CLR()  PinClear(DBG_GPIO2, DBG_PIN2)
#else
#define DBG1_SET()
#define DBG1_CLR()
#endif

rLevel1_t Radio;

#if 1 // ================================ Task =================================
static THD_WORKING_AREA(warLvl1Thread, 256);
__noreturn
static void rLvl1Thread(void *arg) {
    chRegSetThreadName("rLvl1");
    Radio.ITask();
}

__noreturn
void rLevel1_t::ITask() {
    while(true) {
        chThdSleepMilliseconds(45);
//        CC.Recalibrate();
        DBG1_SET();
        CC.Transmit(&Pkt, RPKT_LEN); // Length byte + payload
        DBG1_CLR();
                    // Receive
//                    uint8_t RxRslt = CC.Receive(RX_T_MS, &rPktReply, 2, &Rssi);
//                    if(RxRslt == retvOk) {
//                        EvtMsg_t OutMsg(evtIdRadioRx, Rssi);
//                        EvtQMain.SendNowOrExit(OutMsg);
//                    }
//
//
//            Printf("Par %u; Rssi=%d\r", PktRx.CmdID, Rssi);
            // Transmit reply, it formed inside OnRadioRx
//            if(OnRadioRx() == retvOk) CC.Transmit(&PktTx);
//        } // if RxRslt ok
    } // while
}
#endif // task

#if 1 // ============================
uint8_t rLevel1_t::Init() {
#ifdef DBG_PINS
    PinSetupOut(DBG_GPIO1, DBG_PIN1, omPushPull);
//    PinSetupOut(DBG_GPIO2, DBG_PIN2, omPushPull);
#endif    // Init radioIC

    if(CC.Init() == retvOk) {
        CC.SetTxPower(CC_TX_PWR);
        CC.SetPktSize(RPKT_LEN);
        CC.SetChannel(0);
        // Thread
        chThdCreateStatic(warLvl1Thread, sizeof(warLvl1Thread), HIGHPRIO, (tfunc_t)rLvl1Thread, NULL);
        return retvOk;
    }
    else return retvFail;
}

void rLevel1_t::SetChannel(uint8_t NewChannel) {
    CC.SetChannel(NewChannel);
}
#endif

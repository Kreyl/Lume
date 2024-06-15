/*
 * kl_servo.h
 *
 *  Created on: 10 ���. 2016 �.
 *      Author: Kreyl
 */

#ifndef KL_SERVO_H__
#define KL_SERVO_H__

#include "kl_lib.h"

/*
 * Example:
 * #define SERVO1_PIN      GPIOC, 9, TIM3, 4
 * Servo_t Servo1 {SERVO1_PIN, 544, 2380};
 * Servo1.Init();
 *
    else if(PCmd->NameIs("Set")) {
        int32_t v;
        if(PCmd->GetNext(&v) == retvOk) {
            srv.SetAngle_dg(v);
            PShell->Ok();
        }
        else PShell->BadParam();
    }
 */

class Servo_t : private PinOutputPWM_t {
private:
    const uint32_t imin_us, imax_us;
public:
    void Init() const {
        PinOutputPWM_t::Init();
        PinOutputPWM_t::SetFrequencyHz(50);
        Set_us(imin_us);
    }
    void Set_us(int32_t uS) const {
        PinOutputPWM_t::Set(uS);
    }

    void SetAngle_dg(int32_t Angle) const {
        uint32_t us = Proportion<int32_t>(0, 180, imin_us, imax_us, Angle);
        Printf("%d %d\r", Angle, us);
        Set_us(us);
    }

    void Release() const { PinOutputPWM_t::Set(0); }

    Servo_t(GPIO_TypeDef *PGpio, uint16_t Pin, TIM_TypeDef *PTimer, uint32_t TimerChnl,
            uint32_t min_us = 544, uint32_t max_us = 2400) :
                // Setup timer, making 1 tick = 1us
                PinOutputPWM_t(PGpio, Pin, PTimer, TimerChnl, invNotInverted, omPushPull, 19999),
                imin_us(min_us), imax_us(max_us) {}
};

#endif // KL_SERVO_H__

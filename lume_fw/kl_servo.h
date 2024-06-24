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
    const uint32_t ktim_freq, kmin_us, kmax_us;
public:
    void Init(uint32_t start_us = 0) const {
        PinOutputPWM_t::Init();
        PinOutputPWM_t::SetFrequencyHz(ktim_freq);
        Set_us(start_us);
    }
    void Set_us(int32_t uS) const {
        PinOutputPWM_t::Set(uS);
    }

    void SetAngle_dg(int32_t angle) const {
        uint32_t us = Proportion<int32_t>(0, 180, kmin_us, kmax_us, angle);
        Printf("%d %d\r", angle, us);
        Set_us(us);
    }

    void Release() const { PinOutputPWM_t::Set(0); }

    Servo_t(GPIO_TypeDef *pgpio, uint16_t pin, TIM_TypeDef *ptimer, uint32_t timer_chnl,
            const uint32_t timer_freq = 50,
            const uint32_t min_us = 544, const uint32_t max_us = 2400) :
                // Setup timer, making 1 tick = 1us. Timer input freq can always be made 1MHz
                PinOutputPWM_t(pgpio, pin, ptimer, timer_chnl, invNotInverted, omPushPull, ((1000000UL / timer_freq) - 1UL)),
                ktim_freq(timer_freq), kmin_us(min_us), kmax_us(max_us) { }
};

#endif // KL_SERVO_H__

//
// Created by SuperChen on 2026/6/29.
//
// TIM1 微秒级计时替代 DWT
// TIM1 作为 HAL 时基已配置为: 72MHz / 72 = 1MHz, Period = 999 → 1ms 中断
// 读 TIM1->CNT (0~999) + HAL_GetTick()*1000 得到连续微秒时间戳
//

#ifndef CAR_PROJECT1_TIM1_DELAY_H
#define CAR_PROJECT1_TIM1_DELAY_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim1;

/* ───── 获取连续微秒时间戳 (32-bit, 约 71 分钟回绕) ───── */
static inline uint32_t tim1_get_us(void)
{
    return HAL_GetTick() * 1000 + (uint32_t)(TIM1->CNT);
}

/* ───── 阻塞微秒延时 (基于 TIM1 轮询) ───── */
static inline void tim1_delay_us(uint32_t us)
{
    uint32_t start = tim1_get_us();
    while ((tim1_get_us() - start) < us);
}

#endif //CAR_PROJECT1_TIM1_DELAY_H

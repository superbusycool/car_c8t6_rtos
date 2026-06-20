

#include "gpio.h"
#include "module.h"

/* ---- SR04 超声波测距 ---- */
/*
 * TRIG = PA8 (输出), ECHO = PB9 (输入)
 * 时序: TRIG 发 15µs 高脉冲 → 等待 ECHO 高电平 →
 *       测量 ECHO 高电平脉宽 → 距离(cm) = 脉宽(µs) / 58
 * 返回 0 表示超时/无回波
 */
uint16_t SR04_Measure(void) {
    uint32_t start, timeout;

    /* 1. TRIG 发 15µs 高脉冲 */
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_RESET);
    dwt_delay_us(2);
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_SET);
    dwt_delay_us(15);
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_RESET);

    /* 2. 等待 ECHO 上升沿 (带超时) */
    timeout = dwt_get_time_us() + SR04_TIMEOUT_US;
    while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET)
    {
        if (dwt_get_time_us() > timeout) return 0;
    }

    /* 3. 测量 ECHO 高电平脉宽 (带超时) */
    start = dwt_get_time_us();
    timeout = start + SR04_TIMEOUT_US;
    while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET)
    {
        if (dwt_get_time_us() > timeout) return 0;
    }
    uint32_t pulse_us = dwt_get_time_us() - start;

    /* 4. 转换为厘米 (声速 340m/s, 往返双程) */
    uint16_t dist_cm = (uint16_t)(pulse_us / 58);

    /* 超量程钳位 */
    if (dist_cm > SR04_MAX_DIST_CM) dist_cm = 0;

    return dist_cm;
}
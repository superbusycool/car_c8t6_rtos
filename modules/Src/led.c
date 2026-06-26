//
// Created by SuperChen on 2026/6/21.
//
#include "led.h"
#include "main.h"
#include "cmsis_os.h"

/*
 * @brief 电压检测呼吸灯报警
 * */
void Battery_Alarming(void){
    /* ── 呼吸灯 (软件PWM, 两灯同步) ── */
    for (uint8_t i = 0; i <= 40; i++)            /* 渐亮 */
    {
        uint32_t on_us = (uint32_t)i * 125;
        uint32_t off_us = 5000u - on_us;
        for (uint8_t c = 3; c; c--)
        {
            HAL_GPIO_WritePin(LED_HL_GPIO_Port, LED_HL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_HR_GPIO_Port, LED_HR_Pin, GPIO_PIN_SET);
            { uint32_t _s = DWT->CYCCNT, _t = on_us * (SystemCoreClock/1000000u); while ((DWT->CYCCNT - _s) < _t); }
            HAL_GPIO_WritePin(LED_HL_GPIO_Port, LED_HL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_HR_GPIO_Port, LED_HR_Pin, GPIO_PIN_RESET);
            { uint32_t _s = DWT->CYCCNT, _t = off_us * (SystemCoreClock/1000000u); while ((DWT->CYCCNT - _s) < _t); }
        }
        osDelay(10);
    }
    for (uint8_t i = 40; i > 0; i--)              /* 渐暗 */
    {
        uint32_t on_us = (uint32_t)i * 125;
        uint32_t off_us = 5000u - on_us;
        for (uint8_t c = 3; c; c--)
        {
            HAL_GPIO_WritePin(LED_HL_GPIO_Port, LED_HL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(LED_HR_GPIO_Port, LED_HR_Pin, GPIO_PIN_SET);
            { uint32_t _s = DWT->CYCCNT, _t = on_us * (SystemCoreClock/1000000u); while ((DWT->CYCCNT - _s) < _t); }
            HAL_GPIO_WritePin(LED_HL_GPIO_Port, LED_HL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_HR_GPIO_Port, LED_HR_Pin, GPIO_PIN_RESET);
            { uint32_t _s = DWT->CYCCNT, _t = off_us * (SystemCoreClock/1000000u); while ((DWT->CYCCNT - _s) < _t); }
        }
        osDelay(10);
    }
}



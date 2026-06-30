//
// Created by SuperChen on 2026/6/20.
//

#include "dwt.h"

static uint32_t cpu_freq_mhz = 72;
static volatile uint32_t cyccnt_high = 0;   // CYCCNT 溢出次数（高位），32-bit 读写天然原子
static uint32_t cyccnt_last = 0;            // 上次读取的 DWT->CYCCNT

void dwt_init(uint32_t freq_mhz)
{
    cpu_freq_mhz = freq_mhz;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    cyccnt_high = 0;
    cyccnt_last = DWT->CYCCNT;
}

void dwt_systime_update(void)
{
    uint32_t now = DWT->CYCCNT;

    /* CYCCNT 从 0xFFFFFFFF 回绕到 0 时进位 */
    if (now < cyccnt_last)
    {
        cyccnt_high++;          // 单次 32-bit 写入，对 Cortex-M3 是原子的
    }
    cyccnt_last = now;
}

uint64_t dwt_get_time_us(void)
{
    dwt_systime_update();

    /* 高位左移 32 位 + 当前 CYCCNT，构成完整 64-bit 周期计数 */
    uint64_t total = ((uint64_t)cyccnt_high << 32) | DWT->CYCCNT;
    return total / cpu_freq_mhz;
}

float dwt_get_time_ms(void)
{
    return (float)dwt_get_time_us() / 1000.0f;
}

float dwt_get_time_s(void)
{
    return (float)dwt_get_time_us() / 1000000.0f;
}

float dwt_get_delta(uint32_t *cnt_last)
{
    dwt_systime_update();       // 保持高位累加器同步

    uint32_t now = DWT->CYCCNT;
    uint32_t delta;

    if (now >= *cnt_last)
        delta = now - *cnt_last;
    else
        delta = (0xFFFFFFFF - *cnt_last) + now + 1;

    *cnt_last = now;
    return (float)delta / (cpu_freq_mhz * 1000000.0f);
}

double dwt_get_delta_64(uint32_t *cnt_last)
{
    dwt_systime_update();       // 保持高位累加器同步

    uint32_t now = DWT->CYCCNT;
    uint64_t delta;

    if (now >= *cnt_last)
        delta = (uint64_t)now - (uint64_t)*cnt_last;
    else
        delta = (0xFFFFFFFFULL - *cnt_last) + now + 1;

    *cnt_last = now;
    return (double)delta / (double)(cpu_freq_mhz * 1000000);
}

void dwt_delay_s(float delay)
{
    if (delay <= 0) return;

    uint64_t start = dwt_get_time_us();
    uint64_t us = (uint64_t)(delay * 1000000.0f);
    while (dwt_get_time_us() - start < us);
}

// 微秒级延时
void dwt_delay_us(uint32_t us)
{
    uint64_t start = dwt_get_time_us();
    while (dwt_get_time_us() - start < us);
}

// 毫秒级延时
void dwt_delay_ms(uint32_t ms)
{
    dwt_delay_us(ms * 1000);
}

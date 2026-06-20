//
// Created by SuperChen on 2026/6/20.
//

#include "dwt.h"

static uint32_t cpu_freq_mhz = 72;
static uint32_t cyccnt_over_flow = 0;
static uint32_t cyccnt_last = 0;

void dwt_init(uint32_t freq_mhz)
{
    cpu_freq_mhz = freq_mhz;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    cyccnt_over_flow = 0;
    cyccnt_last = DWT->CYCCNT;
}

void dwt_systime_update(void)
{
    uint32_t now = DWT->CYCCNT;

    if (now < cyccnt_last)
    {
        cyccnt_over_flow++;
    }
    cyccnt_last = now;
}

uint64_t dwt_get_time_us(void)
{
    dwt_systime_update();

    uint64_t total = (uint64_t)cyccnt_over_flow * 0x100000000ULL + DWT->CYCCNT;
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

    uint64_t cycles = (uint64_t)(delay * cpu_freq_mhz * 1000000);
    uint32_t start = DWT->CYCCNT;
    uint64_t diff = 0;

    do
    {
        uint32_t now = DWT->CYCCNT;

        if (now >= start)
            diff = now - start;
        else
            diff = (0xFFFFFFFFULL - start) + now + 1;

    } while (diff < cycles);
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
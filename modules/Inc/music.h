//
// Created by SuperChen on 2026/6/20.
//

#ifndef CAR_PROJECT1_MUSIC_H
#define CAR_PROJECT1_MUSIC_H

#include "tim.h"
#include "stdint.h"
#include <stdbool.h>

typedef struct
{
    uint8_t onoff; /* on or off*/
    uint8_t freq;  /* frequence */
    uint16_t tick; /* time */
}midiType;

extern const midiType Spirited_Away[];
extern const uint16_t SPIRITED_AWAY_LEN;

/* 阻塞播放（旧，不推荐用于多任务环境） */
void Music_Play(const midiType *mid, uint16_t len, uint32_t tick_ms);

/* ──────────────── 非阻塞播放 API ──────────────── */
void Music_Play_Start(const midiType *mid, uint16_t len, uint32_t tick_ms);
bool Music_Play_Update(void);
void Music_Play_Stop(void);
bool Music_Play_IsPlaying(void);

#endif //CAR_PROJECT1_MUSIC_H

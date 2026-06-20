//
// Created by SuperChen on 2026/6/20.
//

#ifndef CAR_PROJECT1_MUSIC_H
#define CAR_PROJECT1_MUSIC_H

#include "tim.h"
#include "stdint.h"

typedef struct
{
    uint8_t onoff; /* on or off*/
    uint8_t freq;  /* frequence */
    uint16_t tick; /* time */
}midiType;

extern const midiType Spirited_Away[];

void play(midiType* mid);

#endif //CAR_PROJECT1_MUSIC_H

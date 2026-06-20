//
// Created by SuperChen on 2026/6/20.
//

#ifndef CAR_PROJECT1_KEY_H
#define CAR_PROJECT1_KEY_H
#include "main.h"

// 按键状态定义
typedef enum {
    KEY_RELEASED = 0,    // 按键释放状态
    KEY_PRESSED,         // 按键按下状态
    KEY_HOLD,            // 按键长按状态
    KEY_JUST_RELEASED    // 按键刚刚释放
} Key_State;

// 按键结构体
typedef struct {
    GPIO_TypeDef* GPIO_Port;    //GPIO端口
    uint16_t GPIO_Pin;         //GPIO引脚号
    Key_State state;            //按键状态
    uint32_t press_start_time;   //按键按下时间
    uint8_t debounce_counter;   //消抖计数器
} Key_Info;

// 全局按键对象
extern Key_Info Key1;

// 函数声明
void Key_Init(void);
void Key_Update(void);
int8_t Key_GetNum(void);
#endif //CAR_PROJECT1_KEY_H

//
// Created by SuperChen on 2026/6/20.
//

#include "key.h"
#include "cmsis_os.h"

// 按键对象初始化
Key_Info Key1 = {
        .GPIO_Port = KEY_GPIO_Port,
        .GPIO_Pin = KEY_Pin,
        .state = KEY_RELEASED,
        .press_start_time = 0,
        .debounce_counter = 0
};


// 按键初始化
void Key_Init(void)
{
    // 初始化状态
    Key1.state = KEY_RELEASED;

}

// 按键状态更新（在TIM2中断中调用）
void Key_Update(void)
{
    // 更新Key1状态
    if (HAL_GPIO_ReadPin(Key1.GPIO_Port, Key1.GPIO_Pin) == GPIO_PIN_RESET) {
        // 按键按下
//        osDelay(10);
        if(HAL_GPIO_ReadPin(Key1.GPIO_Port, Key1.GPIO_Pin) == GPIO_PIN_RESET){
            if (Key1.debounce_counter < 5) {
                Key1.debounce_counter++;
            }

            if (Key1.debounce_counter == 5) {
                if (Key1.state == KEY_RELEASED) {
                    Key1.state = KEY_PRESSED;
                    Key1.press_start_time = HAL_GetTick();
                } else if (HAL_GetTick() - Key1.press_start_time > 500) {
                    Key1.state = KEY_HOLD;
                }
            }

        }

    } else {
        // 按键释放
        if (Key1.debounce_counter > 0) {
            Key1.debounce_counter--;
        }

        if (Key1.debounce_counter == 0) {
            if (Key1.state == KEY_PRESSED || Key1.state == KEY_HOLD) {
                Key1.state = KEY_JUST_RELEASED;
            } else {
                Key1.state = KEY_RELEASED;
            }
        }
    }

}

// 获取按键事件
int8_t Key_GetNum(void)
{
    int8_t keyNum = -1;

    // 检查Key1
    if (Key1.state == KEY_PRESSED) {
        keyNum = 0;
        Key1.state = KEY_HOLD; // 防止重复检测
    }
        // 检查Key1释放事件（可选）
    else if (Key1.state == KEY_JUST_RELEASED) {
        // 可添加释放事件处理
        Key1.state = KEY_RELEASED;
    }


    return keyNum;
}
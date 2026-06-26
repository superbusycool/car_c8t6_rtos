

#ifndef CAR_PROJECT1_RZ7899_H
#define CAR_PROJECT1_RZ7899_H

#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"
void Motor_SetLQSpeed(int speed);

void Motor_SetLHSpeed(int speed);

void Motor_SetRQSpeed(int speed);

//右后
void Motor_SetRHSpeed(int speed);

void Motor_SetLeftSpeed(int Motor1Speed);

void Motor_SetRightSpeed(int Motor2Speed);
/*关于转向*/
void Car_direction_change(float basic_vel,float vel_delta);
/*
 * @brief 前进&后退
 * @param: +:前进; -:后退
 * */
void Car_move(int16_t basic_vel);

#endif //CAR_PROJECT1_RZ7899_H



#include "motor.h"
#include "tim.h"
#include "led.h"
//左前,方向正确
void Motor_SetLQSpeed(int speed)
{
    int speed_real = -speed;
    if (speed_real < 0)
    {
        if (speed_real < -99)
        {
            speed_real = -99;
        }
        int pwm = -speed_real;

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0); //LQ_F
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pwm);//LQ_B
    }
    else if (speed_real == 0)
    {

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
    }
    else
    {
        if(speed_real > 99)
        {
            speed_real = 99;
        }
        int pwm = speed_real;

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pwm);//LQ_F
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);//LQ_B

    }
}
//左后
void Motor_SetLHSpeed(int speed)
{
    int speed_real = -speed;
    if (speed_real < 0)
    {
        if (speed_real < -99)
        {
            speed_real = -99;
        }
        int pwm = -speed_real;

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); //LH_F
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, pwm);//LH_B
    }
    else if (speed_real == 0)
    {

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
    }
    else
    {
        if(speed_real > 99)
        {
            speed_real = 99;
        }
        int pwm = speed_real;

        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm);//LH_F
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);//LH_B

    }
}
//右前 方向正确
void Motor_SetRQSpeed(int speed)
{
    int speed_real = -speed;

    if (speed_real < 0)
    {
        if (speed_real < -99)
        {
            speed_real = -99;
        }
        int pwm = -speed_real;

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm);//RQ_F
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);//RQ_B
    }
    else if (speed_real == 0)
    {

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
    }
    else
    {
        if(speed_real > 99)
        {
            speed_real = 99;
        }
        int pwm = speed_real;

        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);//RQ_F
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwm);//RQ_B

    }
}

//右后 方向正确
void Motor_SetRHSpeed(int speed)
{
    int speed_real = -speed;

    if (speed_real < 0)
    {
        if (speed_real < -99)
        {
            speed_real = -99;
        }
        int pwm = -speed_real;

        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, pwm); //RH_F
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);//RH_B
    }
    else if (speed_real == 0)
    {

        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
    }
    else
    {
        if(speed_real > 99)
        {
            speed_real = 99;
        }
        int pwm = speed_real;

        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);//RH_F
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pwm);//RH_B

    }
}
void Motor_SetLeftSpeed(int Motor1Speed)
{
    Motor_SetLQSpeed(Motor1Speed);
    Motor_SetLHSpeed(Motor1Speed);

}

void Motor_SetRightSpeed(int Motor2Speed)
{
    Motor_SetRQSpeed(Motor2Speed);
    Motor_SetRHSpeed(Motor2Speed);

}

void Car_direction_change(float basic_vel,int left_speed, int right_speed,int vel_delta){/*vel_delta由pid计算得出*/
    if(vel_delta > 5){
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_L_GPIO_Port, LED_L_Pin, GPIO_PIN_RESET);

    }
    else if(vel_delta < -5){
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_L_GPIO_Port, LED_L_Pin, GPIO_PIN_SET);
    }
    else{
        HAL_GPIO_WritePin(LED_L_GPIO_Port, LED_L_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
    }

    Motor_SetLQSpeed(left_speed);
    Motor_SetLHSpeed(left_speed);
    Motor_SetRQSpeed(right_speed);
    Motor_SetRHSpeed(right_speed);
}
void Car_move(int16_t basic_vel){/*vel_delta由pid计算得出*/
    Motor_SetLQSpeed(basic_vel);
    Motor_SetLHSpeed(basic_vel);
    Motor_SetRQSpeed(basic_vel);
    Motor_SetRHSpeed(basic_vel);
}


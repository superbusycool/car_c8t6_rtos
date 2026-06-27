/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "module.h"
#include "tim.h"
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId cmd_taskHandle;
osThreadId chassis_taskHandle;
osThreadId sensorHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_cmd(void const * argument);
void Start_chassis(void const * argument);
void Start_sensor(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of cmd_task */
  osThreadDef(cmd_task, Start_cmd, osPriorityNormal, 0, 128);
  cmd_taskHandle = osThreadCreate(osThread(cmd_task), NULL);

  /* definition and creation of chassis_task */
  osThreadDef(chassis_task, Start_chassis, osPriorityNormal, 0, 128);
  chassis_taskHandle = osThreadCreate(osThread(chassis_task), NULL);

  /* definition and creation of sensor */
  osThreadDef(sensor, Start_sensor, osPriorityNormal, 0, 128);
  sensorHandle = osThreadCreate(osThread(sensor), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_cmd */
/***********value define*************************/
static uint16_t ADC_Value[5];/*循迹模块adc采样*/
static uint16_t distance;/*测距距离*/
static float Basic_vel;

static  pid_obj_t *CarTurn_pid;  // 转向PID控制器
static pid_config_t CarTurn_pid_config = INIT_PID_CONFIG(CarTurn_KP_V, CarTurn_KI_V, CarTurn_KD_V,CarTurn_INTEGRAL_V, CarTurn_MAX_V,
                                                         (PID_Trapezoid_Intergral | PID_Integral_Limit | PID_Derivative_On_Measurement));
static uint8_t detect_current;/*每路是否识别到黑线*/
bool detect_detect_lost;
static float detect_value_current;/*由寻迹模块得到的权重*/
static float detect_value_current_last;
static uint8_t track_status_worse;
static float target_value;/*设置的目标值*/

void Detection_val_calc(uint16_t* adc_value);/*计算寻迹权重*/

/*权重从左到右 : -4 -1 0 1 4*/
#define ADC_HRESHOLD_VALUE_MID 1600//1620  //判断是否寻到线的ADC阈值
#define ADC_HRESHOLD_VALUE_OUT2 1600//1620  //判断是否寻到线的ADC阈值
#define ADC_HRESHOLD_VALUE_OUT1 1600//1620  //判断是否寻到线的ADC阈值

#define ADC_OUT2_VALUE  6.0f   //对称最外侧的两个光电管寻到黑线
#define ADC_OUT1_VALUE  4.0f   //对称次外侧的两个光电管寻到黑线
#define ADC_MID_VALUE  0.0f     //中间寻到黑
uint16_t ADC_NORMAL_VALUE = ADC_OUT2_VALUE+ADC_OUT1_VALUE;

bool move_start_flag;/*开始循迹标志位*/
static int move_stop_flag;/*开始循迹标志位*/
/************************************/

/**
  * @brief  Function implementing the cmd_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_cmd */
__weak void Start_cmd(void const * argument)
{
  /* USER CODE BEGIN Start_cmd */

  /* Infinite loop */
  for(;;)
  {

      /* 开机播放一段旋律 (软件PWM, 阻塞播出后再进入主循环) */
      if(move_start_flag == 1){
//          Music_Play(Spirited_Away, SPIRITED_AWAY_LEN, 1);
      }
//      Battery_Alarming();
//      LED_R();
//      LED_L();

    osDelay(1);
  }
  /* USER CODE END Start_cmd */
}

/* USER CODE BEGIN Header_Start_chassis */
/**
* @brief Function implementing the chassis_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_chassis */
__weak void Start_chassis(void const * argument)
{
  /* USER CODE BEGIN Start_chassis */
    CarTurn_pid = pid_register(&CarTurn_pid_config);
    target_value = 0.0f;
    Basic_vel = 55;
    Key_Init();
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 60);//开启循迹模块
  /* Infinite loop */
  for(;;)
  {
      if(Key1.state == KEY_PRESSED && move_start_flag == 0){/*开始循迹*/
          move_start_flag = 1;

      }
      if(move_start_flag == 1 ){
          pid_calculate(CarTurn_pid,detect_value_current,target_value);
          Car_direction_change(Basic_vel,CarTurn_pid->Output);
      }

      if(move_start_flag == 1 && detect_detect_lost == 1){
//          Motor_SetLQSpeed(-35);
//          Motor_SetLHSpeed(-35);
//          Motor_SetRQSpeed(-35);
//          Motor_SetRQSpeed(-35);
          Car_move(-45);
      }
      if(move_stop_flag > 2){
          Car_move(0);
      }


    osDelay(1);
  }
  /* USER CODE END Start_chassis */
}

/* USER CODE BEGIN Header_Start_sensor */

/**
* @brief Function implementing the sensor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_sensor */
__weak void Start_sensor(void const * argument)
{
  /* USER CODE BEGIN Start_sensor */
    Key_Init();
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);//开启循迹模块
  /* Infinite loop */
  for(;;)
  {
      /* ---- SR04 超声波测距 ---- */
      distance = SR04_Measure();
      /*************寻迹ADC采样****************/
      HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,5);
      Detection_val_calc(ADC_Value);
      Key_Update();
      OLED_ShowSignedNum(1,1,Key1.debounce_counter,1);
      OLED_ShowSignedNum(2,1,distance,2);
      OLED_ShowSignedNum(3,1,ADC_Value[2],4);
      OLED_ShowSignedNum(3,6,ADC_Value[4],4);
      OLED_ShowSignedNum(3,11,ADC_Value[3],4);
      OLED_ShowSignedNum(4,1,ADC_Value[1],4);
      OLED_ShowSignedNum(4,6,ADC_Value[0],4);
      OLED_ShowSignedNum(4,11,move_stop_flag,1);

    osDelay(1);
  }
  /* USER CODE END Start_sensor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Detection_val_calc(uint16_t * adc_value){
    detect_current = 0;
// 中间 bit2
    if(adc_value[3] > ADC_HRESHOLD_VALUE_MID)
        detect_current |= (1U << 2);
// 次左 bit1
    if(adc_value[4] > ADC_HRESHOLD_VALUE_OUT1)
        detect_current |= (1U << 1);
// 最左 bit0
    if(adc_value[2] > ADC_HRESHOLD_VALUE_OUT2)
        detect_current |= (1U << 0);
// 次右 bit3
    if(adc_value[1] > ADC_HRESHOLD_VALUE_OUT1)
        detect_current |= (1U << 3);
// 最右 bit4
    if(adc_value[0] > ADC_HRESHOLD_VALUE_OUT2)
        detect_current |= (1U << 4);

//    if(detect_current == 0x00){
//        detect_detect_lost = 1;
//    }else{
//        detect_detect_lost = 0;
//    }
    detect_detect_lost = 0;
    // 根据传感器组合状态计算循迹偏差值
    switch (detect_current)
    {
        case 0x01:  // 00001：仅XJ1检测到黑线（最左）
            detect_value_current = -30;
            track_status_worse /= 2;  // 状态正常，减少异常计数
            break;
        case 0x03:  // 00011：XJ1、XJ2检测到黑线
            detect_value_current = -3;
            track_status_worse /= 2;
            break;
        case 0x02:  // 00010：仅XJ2检测到黑线
            detect_value_current = -2;
            track_status_worse /= 2;
            break;
        case 0x07:  // 00111：XJ1、XJ2、XJ3检测到黑线（左到中）
            detect_value_current = -5;  // 宽范围左偏，取中间偏左值
            track_status_worse /= 2;
            break;
        case 0x0F:  // 01111：XJ1、XJ2、XJ3、XJ4检测到黑线（左到中右）
            detect_value_current = -7;  // 宽范围左偏，介于123和12之间
            track_status_worse /= 2;
            break;
        case 0x06:  // 00110：XJ2、XJ3检测到黑线
            detect_value_current = -1;
            track_status_worse /= 2;
            break;
        case 0x04:  // 00100：仅XJ3检测到黑线（中间）
            detect_value_current = 0;
            track_status_worse /= 2;
            break;
        case 0x0C:  // 01100：XJ3、XJ4检测到黑线
            detect_value_current = 1;
            track_status_worse /= 2;
            break;
        case 0x08:  // 01000：仅XJ4检测到黑线
            detect_value_current = 2;
            track_status_worse /= 2;
            break;
        case 0x1E:  // 11110：XJ2、XJ3、XJ4、XJ5检测到黑线（中左到右）
            detect_value_current = 7;  // 宽范围右偏，介于345和45之间
            track_status_worse /= 2;
            break;
        case 0x1C:  // 11100：XJ3、XJ4、XJ5检测到黑线（中到右）
            detect_value_current = 5;  // 宽范围右偏，取中间偏右值
            track_status_worse /= 2;
            break;
        case 0x18:  // 11000：XJ4、XJ5检测到黑线
            detect_value_current = 3;
            track_status_worse /= 2;
            break;
        case 0x10:  // 10000：仅XJ5检测到黑线（最右）
            detect_value_current = 10;
            track_status_worse /= 2;
            break;
        case 0x1F:  // 11111：XJ1-XJ5全检测到黑线（全范围）
            detect_value_current = 30;  // 视为居中（如宽黑线）
            track_status_worse /= 2;
            break;
        case 0x00:  // 00000：无传感器检测到黑线（异常）
            detect_value_current = detect_value_current_last;  // 沿用历史值
            track_status_worse++;  // 增加异常计数
            detect_detect_lost = 1;
            break;
        default:    // 其他未定义状态（异常）
            detect_value_current = detect_value_current_last;  // 沿用历史值
            track_status_worse++;  // 增加异常计数
            break;
    }
    if(detect_current == 0x1F){
        move_stop_flag++;
    }else{
//        move_stop_flag = 0;
    }

//    detect_value_current = (detect_current[0]+detect_current[1]+detect_current[2]+detect_current[3]+detect_current[4]);
}
/* USER CODE END Application */


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
//  osThreadDef(cmd_task, Start_cmd, osPriorityNormal, 0, 128);
//  cmd_taskHandle = osThreadCreate(osThread(cmd_task), NULL);

  /* definition and creation of chassis_task */
  osThreadDef(chassis_task, Start_chassis, osPriorityAboveNormal, 0, 128);
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

// 全局PID句柄（用于循迹转向控制）
PID_HandleTypeDef track_pid = {0};
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
bool car_stop_flag;

volatile int left_speed = 0;
volatile int right_speed = 0;
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

//      /* 开机播放一段旋律 (软件PWM, 阻塞播出后再进入主循环) */
//      if(move_start_flag == 1){
//          Music_Play(Spirited_Away, SPIRITED_AWAY_LEN, 1);
//      }
//      Battery_Alarming();

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
    pid_init(&track_pid);
    target_value = 0.0f;
    Basic_vel = 50.0f;
    Key_Init();
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 60);//开启循迹模块
  /* Infinite loop */
  for(;;)
  {
      /* ---- SR04 超声波测距 ---- */
      distance = SR04_Measure();
      /*************寻迹ADC采样****************/
      HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,5);
      Detection_val_calc(ADC_Value);

      if(Key1.state == KEY_PRESSED && move_start_flag == 0){/*开始循迹*/
          move_start_flag = 1;

      }
      if(move_start_flag == 1 ){

          pid_calculate(&track_pid, detect_value_current);
          left_speed = (int)(Basic_vel + track_pid.output);
          right_speed = (int)(Basic_vel - track_pid.output);
          Car_direction_change(Basic_vel,left_speed,right_speed,(int)track_pid.output);
      }

      if(move_start_flag == 1 && detect_detect_lost == 1){
          Car_move(-45);
      }
      if(move_stop_flag > 2 || car_stop_flag == 1){
          Car_move(0);
          car_stop_flag = 1;
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
    static bool music_triggered = false;     /* 确保 Music_Play_Start 只调一次 */
    Key_Init();
  /* Infinite loop */
  for(;;)
  {
      Key_Update();

      /* 首次 move_start_flag == 1 时启动非阻塞音乐 */
      if(move_start_flag == 1 && !music_triggered){
          music_triggered = true;
          Music_Play_Start(Spirited_Away, SPIRITED_AWAY_LEN, 1);
      }

      /* 非阻塞音乐更新（每 loop 调一次，不忙等，不阻塞其他任务） */
      Music_Play_Update();

    osDelay(1);
  }
  /* USER CODE END Start_sensor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Detection_val_calc(uint16_t *adc_value)
{
    /* === 1. 比较结果直接是 0/1，移位拼成 detect_current (无分支) ===
     * 原来 5 个 if + OR，每个都有分支跳转，改为一条表达式完成
     */
    detect_current = ((adc_value[2] > ADC_HRESHOLD_VALUE_OUT2) << 0)
                   | ((adc_value[4] > ADC_HRESHOLD_VALUE_OUT1) << 1)
                   | ((adc_value[3] > ADC_HRESHOLD_VALUE_MID)  << 2)
                   | ((adc_value[1] > ADC_HRESHOLD_VALUE_OUT1) << 3)
                   | ((adc_value[0] > ADC_HRESHOLD_VALUE_OUT2) << 4);

    detect_detect_lost = (detect_current == 0x00);
    if(detect_current == 0x1F){
        move_stop_flag++;
    }else{
        move_stop_flag = 0;
    }
    /* === 2. 偏差值查表 (O(1) 直接索引，替代 16 路 switch) ===
     * val_table[detect_current] 即得偏差值
     * 0 表示该传感器组合无效（查 result_table 对应标志位）
     */
    static const int8_t val_table[32] = {
    /* 0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07 */
         0,   -4,   -2,   -3,    0,    0,   -1,   -5,
    /* 0x08  0x09  0x0A  0x0B  0x0C  0x0D  0x0E  0x0F */
         2,    0,    0,    0,    1,    0,    0,   -6,
    /* 0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17 */
         4,    0,    0,    0,    0,    0,    0,    0,
    /* 0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F */
         3,    0,    0,    0,    5,    0,    6,   0,
    };
    /* 有效状态位图: bit i = 1 表示状态 i 有效 (0x04 值为 0 但有效) */
    static const uint32_t valid_map = 0xD10191FE;

    int8_t val = val_table[detect_current];
    if ((valid_map >> detect_current) & 1) {
        detect_value_current = val;
    } else {
        detect_value_current = detect_value_current_last;

    }
}
/* USER CODE END Application */


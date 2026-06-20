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
/**
  * @brief  Function implementing the cmd_task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_cmd */
__weak void Start_cmd(void const * argument)
{
  /* USER CODE BEGIN Start_cmd */
    OLED_ShowString(1,1,"question:");
    OLED_ShowString(3,1,"pwm:");

  /* Infinite loop */
  for(;;)
  {

//      Motor_SetLQSpeed(-90);
//      Motor_SetRQSpeed(-90);
//      Motor_SetRHSpeed(-90);
//      Motor_SetLHSpeed(-90);
//      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 90); //LH_F
//      __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);//LH_B


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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_chassis */
}

/* USER CODE BEGIN Header_Start_sensor */
static uint16_t ADC_Value[5];
/**
* @brief Function implementing the sensor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_sensor */
__weak void Start_sensor(void const * argument)
{
  /* USER CODE BEGIN Start_sensor */
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,5);
    Key_Init();
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 999);//开启循迹模块
  /* Infinite loop */
  for(;;)
  {
      Key_Update();
      OLED_ShowSignedNum(2,5,Key1.debounce_counter,1);


    osDelay(1);
  }
  /* USER CODE END Start_sensor */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


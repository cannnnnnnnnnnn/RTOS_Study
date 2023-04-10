/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stdio.h"
#include "queue.h"
#include "semphr.h"
#include "stdlib.h"
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
// 创建任务句柄
TaskHandle_t taskHandle1;
TaskHandle_t taskHandle2;
TaskHandle_t taskHandle3;
// 信号量
QueueHandle_t mutex_semphore_handle;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void low_task(void *pvParameters);
void middle_task(void *pvParameters);
void high_task(void *pvParameters);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
    mutex_semphore_handle = xSemaphoreCreateMutex();    /* 创建互斥信号量，并且主动释放一次信号量 */
    if(mutex_semphore_handle != NULL)
    {
        printf("互斥信号量创建成功！！！\r\n");
    }
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    xTaskCreate(low_task,"low_task",200,NULL,osPriorityLow,&taskHandle1);
    xTaskCreate(middle_task,"middle_task",200,NULL,osPriorityLow1,&taskHandle2);
    xTaskCreate(high_task,"high_task",200,NULL,osPriorityLow2,&taskHandle3);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
      HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void low_task(void *pvParameters) {
    while (1){
        printf("low_task获取信号量\n");
        xSemaphoreTake(mutex_semphore_handle,portMAX_DELAY);
        printf("low_task正在运行\n");
        HAL_Delay(3000);
        printf("low_task释放信号量\n");
        xSemaphoreGive(mutex_semphore_handle);
        vTaskDelay(1000);
    }
}

void middle_task(void *pvParameters) {
    while (1){
        printf("middle_task正在运行\n");
        vTaskDelay(1000);
    }
}

void high_task(void *pvParameters) {
    while (1){
        printf("high_task获取信号量\n");
        xSemaphoreTake(mutex_semphore_handle,portMAX_DELAY);
        printf("high_task正在运行\n");
        HAL_Delay(1000);
        printf("high_task释放信号量\n");
        xSemaphoreGive(mutex_semphore_handle);
        vTaskDelay(1000);

    }
}
/* USER CODE END Application */


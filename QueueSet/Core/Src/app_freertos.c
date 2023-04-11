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
// 队列和队列集句柄
QueueSetHandle_t queueset_handle;
QueueHandle_t queue_handle;
QueueHandle_t semphr_handle;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Task1(void *pvParameters);

void Task2(void *pvParameters);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

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
    // 创建队列集
    queueset_handle = xQueueCreateSet(2);
    if (queueset_handle != NULL){
        printf("队列集创建成功！\n");
    }
    // 创建队列
    queue_handle = xQueueCreate( 1, sizeof(uint8_t));
    // 创建二值信号量
    semphr_handle = xSemaphoreCreateBinary();
    // 将队列和信号量添加到队列集中
    BaseType_t res = 0;
    res = xQueueAddToSet(queue_handle,queueset_handle);
    if (res == pdPASS){
        printf("添加成功！\n");
    }
    res = xQueueAddToSet(semphr_handle,queueset_handle);
    if (res == pdPASS){
        printf("添加成功！\n");
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
    xTaskCreate(Task1,"task1",200,NULL,osPriorityLow,&taskHandle1);
    xTaskCreate(Task2,"task2",200,NULL,osPriorityLow,&taskHandle2);
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
//    char *str_buffer;
    for (;;) {
//        str_buffer = malloc(500);
//        vTaskList(str_buffer);
//        printf("任务名\t\t\t状态\t优先级\t剩余栈\t任务序号\r\n");
//        printf("%s\r\n", str_buffer);
//        free(str_buffer);
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        osDelay(500);
    }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Task1(void *pvParameters) {
    // 任务一:实现队列发送以及信号量释放
    uint8_t key = 1;
    BaseType_t err = 0;
    while (1){
//        err = xQueueSend(queue_handle,&key,portMAX_DELAY);
//        if (err == pdPASS){
//            printf("队列写入数据成功！\n");
//        }
        vTaskDelay(1000);
        // 释放信号量
        err = xSemaphoreGive(semphr_handle);
        if (err == pdPASS){
            printf("信号量释放成功！\n");
        }
        vTaskDelay(1000);
    }
}

void Task2(void *pvParameters) {
    // 任务二:获取队列集消息
    QueueSetMemberHandle_t member_handle;
    uint8_t key;
    while (1){
        member_handle = xQueueSelectFromSet(queueset_handle,portMAX_DELAY);
        if (member_handle == NULL){
            printf("获取失败\n");
        }
        if (member_handle == queue_handle){
            xQueueReceive(member_handle,&key,portMAX_DELAY);
            printf("获取到的队列数据:%d\n",key);
        } else if (member_handle == semphr_handle){
            xSemaphoreTake(member_handle,portMAX_DELAY);
            printf("获取信号量成功\n");
        }
    }
}
/* USER CODE END Application */


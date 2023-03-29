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
// 定义任务句柄
TaskHandle_t taskHandle1;
TaskHandle_t taskHandle2;
TaskHandle_t taskHandle3;

// 任务二堆栈和控制块定义
StaticTask_t task2_TCB;         // 任务控制块
StackType_t task2_stack[512];   // 任务堆栈大小

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 128
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
// 测试的任务线程
void Task1(void *argument);

void Task2(void *argument);

void Task3(void *argument);

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
    // 创建任务参数
    xTaskCreate(Task1, "task1", 200, NULL, osPriorityLow, &taskHandle1);
    // 静态任务创建返回的句柄用于销毁任务
    taskHandle2 = xTaskCreateStatic(Task2, "task2", 200, NULL, osPriorityLow, task2_stack, &task2_TCB);
    xTaskCreate(Task3, "task3", 200, NULL, osPriorityLow, &taskHandle3);
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
void StartDefaultTask(void *argument) {
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;) {
        // 默认系统呼吸灯
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        vTaskDelay(500);
//        printf("LED\n");
    }
    /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// 动态创建task
void Task1(void *argument) {
    uint8_t i = 1;
    while (1){
        portENTER_CRITICAL();
        printf("Task1 run:%d\n",i);
        portEXIT_CRITICAL();
        vTaskDelay(500);
        i++;
    }
}

// 静态创建task(运行五次后删除task1)
void Task2(void *argument) {
    uint32_t j = 1;
    while (1){
        portENTER_CRITICAL();
        printf("Task2 run:%d\n",j);
        portEXIT_CRITICAL();
        vTaskDelay(500);
        if (++j == 6){
            vTaskDelete(taskHandle1);
        }
    }
}

// 运行三次删除task
void Task3(void *argument) {
    uint8_t k = 1;
    while (1) {
        portENTER_CRITICAL();
        printf("Task3 run:%d\n", k);
        portEXIT_CRITICAL();
        vTaskDelay(500);
        if (++k == 4) {
            // 直接删除自身任务线程
            vTaskDelete(NULL);
        }
    }
}

/* USER CODE END Application */


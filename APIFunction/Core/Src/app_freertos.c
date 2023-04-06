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
#include "stdlib.h"
#include "string.h"
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
TaskHandle_t taskHandle1;
TaskHandle_t taskHandle2;
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
    xTaskCreate(Task1, "task1", 200, NULL, osPriorityLow, &taskHandle1);
    xTaskCreate(Task2, "task2", 1024, NULL, osPriorityLow, &taskHandle2);
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
    for (;;) {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        osDelay(500);
    }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Task1(void *pvParameters) {
    uint32_t j = 0;
    while (1) {
        // 涓寸晫鍖鸿缃?
        portENTER_CRITICAL();
        printf("Task run:%d\n", ++j);
        portEXIT_CRITICAL();
        vTaskDelay(500);
    }
}

void Task2(void *pvParameters) {
    uint32_t i = 0;
    UBaseType_t task_num = 0;
    TaskStatus_t *status_array = NULL;
    TaskHandle_t task_handle = NULL;
    TaskStatus_t *task_info = NULL;
    eTaskState task_state = eInvalid;
    char *task_state_str = NULL;
    char *task_info_buf = NULL;

    // 使用uxTaskGetSystemState()函数
    printf("使用uxTaskGetSystemState()函数\n");
    task_num = uxTaskGetNumberOfTasks();            // 获取系统任务数量
    status_array = malloc(task_num * sizeof(TaskStatus_t));     // 申请内存
    task_num = uxTaskGetSystemState(status_array,task_num,NULL);
    printf("任务名\t\t优先级\t\t任务编号\r\n");
    for (int j = 0; j < task_num; ++j) {
        printf("%s\t%s%ld\t\t%ld\r\n",
               status_array[j].pcTaskName,
               strlen(status_array[j].pcTaskName) > 7 ? "": "\t",
               status_array[j].uxCurrentPriority,
               status_array[j].xTaskNumber);
    }
    free(status_array);     // 释放内存

    // 函数vTaskGetInfo()的使用
    printf("使用vTaskGetInfo()函数\n");
    task_info = malloc(sizeof(TaskStatus_t));
    // 在cubemx中还要开启相对应的函数启用，仅仅将相对应的标志位置1还不够
    task_handle = xTaskGetHandle("task1");
    vTaskGetInfo(task_handle,task_info,pdTRUE,eInvalid);
    printf("任务名:\t\t\t%s\r\n", task_info->pcTaskName);
    printf("任务编号:\t\t%ld\r\n", task_info->xTaskNumber);
    printf("任务壮态:\t\t%d\r\n", task_info->eCurrentState);
    printf("任务当前优先级:\t\t%ld\r\n", task_info->uxCurrentPriority);
    printf("任务基优先级:\t\t%ld\r\n", task_info->uxBasePriority);
    printf("任务堆栈基地址:\t\t0x%p\r\n", task_info->pxStackBase);
    printf("任务堆栈历史剩余最小值:\t%d\r\n", task_info->usStackHighWaterMark);
    free(task_info);

    // 函数eTaskGetState()的使用
    printf("函数eTaskGetState()的使用\n");
    task_state_str = malloc(10);
    task_handle = xTaskGetHandle("task1");
    task_state = eTaskGetState(task_handle);
    sprintf(task_state_str, task_state == eRunning ? "Runing" :
                            task_state == eReady ? "Ready" :
                            task_state == eBlocked ? "Blocked" :
                            task_state == eSuspended ? "Suspended" :
                            task_state == eDeleted ? "Deleted" :
                            task_state == eInvalid ? "Invalid" :
                            "");
    printf("任务状态值: %d，对应状态为: %s\r\n", task_state, task_state_str);
    free(task_state_str);

    // 函数vTaskList()的使用
    printf("函数vTaskList()的使用\n");
    task_info_buf = malloc(500);
    vTaskList(task_info_buf);
    printf("任务名\t\t状态\t优先级\t剩余栈\t任务序号\r\n");
    printf("%s\r\n", task_info_buf);
    free(task_info_buf);
    printf("/************************实验结束***************************/");


    while (1) {
        vTaskDelay(1000);
    }
}
/* USER CODE END Application */


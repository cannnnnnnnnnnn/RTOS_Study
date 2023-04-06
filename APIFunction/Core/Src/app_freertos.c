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
        // 临界区设�?
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

    // ʹ��uxTaskGetSystemState()����
    printf("ʹ��uxTaskGetSystemState()����\n");
    task_num = uxTaskGetNumberOfTasks();            // ��ȡϵͳ��������
    status_array = malloc(task_num * sizeof(TaskStatus_t));     // �����ڴ�
    task_num = uxTaskGetSystemState(status_array,task_num,NULL);
    printf("������\t\t���ȼ�\t\t������\r\n");
    for (int j = 0; j < task_num; ++j) {
        printf("%s\t%s%ld\t\t%ld\r\n",
               status_array[j].pcTaskName,
               strlen(status_array[j].pcTaskName) > 7 ? "": "\t",
               status_array[j].uxCurrentPriority,
               status_array[j].xTaskNumber);
    }
    free(status_array);     // �ͷ��ڴ�

    // ����vTaskGetInfo()��ʹ��
    printf("ʹ��vTaskGetInfo()����\n");
    task_info = malloc(sizeof(TaskStatus_t));
    // ��cubemx�л�Ҫ�������Ӧ�ĺ������ã����������Ӧ�ı�־λ��1������
    task_handle = xTaskGetHandle("task1");
    vTaskGetInfo(task_handle,task_info,pdTRUE,eInvalid);
    printf("������:\t\t\t%s\r\n", task_info->pcTaskName);
    printf("������:\t\t%ld\r\n", task_info->xTaskNumber);
    printf("����׳̬:\t\t%d\r\n", task_info->eCurrentState);
    printf("����ǰ���ȼ�:\t\t%ld\r\n", task_info->uxCurrentPriority);
    printf("��������ȼ�:\t\t%ld\r\n", task_info->uxBasePriority);
    printf("�����ջ����ַ:\t\t0x%p\r\n", task_info->pxStackBase);
    printf("�����ջ��ʷʣ����Сֵ:\t%d\r\n", task_info->usStackHighWaterMark);
    free(task_info);

    // ����eTaskGetState()��ʹ��
    printf("����eTaskGetState()��ʹ��\n");
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
    printf("����״ֵ̬: %d����Ӧ״̬Ϊ: %s\r\n", task_state, task_state_str);
    free(task_state_str);

    // ����vTaskList()��ʹ��
    printf("����vTaskList()��ʹ��\n");
    task_info_buf = malloc(500);
    vTaskList(task_info_buf);
    printf("������\t\t״̬\t���ȼ�\tʣ��ջ\t�������\r\n");
    printf("%s\r\n", task_info_buf);
    free(task_info_buf);
    printf("/************************ʵ�����***************************/");


    while (1) {
        vTaskDelay(1000);
    }
}
/* USER CODE END Application */


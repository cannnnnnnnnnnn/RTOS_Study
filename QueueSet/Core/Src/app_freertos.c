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
// ����������
TaskHandle_t taskHandle1;
TaskHandle_t taskHandle2;
// ���кͶ��м����
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
    // �������м�
    queueset_handle = xQueueCreateSet(2);
    if (queueset_handle != NULL){
        printf("���м������ɹ���\n");
    }
    // ��������
    queue_handle = xQueueCreate( 1, sizeof(uint8_t));
    // ������ֵ�ź���
    semphr_handle = xSemaphoreCreateBinary();
    // �����к��ź�����ӵ����м���
    BaseType_t res = 0;
    res = xQueueAddToSet(queue_handle,queueset_handle);
    if (res == pdPASS){
        printf("��ӳɹ���\n");
    }
    res = xQueueAddToSet(semphr_handle,queueset_handle);
    if (res == pdPASS){
        printf("��ӳɹ���\n");
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
//        printf("������\t\t\t״̬\t���ȼ�\tʣ��ջ\t�������\r\n");
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
    // ����һ:ʵ�ֶ��з����Լ��ź����ͷ�
    uint8_t key = 1;
    BaseType_t err = 0;
    while (1){
//        err = xQueueSend(queue_handle,&key,portMAX_DELAY);
//        if (err == pdPASS){
//            printf("����д�����ݳɹ���\n");
//        }
        vTaskDelay(1000);
        // �ͷ��ź���
        err = xSemaphoreGive(semphr_handle);
        if (err == pdPASS){
            printf("�ź����ͷųɹ���\n");
        }
        vTaskDelay(1000);
    }
}

void Task2(void *pvParameters) {
    // �����:��ȡ���м���Ϣ
    QueueSetMemberHandle_t member_handle;
    uint8_t key;
    while (1){
        member_handle = xQueueSelectFromSet(queueset_handle,portMAX_DELAY);
        if (member_handle == NULL){
            printf("��ȡʧ��\n");
        }
        if (member_handle == queue_handle){
            xQueueReceive(member_handle,&key,portMAX_DELAY);
            printf("��ȡ���Ķ�������:%d\n",key);
        } else if (member_handle == semphr_handle){
            xSemaphoreTake(member_handle,portMAX_DELAY);
            printf("��ȡ�ź����ɹ�\n");
        }
    }
}
/* USER CODE END Application */


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
    xTaskCreate(Task1,"task1",300,NULL,osPriorityLow,NULL);
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
    static char InfoBuffer[512] = {0};
    for(;;)
  {
      HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
      vTaskList((char *) &InfoBuffer);
      printf("任务名      任务状态 优先级   剩余栈 任务序号  CPU 核\r\n");
      printf("\r\n%s\r\n", InfoBuffer);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// 列表项任务（只执行一次）
void Task1(void *pvParameters){
    printf("Task1 Start\n");

    // 创建列表和列表项
    List_t list;
    ListItem_t item1;
    ListItem_t item2;
    ListItem_t item3;

    // 初始化
    vListInitialise(&list);
    vListInitialiseItem(&item1);
    vListInitialiseItem(&item2);
    vListInitialiseItem(&item3);

    // 打印列表及其初始化信息
    printf("List Info\n");
    printf("List:%p\t\r\n",&list);
    printf("List->pxIndex:%p\t\r\n",list.pxIndex);
    printf("List->xListEnd:%p\t\r\n",(&list.xListEnd));
    printf("List->uxNumberOfItems:%lu\t\r\n",list.uxNumberOfItems);
    printf("List Item Info\n");
    printf("List Item1:%p\t\r\n",&item1);
    printf("List Item2:%p\t\r\n",&item2);
    printf("List Item3:%p\t\r\n",&item3);

    // 插入第一个列表项
    printf("Insert first item\n");
    vListInsert(&list,&item1);
    printf("List Info\n");
    printf("List:%p\t\r\n",&list);
    // 相同的信息输出
//    printf("List->pxIndex->pxNext:%p\t\r\n",list.pxIndex->pxPrevious);
//    printf("List->pxIndex->pxNext:%p\t\r\n",list.pxIndex->pxNext);
    printf("List->xListEnd->pxNext:%p\t\r\n",list.xListEnd.pxNext);
    printf("List->xListEnd->pxPrevious:%p\t\r\n",list.xListEnd.pxPrevious);
    printf("List->uxNumberOfItems:%lu\t\r\n",list.uxNumberOfItems);
    printf("Item1->pxPrevious:%p\t\r\n",item1.pxPrevious);
    printf("Item1->pxNext:%p\t\r\n",item1.pxNext);

    /**
     * // 在这里体现双向循环列表
    List Info
    List:0x2000291c
    List->xListEnd->pxNext:0x20002908
    List->xListEnd->pxPrevious:0x20002908
    List->uxNumberOfItems:1
    Item1->pxPrevious:0x20002924
    Item1->pxNext:0x20002924
     **/

    // 插入第二个列表项
    printf("Insert Second Item\n");
    vListInsert(&list,&item2);
    printf("List Info\n");
    printf("List->pxIndex->pxPrevious:%p\t\r\n", list.pxIndex->pxPrevious);
    printf("List->pxIndex->pxNext:%p\t\r\n", list.pxIndex->pxNext);
    printf("List->xListEnd->pxPrevious:%p\t\r\n",list.xListEnd.pxPrevious);
    printf("List->xListEnd->pxNext:%p\t\r\n",list.xListEnd.pxNext);
    printf("Item1->pxPrevious:%p\t\r\n",item1.pxPrevious);
    printf("Item1->pxNext:%p\t\r\n",item1.pxNext);
    printf("Item2->pxPrevious:%p\t\r\n",item2.pxPrevious);
    printf("Item2->pxNext:%p\t\r\n",item2.pxNext);

    /** 这里的地址和之前的不同，是因为每一次在修改代码之后烧录程序输出的对象信息都是不一样的
     * Insert Second Item
        List Info
        List->pxIndex->pxPrevious:0x200028f4
        List->pxIndex->pxNext:0x20002908
        List->xListEnd->pxPrevious:0x200028f4
        List->xListEnd->pxNext:0x20002908
        Item1->pxPrevious:0x20002924
        Item1->pxNext:0x200028f4
        Item2->pxPrevious:0x20002908
        Item2->pxNext:0x20002924
     * */

    // 插入列表项三
    printf("Insert Third Item\n");
    vListInsert(&list,&item3);
    printf("List->xListEnd->pxPrevious:%p\t\r\n",list.xListEnd.pxPrevious);
    printf("List->xListEnd->pxNext:%p\t\r\n",list.xListEnd.pxNext);
    printf("Item1->pxPrevious:%p\t\r\n",item1.pxPrevious);
    printf("Item1->pxNext:%p\t\r\n",item1.pxNext);
    printf("Item2->pxPrevious:%p\t\r\n",item2.pxPrevious);
    printf("Item2->pxNext:%p\t\r\n",item2.pxNext);
    printf("Item3->pxPrevious:%p\t\r\n",item3.pxPrevious);
    printf("Item3->pxNext:%p\t\r\n",item3.pxNext);

    /**
        Insert Third Item
        List->xListEnd->pxPrevious:0x200028e0
        List->xListEnd->pxNext:0x20002908
        Item1->pxPrevious:0x20002924
        Item1->pxNext:0x200028f4
        Item2->pxPrevious:0x20002908
        Item2->pxNext:0x200028e0
        Item3->pxPrevious:0x200028f4
        Item3->pxNext:0x20002924
     * */

    // 移除列表项
    printf("Remove Item2\n");
    uxListRemove(&item2);
    printf("List->xListEnd->pxPrevious:%p\t\r\n",list.xListEnd.pxPrevious);
    printf("List->xListEnd->pxNext:%p\t\r\n",list.xListEnd.pxNext);
    printf("Item1->pxPrevious:%p\t\r\n",item1.pxPrevious);
    printf("Item1->pxNext:%p\t\r\n",item1.pxNext);
    printf("Item3->pxPrevious:%p\t\r\n",item3.pxPrevious);
    printf("Item3->pxNext:%p\t\r\n",item3.pxNext);

    /**
     * Remove Item2
        List->xListEnd->pxPrevious:0x200028e0
        List->xListEnd->pxNext:0x20002908
        Item1->pxPrevious:0x20002924
        Item1->pxNext:0x200028e0
        Item3->pxPrevious:0x20002908
        Item3->pxNext:0x20002924
     * */

    //将列表项插入尾部
    printf("Insert End Item\n");
    vListInsertEnd(&list,&item2);
    printf("List->xListEnd->pxPrevious:%p\t\r\n",list.xListEnd.pxPrevious);
    printf("List->xListEnd->pxNext:%p\t\r\n",list.xListEnd.pxNext);
    printf("Item1->pxPrevious:%p\t\r\n",item1.pxPrevious);
    printf("Item1->pxNext:%p\t\r\n",item1.pxNext);
    printf("Item2->pxPrevious:%p\t\r\n",item2.pxPrevious);
    printf("Item2->pxNext:%p\t\r\n",item2.pxNext);
    printf("Item3->pxPrevious:%p\t\r\n",item3.pxPrevious);
    printf("Item3->pxNext:%p\t\r\n",item3.pxNext);

    /**
     * Insert End Item
        List->xListEnd->pxPrevious:0x200028ec
        List->xListEnd->pxNext:0x20002900
        Item1->pxPrevious:0x2000291c
        Item1->pxNext:0x200028d8
        Item2->pxPrevious:0x200028d8
        Item2->pxNext:0x2000291c
        Item3->pxPrevious:0x20002900
        Item3->pxNext:0x200028ec
     * */


    // 在这里加循环是为了判断任务分配的栈控件是否足够
    while(1)
    {
        vTaskDelay(1000);
    }

}
/* USER CODE END Application */


/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     

#include "app.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId AppTaskHandle;
osThreadId Bc95InitTaskHandle;
osThreadId M35InitTaskHandle;
osThreadId ControlTaskHandle;
osThreadId ErrorCheckTaskHandle;
osThreadId Bc95RecvTaskHandle;
osSemaphoreId Bc95RecvSemHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartAppTask(void const * argument);
void StartBc95InitTask(void const * argument);
void StartM35InitTask(void const * argument);
void StartControlTask(void const * argument);
void StartErrorCheckTask(void const * argument);
void StartBc95RecvTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
    
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of Bc95RecvSem */
  osSemaphoreDef(Bc95RecvSem);
  Bc95RecvSemHandle = osSemaphoreCreate(osSemaphore(Bc95RecvSem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of AppTask */
  osThreadDef(AppTask, StartAppTask, osPriorityAboveNormal, 0, 128);
  AppTaskHandle = osThreadCreate(osThread(AppTask), NULL);

  /* definition and creation of Bc95InitTask */
  osThreadDef(Bc95InitTask, StartBc95InitTask, osPriorityIdle, 0, 128);
  Bc95InitTaskHandle = osThreadCreate(osThread(Bc95InitTask), NULL);

  /* definition and creation of M35InitTask */
  osThreadDef(M35InitTask, StartM35InitTask, osPriorityIdle, 0, 128);
  M35InitTaskHandle = osThreadCreate(osThread(M35InitTask), NULL);

  /* definition and creation of ControlTask */
  osThreadDef(ControlTask, StartControlTask, osPriorityIdle, 0, 128);
  ControlTaskHandle = osThreadCreate(osThread(ControlTask), NULL);

  /* definition and creation of ErrorCheckTask */
  osThreadDef(ErrorCheckTask, StartErrorCheckTask, osPriorityIdle, 0, 128);
  ErrorCheckTaskHandle = osThreadCreate(osThread(ErrorCheckTask), NULL);

  /* definition and creation of Bc95RecvTask */
  osThreadDef(Bc95RecvTask, StartBc95RecvTask, osPriorityIdle, 0, 128);
  Bc95RecvTaskHandle = osThreadCreate(osThread(Bc95RecvTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartAppTask function */
void StartAppTask(void const * argument)
{

  /* USER CODE BEGIN StartAppTask */
    
    uint32_t taskStartTime = 0;
    taskStartTime = xTaskGetTickCount();
    
    /* Infinite loop */
    for(;;)
    {
        boot_rinse();
        osDelayUntil(&taskStartTime, 120000);
    }
  /* USER CODE END StartAppTask */
}

/* StartBc95InitTask function */
void StartBc95InitTask(void const * argument)
{
  /* USER CODE BEGIN StartBc95InitTask */
    /* Infinite loop */
    for(;;)
    {
        if(device_status.bc95_run){
            vTaskSuspend(NULL);
        }
        bc95_reboot();
        osDelay(1);
    }
  /* USER CODE END StartBc95InitTask */
}

/* StartM35InitTask function */
void StartM35InitTask(void const * argument)
{
  /* USER CODE BEGIN StartM35InitTask */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END StartM35InitTask */
}

/* StartControlTask function */
void StartControlTask(void const * argument)
{
  /* USER CODE BEGIN StartControlTask */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END StartControlTask */
}

/* StartErrorCheckTask function */
void StartErrorCheckTask(void const * argument)
{
  /* USER CODE BEGIN StartErrorCheckTask */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END StartErrorCheckTask */
}

/* StartBc95RecvTask function */
void StartBc95RecvTask(void const * argument)
{
  /* USER CODE BEGIN StartBc95RecvTask */
    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }
  /* USER CODE END StartBc95RecvTask */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

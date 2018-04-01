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
osThreadId ControlTaskHandle;
osThreadId ErrorCheckTaskHandle;
osThreadId Bc95RecvTaskHandle;
osThreadId Bc95SendTaskHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartAppTask(void const * argument);
void StartBc95InitTask(void const * argument);
void StartControlTask(void const * argument);
void StartErrorCheckTask(void const * argument);
void StartBc95RecvTask(void const * argument);
void StartBc95SendTask(void const * argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of AppTask */
  osThreadDef(AppTask, StartAppTask, osPriorityHigh, 0, 128);
  AppTaskHandle = osThreadCreate(osThread(AppTask), NULL);

  /* definition and creation of Bc95InitTask */
  osThreadDef(Bc95InitTask, StartBc95InitTask, osPriorityIdle, 0, 800);
  Bc95InitTaskHandle = osThreadCreate(osThread(Bc95InitTask), NULL);

  /* definition and creation of ControlTask */
  osThreadDef(ControlTask, StartControlTask, osPriorityAboveNormal, 0, 128);
  ControlTaskHandle = osThreadCreate(osThread(ControlTask), NULL);

  /* definition and creation of ErrorCheckTask */
  osThreadDef(ErrorCheckTask, StartErrorCheckTask, osPriorityIdle, 0, 256);
  ErrorCheckTaskHandle = osThreadCreate(osThread(ErrorCheckTask), NULL);

  /* definition and creation of Bc95RecvTask */
  osThreadDef(Bc95RecvTask, StartBc95RecvTask, osPriorityRealtime, 0, 800);
  Bc95RecvTaskHandle = osThreadCreate(osThread(Bc95RecvTask), NULL);

  /* definition and creation of Bc95SendTask */
  osThreadDef(Bc95SendTask, StartBc95SendTask, osPriorityIdle, 0, 800);
  Bc95SendTaskHandle = osThreadCreate(osThread(Bc95SendTask), NULL);

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
    uint32_t send_data_time_count = 0;
    /* Infinite loop */
    for(;;)
    {
        _system_led;
        
        send_data_time_count++;
        
        if(device_status.device_mode == 1){             //�豸Ϊ����ģʽ
            
            if(device_status.arrears_boot == 0){        //����豸Ƿ����ͣ��
                device_status.boot = 0;
            }
            
            if(device_status.flow_or_time == 1){        //����ģʽ
                
            }else if(device_status.flow_or_time == 2){ //ʱ��ģʽ
                
            }
        }else if(device_status.device_mode == 2){       //�豸Ϊ����ģʽ
            //����ģʽ������ͣ�����ػ�����Զͣ������
            device_status.arrears_boot = 1;      
        }
        
        if(send_data_time_count >= 2880){               //48���ӷ���һ��
            bc95_status.need_send = 1;
            send_data_time_count = 0;
        }
        osDelay(1000);
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
            
            //��ʼ���bc95��������
            /* definition and creation of Bc95RecvTask */
            osThreadDef(Bc95RecvTask, StartBc95RecvTask, osPriorityIdle, 0, 512);
            Bc95RecvTaskHandle = osThreadCreate(osThread(Bc95RecvTask), NULL);
            
            /* definition and creation of ErrorCheckTask */
            osThreadDef(ErrorCheckTask, StartErrorCheckTask, osPriorityIdle, 0, 400);
            ErrorCheckTaskHandle = osThreadCreate(osThread(ErrorCheckTask), NULL);
            
            status_network_led(GPIO_PIN_SET);
            
            //��ʼ����ɺ�ɾ��������
            vTaskDelete(NULL);
        }
        bc95_reboot();
        osDelay(1300);
    }
  /* USER CODE END StartBc95InitTask */
}

/* StartControlTask function */
void StartControlTask(void const * argument)
{
  /* USER CODE BEGIN StartControlTask */
    uint8_t no_water_count = 0;
    /* Infinite loop */
    for(;;)
    {
        //boot_rinse();
        
        //�����Ҫ��ϴ��������ϴ
        if(device_status.rinse){
            device_status.rinse = 0;
            start_rinse();
            osDelay(device_status.rinse_time * 1000);
            stop_rinse();
        }
        
        //����Ƿ�ȱˮ
        if(get_low_switch()){
            no_water_count++;
            stop_rinse();
            
            if(no_water_count >= 5){    //�������10��û��ˮ
                device_error.raw_no_water = 1;
                
            }
        }else{
            //���ȱˮ����
            no_water_count = 0;
            device_error.raw_no_water = 0;
            //�����ٴη���ȱˮ��Ϣ
            if(device_error.raw_no_water_sended == 1){
                device_status.processing_status = 0;
                bc95_status.need_send = 1;
                device_error.raw_no_water_sended = 0;
            }
            
            //ˮ����ֹͣ��ˮ
            if(get_high_switch()){
                stop_create_water();
            }else{      //ˮδ����������ˮ
                start_create_water();
            }
        }
        osDelay(2000);
    }
  /* USER CODE END StartControlTask */
}

/* StartErrorCheckTask function */
void StartErrorCheckTask(void const * argument)
{
  /* USER CODE BEGIN StartErrorCheckTask */
    
    uint8_t leakage_count = 0;
    
    /* Infinite loop */
    for(;;)
    {
        feed_wdi();     //ι��
        
        //�ۼ�20��©ˮ�����
        uint32_t leakage_value = 0;
        for(uint8_t i = 0 ; i < 20 ; i++){
            leakage_value += adc_value[ i * 3 ];
        }
        /* ����©ˮ��⵽����ֵ�����ж��Ƿ�©ˮ���������©ˮ��©ˮ����+1
        �����©ˮ����0�� 
        */
        if(leakage_value/20 <= 2500){
            leakage_count++;
        }else{
            leakage_count = 0;
        }
        
        /* �������©ˮ����30�룬������©ˮ��־��
        �����©ˮ�ˣ������ÿ��Է��ʹ����־λ�����������
        */
        if(leakage_count >= 6){
            device_error.leakage = 1;
            leakage_count = 6;
            beep(100);
        }else{
            device_error.leakage = 0;
            if(device_error.leakage_sended == 1){
                device_status.processing_status = 0;
                bc95_status.need_send = 1;
                device_error.leakage_sended = 0;
            }
        }
        
        //�ж��Ƿ��д���,����д�������������ʲô����
        if(device_error.bc95_init_error != 0 ||         // bc95��ʼ��ʧ��
           device_error.create_water_too_long != 0 ||   // ��ˮʱ�����
               device_error.leakage != 0 ||                 // ©ˮ
                   device_error.raw_no_water != 0 ||            // ԭˮȱˮ
                       device_status.arrears_boot == 0 )            // ͣ��
        {
            
            device_status.processing_status = 3;        //���ô���
            
            //���BC95�Ƿ��ʼ��ʧ�ܣ����ʧ�������³�ʼ��
            if(device_error.bc95_init_error == BC95_CONNECT_ERROR){
                beep(50);
                //���BC95��ʼ��ʧ�ܣ������´�����ʼ������
                /* definition and creation of Bc95InitTask */
                osThreadDef(Bc95InitTask, StartBc95InitTask, osPriorityIdle, 0, 512);
                Bc95InitTaskHandle = osThreadCreate(osThread(Bc95InitTask), NULL);
            }
            
            //����Ƿ��Ѿ�ȱˮ������Ѿ�ȱˮ������û�з��ʹ�����Ϣ�����ʹ���
            if(device_error.raw_no_water == 1 && device_error.raw_no_water_sended == 0){
                bc95_status.need_send = 1;
            }
            
            //����Ƿ���©ˮ������Ѿ�©ˮ������û�з��ʹ�����Ϣ�����ʹ�����Ϣ
            if(device_error.leakage == 1 && device_error.leakage_sended == 0){
                device_status.boot = 0;                 //©ˮ�ػ�,
                bc95_status.need_send = 1;
            }
            status_error_led(GPIO_PIN_SET);
        }else{
            device_status.processing_status = 0;
            status_error_led(GPIO_PIN_RESET);
        }
        
        osDelay(5000);
    }
  /* USER CODE END StartErrorCheckTask */
}

/* StartBc95RecvTask function */
void StartBc95RecvTask(void const * argument)
{
  /* USER CODE BEGIN StartBc95RecvTask */
    uint8_t count = 0;
    /* Infinite loop */
    for(;;)
    {
        _network_led;
        count++;
        
        if(count >= 5){
            count = 0;
            //��ȡ����״̬���������������ע��
            if(!bc95_get_profile_status()){
                bc95_send_command("AT+CGATT=1\r\n", "OK", BC95_TIMEOUT, BC95_LOOP_NUMBER);
            }
            
            bc95_get_csq();
        }
        bc95_check_recv();
        osDelay(3000);
    }
  /* USER CODE END StartBc95RecvTask */
}

/* StartBc95SendTask function */
void StartBc95SendTask(void const * argument)
{
  /* USER CODE BEGIN StartBc95SendTask */
    /* Infinite loop */
    for(;;)
    {
        if(bc95_status.need_send == 1){
            bc95_status.need_send = 0;
            
            if(device_error.leakage_sended == 0 && device_error.leakage == 1){
                device_error.leakage_sended = 1;
                device_status.processing_status = COAP_DEVICE_ERROR;
            }
            
            if(device_error.raw_no_water_sended == 0 && device_error.raw_no_water == 1){
                device_error.raw_no_water_sended = 1;
                device_status.processing_status = COAP_DEVICE_ERROR;
            }
            
            bc95_send_coap(cmd_coap_send_ok);
        }
        osDelay(1000);
    }
  /* USER CODE END StartBc95SendTask */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

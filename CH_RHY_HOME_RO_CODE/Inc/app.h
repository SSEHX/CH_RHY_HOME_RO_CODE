#ifndef __APP_H__
#define	__APP_H__
#include "string.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "bc95.h"
#include "i2c_eeprom.h"

#define QR_CODE_LEN 20

#define ADC_CHANNEL_NUMBER      7
#define ADC_LOOP_NUMBER         5

typedef struct {
    uint8_t     booting;                // �Ѿ�����
    uint8_t     processing_status;
    uint8_t     bc95_run;               // bc95��������
    uint32_t    boot;                   // �豸���ػ�
    uint32_t    arrears_boot;           // ͣ�����ػ�
    uint32_t    device_registe;         // �豸ע��״̬
    uint32_t    device_mode;            // �豸����
    uint16_t    pure_tds;               // ��ˮTDS 
    uint16_t    raw_tds;                // ԭˮTDS
    uint32_t    time;                   // ����ʱ��
    uint32_t    flow;                   // ����
    uint32_t    rinse;                  // ��ϴ
    uint8_t     rinse_time;             // ��ϴʱ��
    uint32_t    filter[10];             // ��о
    uint32_t    create_water_time_m;    // ��ˮʱ�����
    uint8_t     flow_or_time;           // ����������������ʱ��
    uint8_t     create_water_time_rinse;// ��ˮ�೤ʱ���ϴ ���� 15����һ����
    uint32_t    proportion;             // ʱ�任�����
    uint8_t     use_water_table;        // �Ƿ�ʹ��ˮλ
    uint8_t     qr_code[QR_CODE_LEN+1]; // ��ά��
}device_statusTypeDef;

extern device_statusTypeDef device_status; 

typedef struct{
    uint8_t     bc95_init_error;
    uint8_t     leakage;                    // leakage flag
    uint8_t     leakage_sended;             // ©ˮ�����ѷ���
    uint8_t     raw_no_water;               // no water flag
    uint8_t     raw_no_water_sended;        // ȱˮ�����ѷ���
    uint8_t     create_water_too_long;
    
}device_errorTypeDef;

extern device_errorTypeDef device_error;

extern uint32_t device_adc_value[ADC_CHANNEL_NUMBER*ADC_LOOP_NUMBER-1];

void boot_rinse();
uint8_t registe_device();

#endif
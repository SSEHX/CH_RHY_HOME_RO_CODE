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

#define QR_CODE_LEN 20

#define SYSTEM_LED_PORT         GPIOA
#define SYSTEM_LED_PIN          GPIO_PIN_15
#define _system_led             HAL_GPIO_TogglePin(SYSTEM_LED_PORT, SYSTEM_LED_PIN)

#define SWITCH_1_PORT           GPIOC
#define SWITCH_1_PIN            GPIO_PIN_1

#define SWITCH_2_PORT           GPIOC
#define SWITCH_2_PIN            GPIO_PIN_2

#define SWITCH_3_PORT           GPIOC
#define SWITCH_3_PIN            GPIO_PIN_3

#define SWITCH_4_PORT           GPIOC
#define SWITCH_4_PIN            GPIO_PIN_4

#define SWITCH_5_PORT           GPIOC
#define SWITCH_5_PIN            GPIO_PIN_5

#define PUMP_PORT               GPIOC
#define PUMP_PIN                GPIO_PIN_6

#define HIGHT_SWITCH_PORT       GPIOB
#define HIGHT_SWITCH_PIN        GPIO_PIN_0

#define ADC_CHANNEL_NUMBER      7
#define ADC_LOOP_NUMBER         5

typedef struct {
    uint8_t     booting;                 //已经启动
    uint8_t     processing_status;
    uint8_t     bc95_run;               //bc95正常运行
    uint32_t    boot;                   //设备开关机
    uint32_t    arrears_boot;           //停机开关机
    uint32_t    device_registe;         //设备注册状态
    uint32_t    device_mode;            //设备类型
    uint16_t    pure_tds;
    uint16_t    raw_tds;
    uint32_t    time;
    uint32_t    flow;
    uint32_t    rinse;
    uint32_t    filter[10];
    uint32_t    create_water_time_m;
    uint8_t     qr_code[QR_CODE_LEN+1];
}device_statusTypeDef;

extern device_statusTypeDef device_status; 

typedef struct{
    uint8_t     bc95_init_error;
    uint8_t     leakage;                    // leakage flag
    uint8_t     raw_no_water;               // no water flag
    uint8_t     create_water_too_long;
}device_errorTypeDef;

extern device_errorTypeDef device_error;

extern uint32_t device_adc_value[ADC_CHANNEL_NUMBER*ADC_LOOP_NUMBER-1];

void test(uint8_t* string);
void boot_rinse();

#endif
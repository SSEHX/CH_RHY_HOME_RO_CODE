#ifndef __TOOLS_H__
#define	__TOOLS_H__
#include "main.h"
#include "stm32f1xx_hal.h"
#include "app.h"

#define FLASH_DATA_ADDR 0x0800F000
#define FLASH_DATA_LEN  50

#define FIRST_BOOT_ADDR     0
#define REGISTE_ADDR        1                                         //
#define CREATE_WATER_TIME_M 2
#define CREATE_WATER_FIRST  3
#define TIME_ADDR           4                                         //
#define FLOW_ADDR           5                                         //
#define BOOT_ADDR           6                                         //
#define DEVICE_MODE_ADDR    7         //1:租赁  2：零售                //  

#define FILTER_ADDR         8         //10个滤芯，依次后排    到19      //

#define RINSE_TIME          19        //冲洗时间                      //
#define ARREARS_BOOT_ADDR   20        //停机开关机                    //   
#define QR_CODE_ADDR        21        //二维码                        //


extern uint32_t flash_pages_data[FLASH_DATA_LEN];

uint8_t *ByteToHexStr(const uint8_t* source, uint32_t sourceLen);

void flash_write(uint32_t *data);
void flash_write_word(uint8_t address, uint32_t data);
uint32_t flash_read(uint32_t addr);

void processing_server_command();
void flash_device_status();

void pump_open();
void pump_close();
void switch1_open();
void switch1_close();
void switch2_open();
void switch2_close();
void switch3_open();
void switch3_close();
void switch4_open();
void switch4_close();
void switch5_open();
void switch5_close();


void start_rinse();
void stop_rinse();
void start_create_water();
void stop_create_water();
void start_pure_water_protect_RO();
void stop_pure_water_protect_RO();
void start_pure_water_protect_UF();
void stop_pure_water_protect_UF();
void stop_all_switch();
void leak_protect();

#endif
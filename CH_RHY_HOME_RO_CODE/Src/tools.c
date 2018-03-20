#include "tools.h"
#include <stdlib.h>

uint32_t flash_pages_data[FLASH_DATA_LEN] = {0};


uint8_t *ByteToHexStr(const uint8_t* source, uint32_t sourceLen)  
{  
    uint8_t a[100] = {0};
    uint8_t *dest = a;
    short i;  
    unsigned char highByte, lowByte;  
  
    for (i = 0; i < sourceLen; i++)  
    {  
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;
  
        highByte += 0x30;  
  
        if (highByte > 0x39)  
                dest[i * 2] = highByte + 0x07;  
        else  
                dest[i * 2] = highByte;  
  
        lowByte += 0x30;  
        if (lowByte > 0x39)  
            dest[i * 2 + 1] = lowByte + 0x07;  
        else  
            dest[i * 2 + 1] = lowByte;  
    }
    return dest;  
}

/*----------------------------------------------------------------
 | Function    :    flash_write
 | Description :    write the *data to flash 
 | Input       :    
 | Output      :    
 | Return      :    
----------------------------------------------------------------*/
void flash_write(uint32_t *data){

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = FLASH_DATA_ADDR;
    f.NbPages = 1;
    uint32_t PageError = 0;
    HAL_FLASHEx_Erase(&f, &PageError);

    uint32_t addr = FLASH_DATA_ADDR;
    for(uint8_t i = 0 ; i < FLASH_DATA_LEN ; i++){
        
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, *data++);
        addr += 4;
    }
    HAL_FLASH_Lock();
}

void flash_write_word(uint8_t address, uint32_t data){

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = FLASH_DATA_ADDR;
    f.NbPages = 1;
    uint32_t PageError = 0;
    HAL_FLASHEx_Erase(&f, &PageError);

    uint32_t addr = FLASH_DATA_ADDR;

    flash_pages_data[address] = data;

    for(uint8_t i = 0 ; i < FLASH_DATA_LEN ; i++){
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, flash_pages_data[i]);
        addr += 4;
    }
    HAL_FLASH_Lock();
}

uint32_t flash_read(uint32_t addr){
    return *(uint32_t*)(FLASH_DATA_ADDR+(addr*4));
}

void processing_server_command(){
//    0  2  4  6        14       22 24 26 28 30 32 34 36 38 40 42 44 46
//    01 01 00 00000000 00000000 00 64 64 64 64 64 64 64 64 64 64 01 43482D524F2D3030303031232323232323232323
    /*!< A 设备开机 */
    uint8_t boot[3] = {0};              
    memmove(boot, &bc95_recv.server_cmd[0], 2);
    device_status.boot = atoi((const char*)boot);
    
    /*!< B 停机开关机 */
    uint8_t arrears_boot[3] = {0};
    memmove(arrears_boot, &bc95_recv.server_cmd[2], 2);
    sscanf((const char*)arrears_boot, "%x", &device_status.arrears_boot);

    /*!< C 冲洗开关 */
    uint8_t rinse[3] = {0};
    memmove(rinse, &bc95_recv.server_cmd[4], 2);
    sscanf((const char*)rinse, "%x", &device_status.rinse);
    
    /*!< D 时间 */
    uint8_t time[9] = {0};
    memmove(time, &bc95_recv.server_cmd[6], 8);
    sscanf((const char*)time, "%x", &device_status.time);
    
    /*!< F 流量 */
    uint8_t flow[9] = {0};
    memmove(flow, &bc95_recv.server_cmd[14], 8);
    sscanf((const char*)flow, "%x", &device_status.flow);
    
    /*!< F 设备类型  */
    uint8_t device_mode[3] = {0};
    memmove(device_mode, &bc95_recv.server_cmd[22], 2);
    sscanf((const char*)device_mode, "%x", &device_status.device_mode);

    /*!< g1-g10 滤芯 */
    for(uint8_t i = 0 ; i < 9 ; i++){
        uint8_t filter[3] = {0};
        memmove(filter, &bc95_recv.server_cmd[24 + (i * 2)], 2);
        sscanf((const char*)filter, "%02x", &device_status.filter[i]);
    }
    
    /*!< H 注册是否成功*/
    uint8_t device_registe[3] = {0};
    memmove(device_registe, &bc95_recv.server_cmd[44], 2);
    sscanf((const char*)device_registe, "%x", &device_status.device_registe);

    /*!< I 二维码 */
    uint8_t qr_code[(QR_CODE_LEN*2)+1] = {0};
    memmove(qr_code, &bc95_recv.server_cmd[46], (sizeof(device_status.qr_code) * 2));
    for(uint8_t i = 0 ; i < QR_CODE_LEN ; i++){
        uint8_t temp[3] = {0};
        memmove(temp, &qr_code[ (i*2) ], 2);
        sscanf((const char*)temp , "%x", (uint32_t *)&device_status.qr_code[i]);
    }

    /*!< I 冲洗时间 */
    //uint8_t rinse_time[3] = {0};
    //memmove(rinse_time, &bc95_recv.server_cmd[86], 2);
    //sscanf((const char*)rinse_time, "%x", &device_status.rinse_time);

    //保存所有设备状态到eeprom
    //flash_device_status();
    
    //设置处理状态为成功 -> 发送当前状态 -> 重置处理状态
    device_status.processing_status = 1;
    bc95_send_coap("+NSMI:SENT");
    device_status.processing_status = 0;
}
/*----------------------------------------------------------------
 | Function    :    flash_device_status
 | Description :    save the device status to flash
 | Input       :    null
 | Output      :    null
 | Return      :    null
----------------------------------------------------------------*/
void flash_device_status(){
    flash_pages_data[TIME_ADDR]   =    device_status.time;
    flash_pages_data[FLOW_ADDR]   =    device_status.flow;
    flash_pages_data[BOOT_ADDR]   =    device_status.boot;
    flash_pages_data[REGISTE_ADDR]        =    device_status.device_registe;
    flash_pages_data[DEVICE_MODE_ADDR]    =    device_status.device_mode;
    flash_pages_data[ARREARS_BOOT_ADDR]   =    device_status.arrears_boot;
    //滤芯
    for(uint8_t i = 0 ; i < 10 ; i++){
        flash_pages_data[FILTER_ADDR+i]   =    device_status.filter[i];
    }
    //二维码
    for(uint8_t i = 0 ; i < QR_CODE_LEN ; i++){
        flash_pages_data[QR_CODE_ADDR+i]  =    device_status.qr_code[i];
    }

    flash_write(flash_pages_data);
}

void pump_open(){
    HAL_GPIO_WritePin(PUMP_PORT    , PUMP_PIN    , GPIO_PIN_RESET);
}

void pump_close(){
    HAL_GPIO_WritePin(PUMP_PORT    , PUMP_PIN    , GPIO_PIN_SET);
}

void switch1_open(){
    HAL_GPIO_WritePin(SWITCH_1_PORT, SWITCH_1_PIN, GPIO_PIN_RESET);
}

void switch1_close(){
    HAL_GPIO_WritePin(SWITCH_1_PORT, SWITCH_1_PIN, GPIO_PIN_SET);
}

void switch2_open(){
    HAL_GPIO_WritePin(SWITCH_2_PORT, SWITCH_2_PIN, GPIO_PIN_RESET);
}

void switch2_close(){
    HAL_GPIO_WritePin(SWITCH_2_PORT, SWITCH_2_PIN, GPIO_PIN_SET);
}
void switch3_open(){
    HAL_GPIO_WritePin(SWITCH_3_PORT, SWITCH_3_PIN, GPIO_PIN_RESET);
}

void switch3_close(){
    HAL_GPIO_WritePin(SWITCH_3_PORT, SWITCH_3_PIN, GPIO_PIN_SET);
}
void switch4_open(){
    HAL_GPIO_WritePin(SWITCH_4_PORT, SWITCH_4_PIN, GPIO_PIN_RESET);
}

void switch4_close(){
    HAL_GPIO_WritePin(SWITCH_4_PORT, SWITCH_4_PIN, GPIO_PIN_SET);
}
void switch5_open(){
    HAL_GPIO_WritePin(SWITCH_5_PORT, SWITCH_5_PIN, GPIO_PIN_RESET);
}

void switch5_close(){
    HAL_GPIO_WritePin(SWITCH_5_PORT, SWITCH_5_PIN, GPIO_PIN_SET);
}


void start_rinse(){
    switch1_close();
    switch2_close();
    switch4_close();
    
    switch3_open();
    switch5_open();
    pump_open();
}

void stop_rinse(){
    switch3_close();
    switch5_close();
    pump_close();
}

void start_create_water(){
    switch1_close();
    switch2_close();
    switch4_close();
    switch5_close();
    
    switch3_open();
    pump_open();
}

void stop_create_water(){
    switch3_close();
    pump_close();
}

void start_pure_water_protect_RO(){
    switch1_close();
    switch2_close();
    switch3_close();
    pump_close();
    
    switch4_open();
    switch5_open();
}

void stop_pure_water_protect_RO(){
    switch4_close();
    switch5_close();
}

void start_pure_water_protect_UF(){
    switch5_close();
    switch3_close();
    pump_close();
    
    switch1_open();
    switch2_open ();
    switch4_open();
    
}

void stop_pure_water_protect_UF(){
    switch1_close();
    switch2_close();
    switch4_close();
}

void stop_all_switch(){
    switch1_close();
    switch2_close();
    switch4_close();
    switch5_close();
    switch3_close();
    pump_close();
}

void leak_protect(){
    switch1_open();
    switch2_close();
    switch4_close();
    switch5_close();
    switch3_close();
    pump_close();
}
#include "App.h"

device_statusTypeDef device_status = {0};
device_errorTypeDef device_error   = {0};

uint32_t device_adc_value[ADC_CHANNEL_NUMBER*ADC_LOOP_NUMBER-1];

void test(uint8_t* string){
    HAL_UART_Transmit_DMA(&huart1, string, 10);
}


/*----------------------------------------------------------------
 | Function    :    boot_rinse
 | Description :    ¿ª»ú³åÏ´2·ÖÖÓ
 | Input       :    null
 | Output      :    null
 | Return      :    null
----------------------------------------------------------------*/
void boot_rinse(){
    if(device_status.booting == 0){
        TickType_t StartTime = xTaskGetTickCount();
        start_rinse();
        device_status.booting = 1;
        osDelayUntil(&StartTime, 120000);
        stop_rinse();
    }
}

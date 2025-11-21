#include "system_logger.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void){
    system_logger_log_message("system logger says heyyy girlll\n");
    system_logger_log_message("testing an integer: %d\n", 32);
    system_logger_log_message("testing a string: %s and a float: %.2f\n", "ESP32", 3.14);

    while (1){
        vTaskDelay(1000 / portTICK_PERIOD_MS); // pause task for 1000 ms
    }
}
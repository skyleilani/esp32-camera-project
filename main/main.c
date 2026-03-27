#include "sccb_interface.h"
#include "esp_log.h"

static const char *TAG = "MAIN_APP";

void app_main(void){
    esp_err_t init_status = sccb_interface_init();
    
    if (init_status != ESP_OK) {
        ESP_LOGE(TAG, "I2C manager initialization failed with error: %s. Aborting.", esp_err_to_name(init_status));
        return;
    }

    ESP_LOGI(TAG, "I2C manager initialized successfully. Starting bus scan.");
    sccb_interface_scan();

    ESP_LOGI(TAG, "Scan complete.");
}
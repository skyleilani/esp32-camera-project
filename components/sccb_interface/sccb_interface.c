#include "sccb_interface.h"
#include "esp_log.h"

// tag for logging
static const char *TAG = "SCCB_INTERFACE";

// initialized I2C peripheral object. Stores state and config info for I2C 
static i2c_master_bus_handle_t sccb_bus_handle = NULL;

// setup I2C hardware (configure I2C bus)
esp_err_t sccb_interface_init(void) {
    // check if already initialized so there's no resource leaks
    if (sccb_bus_handle != NULL){
        ESP_LOGW(TAG, "SCCB interface is already initialized");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Initializing SCCB interface...");

    // configure i2c bus params 
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = SCCB_I2C_PORT,
        .sda_io_num = SCCB_SDA_GPIO, // SDA GPIO
        .scl_io_num = SCCB_SCL_GPIO, // SCL GPIO
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,              // number of consecutive glitches to be ignored by I2C bus
        .intr_priority = 0,
        .flags.enable_internal_pullup = true, // instruct driver to enable SDA and SCL pull-up resistors
    };

    // create I2C master bus handle
    esp_err_t err = i2c_new_master_bus(&i2c_bus_config,&sccb_bus_handle);
    if (err!= ESP_OK) {
        ESP_LOGE(TAG, "I2C master bus initialization failed: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGD(TAG, "I2C bus created on port %d", SCCB_I2C_PORT);
    return ESP_OK;
}

// loop from address 0x01 to 0x7F and attempt I2C communication. Log if peripheral sends ACK
void sccb_interface_scan(void) {
    if (sccb_bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C bus isn't initialized. Call sccb_interface_init() first.");
        return;
    }

    ESP_LOGI(TAG, "Scanning I2C bus...");

    uint8_t devices_found = 0;

    for (uint8_t address = 1; address < 127; address++){
        // See if peripheral sends ACK.
        esp_err_t err = i2c_master_probe(sccb_bus_handle, address, 50);
        
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Device found at address: 0x%02X", address);
            devices_found++;
        }
    }

    if (devices_found == 0) {
        ESP_LOGW(TAG, "No devices found on I2C bus");
    } else {
        ESP_LOGI(TAG, "Scan complete: %d devices found", devices_found);
    }
}
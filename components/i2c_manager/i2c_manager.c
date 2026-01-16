#include "i2c_manager.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

// tag for logging
static const char *TAG = "I2C_MANAGER";

// I2C Configuration Macros
#define I2C_MASTER_PORT_NUM          I2C_NUM_0     // using default controller
#define I2C_MASTER_SCL_IO            22            
#define I2C_MASTER_SDA_IO            21
#define I2C_MASTER_FREQ_HZ           100000        // clock rate 100kHz (standard mode)

// handle to I2c bus
static i2c_master_bus_handle_t bus_handle;

// configure I2C bus
esp_err_t i2c_manager_init(void) {
    // config structure for i2c bus
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = I2C_MASTER_PORT_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO, // SDA GPIO
        .scl_io_num = I2C_MASTER_SCL_IO, // SCL GPIO
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,              // number of consecutive glitches to be ignored by I2C bus
        .flags.enable_internal_pullup = true,
    };

    // create I2C bus handle
    esp_err_t err = i2c_new_master_bus(&i2c_bus_config,&bus_handle);
    if (err!= ESP_OK) {
        ESP_LOGE(TAG, "I2C master bus initialization failed: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "I2C manager initialized succesfully on por %d.", I2C_MASTER_PORT_NUM);
    return ESP_OK;
}
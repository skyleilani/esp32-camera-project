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

// initialized I2C peripheral object. Stores state and config info for I2C 
static i2c_master_bus_handle_t bus_handle;

// setup I2C hardware (configure I2C bus)
esp_err_t i2c_manager_init(void) {
    // Set config with all params needed for the bus
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = I2C_MASTER_PORT_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO, // SDA GPIO
        .scl_io_num = I2C_MASTER_SCL_IO, // SCL GPIO
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,              // number of consecutive glitches to be ignored by I2C bus
        .flags.enable_internal_pullup = true, // instruct driver to enable SDA and SCL pull-up resistors
    };

    // create I2C bus handle according to i2c_bus_config params
    esp_err_t err = i2c_new_master_bus(&i2c_bus_config,&bus_handle);
    if (err!= ESP_OK) {
        ESP_LOGE(TAG, "I2C master bus initialization failed: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "I2C manager initialized succesfully on por %d.", I2C_MASTER_PORT_NUM);
    return ESP_OK;
}

// loop from address 0x01 to 0x7F and attempt I2C communication. Log if peripheral sends ACK
void i2c_manager_scan(void) {
    if (bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C manager isn't initialized. Call i2c_manager_init() first to do so.");
        return;
    }

    ESP_LOGI(TAG, "Scanning I2C bus...");
    uint8_t address;
    esp_err_t err;

    for (address = 1; address < 127; address++){
        // See if peripheral sends ACK.
        err = i2c_master_probe(bus_handle, address, 50);
        
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Device found at address: 0x%02X", address);
        } else if (err != ESP_ERR_TIMEOUT) {
            ESP_LOGW(TAG, "Error probing address 0x%02X: %s", address, esp_err_to_name(err));
        }
    }
}
#include "i2c_manager.h"
#include "driver/i2c_master.h"
#include "esp_log.h"

// tag for logging
static const char *TAG = "I2C_MANAGER";

// I2C Configuration Macros
#define I2C_CONTROLLER_PORT_NUM          I2C_NUM_0     // using default controller
#define I2C_CONTROLLER_SCL_IO            22            
#define I2C_CONTROLLER_SDA_IO            21
#define I2C_CONTROLLER_FREQ_HZ           100000        // clock rate 100kHz (standard mode)

// handle to I2c bus
static i2c_master_bus_handle_t bus_handle;

// configure I2C bus
esp_err_t i2c_manager_init(void) {
    // config structure for i2c bus
    i2c_master_bus_config_t conf = {
        .i2c_port = I2C_CONTROLLER_PORT_NUM,
        .sda_io_num = I2C_CONTROLLER_SDA_IO, // SDA GPIO
        .scl_io_num = I2C_CONTROLLER_SCL_IO, // SCL GPIO
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    
}
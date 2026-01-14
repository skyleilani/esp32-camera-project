#include "i2c_manager.h"
#include "driver/i2c.h"
#include "esp_log.h"

// tag for logging
static const char *TAG = "I2C_MANAGER";

// I2C Configuration Macros
#define I2C_CONTROLLER_PORT_NUM          I2C_NUM_0     // using default controller
#define I2C_CONTROLLER_SCL_IO            22            
#define I2C_CONTROLLER_SDA_IO            21
#define I2C_CONTROLLER_FREQ_HZ           100000        // clock rate 100kHz (standard mode)
#define I2C_CONTROLLER_TX_BUF_DISABLE    0             // disable internal driver ring buffer
#define I2C_CONTROLLER_RX_BUF_DISABLE    0             
#define ACK_CHECK_EN                     0x1           // enable controller check for an ACK from target; verify successful communication
#define ACK_CHECK_DIS                    0x0           
#define ACK_VAL                          0x0           // success acknowledgement 
#define NACK_VAL                         0x1           // negative acknowledgement

esp_err_t i2c_manager_init(void) {
    return ESP_OK;
}
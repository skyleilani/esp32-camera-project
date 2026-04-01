#ifndef SCCB_INTERFACE_H
#define SCCB_INTERFACE_H

#include "esp_err.h"
#include "driver/i2c_master.h"

// SCCB_Interface config params
#define SCCB_I2C_PORT I2C_NUM_0
#define SCCB_SCL_GPIO 22
#define SCCB_SDA_GPIO 21
#define SCCB_FREQ_HZ 100000
#define SCCB_DEVICE_ADDR_WRITE 0x30 // OV2640 SCCB write addy
#define SCCB_DEVICE_ADDR_READ 0x31 // OV2640 SCCB read addy
#define SCCB_TIMEOUT_MS 100

// initialize sccb interface
esp_err_t sccb_interface_init(void);

// write 8 bit value to ov2640 register
esp_err_t sccb_write_reg(uint8_t reg_addr, uint8_t value);

// read 8 bit value from register
esp_err_t sccb_read_reg(uint8_t reg_addr, uint8_t value);

// write multiple bytes to ov2640 registers
esp_err_t sccb_write_regs(uint8_t reg_addr, const uint8_t *data, size_t data_len);

// read multiple bytes from ov2640 registers
esp_err_t sccb_read_regs(uint8_t reg_addr, uint8_t *data, size_t data_len);

// scan i2c bus for devices
void sccb_interface_scan(void);

// probe a specific i2c addy
esp_err_t sccb_probe_address(uint8_t address);

#endif
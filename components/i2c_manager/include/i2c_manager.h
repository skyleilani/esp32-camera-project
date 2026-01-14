#ifndef I2C_MANAGER_H
#define I2C_MANAGER_H
#include "esp_err.h"

esp_err_t i2c_manager_init(void);

void i2c_manager_scan(void);

#endif // I2C_MANAGER_H
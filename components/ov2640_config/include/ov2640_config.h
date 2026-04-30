#ifndef OV2640_CONFIG_H
#define OV2640_CONFIG_H

#include <stdint.h>
#include <esp_err.h>

/**
 * @brief Configure OV2640 SCCB/I2C registers with expected value (register baseline) and verify match
 * 
 * @param[out] failed_reg_addr Optional: OV2640 register address that contains a mismatched value
 *                              May be NULL if caller doesn't need failed register address
 *                              Value is only valid on failure                             
 * @return esp_err_t
 *          - ESP_OK: Success
 *          - ESP_ERR_INVALID_STATE: Verification failed
 */
esp_err_t ov2640_configure_and_verify(uint8_t *failed_reg_addr);

#endif // OV2640_CONFIG_H
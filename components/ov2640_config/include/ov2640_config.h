#ifndef OV2640_CONFIG_H
#define OV2640_CONFIG_H

#include <stdint.h>
#include "esp_err.h"
#include "sccb_interface.h"

typedef struct {
    uint8_t reg_addr;
    uint8_t expected_value;
} reg_entry_t;

esp_err_t ov2640_configure_and_verify(uint8_t *failed_reg_addr);

#endif // OV2640_CONFIG_H
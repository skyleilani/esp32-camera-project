#include "ov2640_config.h"

typedef struct {
    uint8_t reg_addr;
    uint8_t expected_value;
} ov2640_reg_entry_t;
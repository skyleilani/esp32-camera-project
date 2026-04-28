#include "ov2640_config.h"
#include "sccb_interface.h"

typedef struct {
    uint8_t reg_addr;
    uint8_t expected_value;
} ov2640_reg_entry_t;

static const ov2640_reg_entry_t baseline[] = {
    {0x0A, 0x26},   // Product ID
    {0x12, 0x80},   // Soft Reset
    {0x11, 0x00},   // Clock Control
};

static const size_t baseline_count = sizeof(baseline) / sizeof(baseline[0]);
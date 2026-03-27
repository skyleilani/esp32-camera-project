#ifndef SCCB_INTERFACE_H
#define SCCB_INTERFACE_H
#include "esp_err.h"

esp_err_t sccb_interface_init(void);

void sccb_interface_scan(void);

#endif // SCCB_INTERFACE_H
#ifndef SCCB_INTERFACE_H
#define SCCB_INTERFACE_H

// SCCB_Interface config params
#define SCCB_I2C_PORT I2C_NUM0
#define SCCB_SCL_GPIO 22
#define SCCB_SDA_GPIO 21
#define SCCB_FREQ_HZ 100000
#define SCCB_DEVICE_ADDR_WRITE 0x30 // OV2640 SCCB write addy
#define SCCB_DEVICE_ADDR_READ 0x31 // OV2640 SCCB read addy
#define SCCB_TIMEOUT_MS 100

#endif
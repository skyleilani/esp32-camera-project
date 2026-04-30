#ifndef CAMERA_INTERFACE_H
#define CAMERA_INTERFACE_H

#include <stdint.h>
#include <stddef.h>
#include <esp_err.h>

esp_err_t camera_init(void);

esp_err_t camera_configure_and_verify(uint8_t *failed_reg_addr);

esp_err_t camera_capture_frame(uint8_t *out_buffer, size_t buffer_size, size_t *bytes_captured);

esp_err_t camera_deinit(void);

#endif // CAMERA_INTERFACE_H
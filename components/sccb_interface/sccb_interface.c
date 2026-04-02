#include "sccb_interface.h"
#include "esp_log.h"
#include "esp_check.h"

// tag for logging
static const char *TAG = "SCCB_INTERFACE";

// initialized I2C peripheral object. Stores state and config info for I2C 
static i2c_master_bus_handle_t sccb_bus_handle = NULL;
static i2c_master_dev_handle_t sccb_dev_handle = NULL;

esp_err_t sccb_create_device(void) {
    if (sccb_bus_handle == NULL) {
        ESP_LOGE(TAG, "Can't create the device, I2C bus not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_7,
        .device_address = SCCB_DEVICE_ADDR_WRITE,
        .scl_speed_hz = SCCB_FREQ_HZ,
        .scl_wait_us = 0,
        .flags.disable_ack_check = false, // enable ack checking
    };

    esp_err_t err = i2c_master_bus_add_device(sccb_bus_handle, &dev_config, &sccb_dev_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Failed to add I2C device: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGD(TAG, "I2C device handle was successfully created");
    return ESP_OK;
}
// setup I2C hardware (configure I2C bus)
esp_err_t sccb_interface_init(void) {
    // check if already initialized so there's no resource leaks
    if (sccb_bus_handle != NULL){
        ESP_LOGW(TAG, "SCCB interface already initialized");
        return ESP_ERR_INVALID_STATE;
    }

    ESP_LOGI(TAG, "Initializing SCCB interface...");

    // configure i2c bus params 
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = SCCB_I2C_PORT,
        .sda_io_num = SCCB_SDA_GPIO, // SDA GPIO
        .scl_io_num = SCCB_SCL_GPIO, // SCL GPIO
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,              // number of consecutive glitches to be ignored by I2C bus
        .intr_priority = 0,
        .flags.enable_internal_pullup = true, // instruct driver to enable SDA and SCL pull-up resistors
    };

    // create I2C master bus handle
    esp_err_t err = i2c_new_master_bus(&i2c_bus_config,&sccb_bus_handle);
    ESP_RETURN_ON_ERROR(err, TAG, "I2C master bus initialization failed");

    ESP_LOGD(TAG, "I2C bus created on port %d", SCCB_I2C_PORT);

    err = sccb_create_device();
    ESP_RETURN_ON_ERROR(err, TAG, "I2C device registration failed");

    ESP_LOGI(TAG, "SCCB interface initialized succcessfully (SCL=%d, SDA=%d, Freq=%d Hz)", SCCB_SCL_GPIO, SCCB_SDA_GPIO, SCCB_FREQ_HZ);
    return ESP_OK;
}

esp_err_t sccb_interface_deinit(void){
    esp_err_t err = ESP_OK;

    // remove device from the bus if it exists
    if (sccb_dev_handle != NULL) {
        err = i2c_master_bus_rm_device(sccb_dev_handle);
        if (err != ESP_OK){
            ESP_LOGE(TAG, "Failed to remove I2C device: %s", esp_err_to_name(err));
        }
        sccb_dev_handle = NULL;
    }

    // delete i2c bus if it exists
    if (sccb_bus_handle != NULL) {
        err = i2c_del_master_bus(sccb_bus_handle);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to delete I2C bus: %s", esp_err_to_name(err));
        }
        sccb_bus_handle = NULL;
    }

    ESP_LOGI(TAG, "SCCB interface deinitialized");
    return err;
}

esp_err_t sccb_write_reg(uint8_t reg_addr, uint8_t value) {
    if (sccb_dev_handle == NULL) {
        ESP_LOGE(TAG, "Device not initialized");
        return ESP_ERR_INVALID_STATE;
    }

    uint8_t write_buf[2] = {reg_addr, value};

    esp_err_t err = i2c_master_transmit(sccb_dev_handle, write_buf, sizeof(write_buf), SCCB_TIMEOUT_MS);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Write failed for register 0x%02X: %s", reg_addr, esp_err_to_name(err));
        return err;
    }

    ESP_LOGD(TAG, "Write: reg=0x%02X, val=0x%02X", reg_addr, value);
    return ESP_OK;
}

esp_err_t sccb_read_reg(uint8_t reg_addr, uint8_t *value) {
    if (sccb_dev_handle == NULL) {
        ESP_LOGE(TAG, "Device isn't initialized");
        return ESP_ERR_INVALID_STATE;
    }

    if (value == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = i2c_master_transmit_receive(sccb_dev_handle, &reg_addr, 1, value, 1, SCCB_TIMEOUT_MS);
    
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Read failed for reg=0x%02X: %s", reg_addr, esp_err_to_name(err));
    }

    ESP_LOGD(TAG, "Read: reg=0x%02X, val=0x%02X", reg_addr, *value);
    return ESP_OK;
}

esp_err_t sccb_write_regs(uint8_t reg_addr, const uint8_t *data, size_t data_len) {
    if (sccb_dev_handle == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    if (data == NULL || data_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    // buffer for register addy and data
    uint8_t *write_buf = malloc(data_len +1);
    if (write_buf == NULL){
        return ESP_ERR_NO_MEM;
    }

    write_buf[0] = reg_addr;
    memcpy(&write_buf[1], data, data_len);

    esp_err_t err = i2c_master_transmit(sccb_dev_handle, write_buf, data_len + 1, SCCB_TIMEOUT_MS);
    free(write_buf);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Multiple byte write failed: %s", esp_err_to_name(err));
    }

    return err;
}

esp_err_t sccb_read_regs(uint8_t reg_addr, uint8_t *data, size_t data_len) {
    if (sccb_dev_handle == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    if (data == NULL || data_len == 0) {
        return ESP_ERR_INVALID_ARG;
    }
    
    esp_err_t err = i2c_master_transmit_receive(sccb_dev_handle, &reg_addr, 1, data, data_len, SCCB_TIMEOUT_MS);
    
    if (err != ESP_OK){
        ESP_LOGE(TAG, "Multiple byte read failed: %s", esp_err_to_name(err));
    }
    return err;
}

// loop from address 0x01 to 0x7F and attempt I2C communication. Log if peripheral sends ACK
void sccb_interface_scan(void) {
    if (sccb_bus_handle == NULL) {
        ESP_LOGE(TAG, "I2C bus isn't initialized. Call sccb_interface_init() first.");
        return;
    }

    ESP_LOGI(TAG, "Scanning I2C bus...");

    uint8_t devices_found = 0;

    for (uint8_t address = 1; address < 127; address++){
        // See if peripheral sends ACK.
        esp_err_t err = i2c_master_probe(sccb_bus_handle, address, 50);
        
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Device found at address: 0x%02X", address);
            devices_found++;
        }
    }

    if (devices_found == 0) {
        ESP_LOGW(TAG, "No devices found on I2C bus");
    } else {
        ESP_LOGI(TAG, "Scan complete: %d devices found", devices_found);
    }
}

esp_err_t sccb_probe_address(uint8_t address){
    if (sccb_bus_handle == NULL) {
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t err = i2c_master_probe(sccb_bus_handle, address, SCCB_TIMEOUT_MS);

    if (err == ESP_OK) {
        ESP_LOGD(TAG, "Device acknowledged at 0x%02X", address);
    } else  if (err == ESP_ERR_TIMEOUT) {
        ESP_LOGD(TAG, "No device at 0x%02X", address);
    } else {
        ESP_LOGW(TAG, "Probe error at 0x%02X", address, esp_err_to_name(err));
    }
    return err;
}
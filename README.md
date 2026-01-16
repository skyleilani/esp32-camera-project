# ESP32 Camera Development Platform

A containerized ESP-IDF development environment for ESP32-WROVER camera firmware.
Building a secure camera system on ESP32 with focus on data integrity verification and a reproducible development environment.

**Status**: **_In Progress_** - Researching, designing components, and testing.

## Hardware

- **MCU**: [ESP32-WROVER-IE](https://documentation.espressif.com/esp32-wrover-e_esp32-wrover-ie_datasheet_en.html) (8MB PSRAM, **Rev v3.0**)
- **Board**: ESP32-DevKitC_V4
- **Camera**: [OV2640 Camera](https://dl.espressif.com/dl/schematics/Camera_OV2640.pdf) (24-pin FPC)
- **Framework**: ESP-IDF v5.5.2

## Architecture

### Docker-based Environment
This project uses Docker to encapsulate the build toolchain and dependencies.
Enforces a consistent environment across host machines and ensures that builds are reproducible.

- **Base Image**: [espressif/idf:v5.5.2](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html)
- **Language Server**: `clangd` accessed via a wrapper script (`scripts/clangd-wrapper.sh`). Necessary because the host IDE is sandboxed (VSCodium via Flatpak on Linux). Running v20 for native Xtensa CPU target support.
- **Host Consistency**: Matching host UID/GID prevents permission issues with mounted volumes
- **Cross-platform support**: Consistent builds across Windows, MacOS, and Linux
- **Build System**: CMake, managed by the ESP-IDF toolchain
- **Debugging**: GDB Stub (`CONFIG_ESP_GDBSTUB_ENABLED=y`)
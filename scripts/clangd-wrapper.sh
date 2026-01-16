#!/bin/bash

# Get absolute path to the project root on HOST
PROJECT_ROOT="$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd)"

LOGS_DIR="${PROJECT_ROOT}/logs"
mkdir -p "${LOGS_DIR}"

LOG_FILE="${LOGS_DIR}/clangd.log"

# Capture session info for the log header
SESSION_START=$(date '+%Y-%m-%d %H:%M:%S %Z')

# Construct the mapping in the format: HostPath=ContainerPath
MAPPING="${PROJECT_ROOT}=/project"

# Write header to log file 
cat > "${LOG_FILE}" << EOF
===========================================
CLANGD SESSION LOG
===========================================
Session Start:          ${SESSION_START}
Project Root:           ${PROJECT_ROOT}
Path Mapping:           ${MAPPING}
===========================================
CLANGD OUTPUT
===========================================

EOF

# Run clangd, redirect stderr to log file
flatpak-spawn --host docker exec -i \
    -w /project \
    esp32-dev-container \
    /usr/bin/clangd \
    --path-mappings="${MAPPING}" \
    --compile-commands-dir=/project/build \
    --query-driver=/opt/esp/tools/**/bin/xtensa-esp32-elf-gcc \
    --header-insertion=never \
    "$@" 2>> "${LOG_FILE}"
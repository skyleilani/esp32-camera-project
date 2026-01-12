#!/bin/bash

# Get absolute path to the project root on HOST
PROJECT_ROOT="$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd)"

LOGS_DIR="${PROJECT_ROOT}/logs/clangd"
mkdir -p "${LOGS_DIR}"

LOG_FILE="${LOGS_DIR}/clangd_$(date +%Y-%m-%d_%H-%M-%S).log"

{
    echo "--- clangd session started at $(date) ---"
    echo "Passed arguments: $@"
    echo "Project Root: ${PROJECT_ROOT}"
} > "${LOG_FILE}"

# Construct the mapping in the format: HostPath=ContainerPath
MAPPING="${PROJECT_ROOT}=/project"
echo "Mapping: ${MAPPING}" >> "${LOG_FILE}"

# Run clangd, redirecting its error output to our log file
flatpak-spawn --host docker exec -i \
    -w /project \
    esp32-dev-container \
    /usr/bin/clangd \
    --path-mappings="${MAPPING}" \
    --compile-commands-dir=/project/build \
    --header-insertion=never \
    "$@" 2> >(tee -a "${LOG_FILE}" >&2)
# executable for vscodium - clangd language server
#!/bin/bash

LOG_FILE="$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd)/logs/clangd-wrapper.log"

echo "--- Wrapper script started at $(date) ---" > "${LOG_FILE}"
echo "Passed arguments: $@" >> "${LOG_FILE}"

# Get absolute path to the project root on HOST
PROJECT_ROOT="$(cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd)"
echo "Project Root: ${PROJECT_ROOT}" >> "${LOG_FILE}"

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
    "$@" 2>> "${LOG_FILE}"
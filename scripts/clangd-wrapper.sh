# executable for vscodium - clangd language server
#!/bin/bash

# Get absolute path to the project root on HOST
PROJECT_ROOT="$(cd "$( dirname "{BASH_SOURCE[0]}" )/.." && pwd)"

# Construct the mapping in the format: HostPath=ContainerPath
MAPPING="${PROJECT_ROOT}=/project"

# Run clangd inside the container with the mapping above
flatpak-spawn --host docker exec -i esp32-dev-container \
    /usr/bin/clangd \
    --path-mappings="${MAPPING}" \
    --compile-commands-dir=/project/build \
    --header-insertion=never
     "$@"

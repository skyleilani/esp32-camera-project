#!/bin/bash
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"
IMAGE_NAME="esp32-camera-env"

# Create logs directory for docker output
LOGS_DIR="${PROJECT_DIR}/logs"
mkdir -p "${LOGS_DIR}"

# Generate log filename with timestamp
LOG_FILE="${LOGS_DIR}/docker.log"

# Capture session info for the log header
SESSION_START=$(date '+%Y-%m-%d %H:%M:%$S %Z')
HOST_OS=$(uname -s)
HOST_KERNEL=$(uname -r)
HOST_ARCH=$(uname -m)
USER_UID=$(id -u)
USER_GID=$(id -g)
GIT_BRANCH=$(cd "$PROJECT_DIR" && git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "N/A")
GIT_COMMIT=$(cd "$PROJECT_DIR" && git rev-parse --short HEAD 2>/dev/null || echo "N/A")

echo "ESP32 Camera Project Development Container"  
echo "Building development container image: $IMAGE_NAME"

docker build \
  --build-arg HOST_USER_UID=$USER_UID \
  --build-arg HOST_USER_GID=$USER_GID \
  -t "$IMAGE_NAME" \
  "$PROJECT_DIR"

echo "Build complete"
echo "Starting development container..."
echo "Project source code is mounted from: $PROJECT_DIR"
echo "Your code will be at: /project"
echo "Docker output is being logged to: $LOG_FILE"

# Display clangd version in the container before starting interactive session
echo ""
echo "Checking clangd version in container..."
docker run --rm "$IMAGE_NAME" clangd --version

echo ""
echo "Starting interactive development session..."

# Write header to log file 
cat > "${LOG_FILE}" << EOF
===================================================
ESP32 DEVELOPMENT SESSION LOG
===================================================
Session Start:          ${SESSION_START}
Container Name:         esp32-dev container
Image Name:             ${IMAGE_NAME}
Project Directory:      ${PROJECT_DIR}
Mount Point:            /project

Host System:
    OS:                 ${HOST_OS}
    Kernel:             ${HOST_KERNEL}
    Architecture:       ${HOST_ARCH}

User Context:
    UID:                ${USER_UID}
    GID:                ${USER_GID}

Git Context:
    Branch:             ${GIT_BRANCH}
    Commit:             ${GIT_COMMIT}

clangd Version:         ${CLANGD_VERSION}

===================================================
TERMINAL SESSION OUTPUT
===================================================

EOF
# Run docker with output captured to log file while still displaying in terminal
# Using script to capture all terminal output including colors and interactive elements
docker run -it --rm \
    --name esp32-dev-container \
    --device=/dev/ttyUSB0 \
    -v "$PROJECT_DIR":/project \
    -w /project \
    "$IMAGE_NAME" 2>&1 | tee -a "${LOG_FILE}"
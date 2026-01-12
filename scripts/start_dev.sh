#!/bin/bash
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"
IMAGE_NAME="esp32-camera-env"

# Create logs directory for docker output
LOGS_DIR="${PROJECT_DIR}/logs/docker"
mkdir -p "${LOGS_DIR}"

# Generate log filename with timestamp
LOG_FILE="${LOGS_DIR}/Docker_$(date +%Y-%m-%d_%H-%M-%S).log"

echo "ESP32 Camera Project Development Container"  
echo "Building development container image: $IMAGE_NAME"

docker build \
  --build-arg HOST_USER_UID=$(id -u) \
  --build-arg HOST_USER_GID=$(id -g) \
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

# Run docker with output captured to log file while still displaying in terminal
# Using script to capture all terminal output including colors and interactive elements
script -q -c "docker run -it --rm \
  --name esp32-dev-container \
  --device=/dev/ttyUSB0 \
  -v \"$PROJECT_DIR\":/project \
  -w /project \
  \"$IMAGE_NAME\"" "${LOG_FILE}"
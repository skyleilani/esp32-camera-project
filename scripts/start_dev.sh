#!/bin/bash
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"
IMAGE_NAME="esp32-camera-env"

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

docker run -it --rm \
  --name esp32-dev-container \
  --device=/dev/ttyUSB0 \
  -v "$PROJECT_DIR":/project \
  -w /project \
  "$IMAGE_NAME"
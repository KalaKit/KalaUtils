#!/bin/bash
# This script builds KalaUtils from source using g++ and CMake with Unix Makefiles in Debug mode on Linux.

# Set the root folder as the location of this script
UTILS_ROOT="$(dirname "$(readlink -f "$0")")"
BUILD_DIR="$UTILS_ROOT/build-debug"
INSTALL_DIR="$UTILS_ROOT/install-debug"

# Record start time
TIME_START=$(date +%T)

# Create the build directory if it doesn't exist
mkdir -p "$BUILD_DIR" || { echo "[ERROR] Failed to create build directory: $BUILD_DIR"; exit 1; }
cd "$BUILD_DIR" || { echo "[ERROR] Failed to access build directory: $BUILD_DIR"; exit 1; }

# Configure KalaUtils with CMake using Unix Makefiles in Debug mode
echo "[INFO] Configuring KalaUtils with CMake in Debug mode..."
cmake -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=gcc \
  -DCMAKE_CXX_COMPILER=g++ \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_C_FLAGS="-g -O0" \
  -DCMAKE_CXX_FLAGS="-g -O0" \
  -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
  -Wno-dev \
  "$UTILS_ROOT" || { echo "[ERROR] CMake configuration failed."; exit 1; }

# Build KalaUtils with make
echo "[INFO] Building KalaUtils in Debug mode..."
make -j"$(nproc)" || { echo "[ERROR] Build process failed."; exit 1; }

# Install KalaUtils
echo "[INFO] Installing KalaUtils in Debug mode..."
make install || { echo "[ERROR] Install process failed."; exit 1; }

# Record end time
TIME_END=$(date +%T)

# Success message
echo "[SUCCESS] KalaUtils built and installed successfully in Debug mode."
echo "---------------------------------------------"
echo "Shared library: $INSTALL_DIR/lib/libKalaUtils.so"
echo "Include headers: $INSTALL_DIR/include"
echo "Build duration: $TIME_START - $TIME_END"
echo "---------------------------------------------"

# Pause to allow to review the output
read -r -p "Press enter to exit..."
exit 0

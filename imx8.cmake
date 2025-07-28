# Toolchain file for Verdin i.MX8M Plus (ARM64)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Direct compiler paths (available in the container)
set(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)

# Set sysroot for target libraries and headers
set(CMAKE_SYSROOT /usr/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)

# Prevent accidental host includes/libs
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set up pkg-config to use cross-target .pc files
set(ENV{PKG_CONFIG_SYSROOT_DIR} /usr/aarch64-linux-gnu)
set(ENV{PKG_CONFIG_PATH} /usr/lib/aarch64-linux-gnu/pkgconfig:/usr/share/pkgconfig)

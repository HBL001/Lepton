# imx8.cmake
cmake_minimum_required(VERSION 3.10)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Use environment variables (set by the container's environment-setup script)
set(CMAKE_C_COMPILER $ENV{CC})
set(CMAKE_CXX_COMPILER $ENV{CXX})
set(CMAKE_FIND_ROOT_PATH $ENV{SDKTARGETSYSROOT})
set(CMAKE_SYSROOT $ENV{SDKTARGETSYSROOT})

# Also inherit pkg-config path
set(ENV{PKG_CONFIG_SYSROOT_DIR} $ENV{PKG_CONFIG_SYSROOT_DIR})
set(ENV{PKG_CONFIG_PATH} $ENV{PKG_CONFIG_PATH})

cmake_minimum_required(VERSION 3.10)
project(thermal_viewer)
set(CMAKE_CXX_STANDARD 17)

# Global include path
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find required libraries using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(GLESv2 REQUIRED glesv2)
pkg_check_modules(EGL REQUIRED egl)

# Include SDL2 and GLES2 headers
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${GLESv2_INCLUDE_DIRS}
    ${EGL_INCLUDE_DIRS}
)

# Add source modules
add_subdirectory(src/spi)
add_subdirectory(src/I2C)
add_subdirectory(src/sensors/lepton)
add_subdirectory(src/sensors/mlx90640)
add_subdirectory(src/viewer)
add_subdirectory(src/tools)

# Combine object libraries
set(MODULE_OBJECTS
    $<TARGET_OBJECTS:spi>
    $<TARGET_OBJECTS:lepton>
    $<TARGET_OBJECTS:mlx90640>
    $<TARGET_OBJECTS:viewer>
)

# Main viewer application
add_executable(thermal_viewer main.cpp ${MODULE_OBJECTS})
target_link_libraries(thermal_viewer
    ${SDL2_LIBRARIES}
    ${GLESv2_LIBRARIES}
    ${EGL_LIBRARIES}
    m
)

# Standalone SPI tool
add_executable(spi_tool src/tools/spi_tools.cpp $<TARGET_OBJECTS:spi>)
target_link_libraries(spi_tool m)

cmake_minimum_required(VERSION 3.10)
project(lepton_viewer)
set(CMAKE_CXX_STANDARD 17)

# Qt5 for GUI
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Global include path
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add all modules
add_subdirectory(src/spi)
add_subdirectory(src/lepton)
add_subdirectory(src/viewer)

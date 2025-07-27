add_library(spi_if spiIf.cpp)
target_include_directories(spi_if PUBLIC ${CMAKE_SOURCE_DIR}/include/spi)

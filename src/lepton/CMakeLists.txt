add_library(lepton_if leptonI2cIf.cpp)
target_include_directories(lepton_if PUBLIC ${CMAKE_SOURCE_DIR}/include/lepton)
target_link_libraries(lepton_if spi_if)

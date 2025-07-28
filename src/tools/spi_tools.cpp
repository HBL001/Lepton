#include <iostream>
#include "thermal/spi.hpp"

int main() {
    std::cout << "[spi_tool] Starting SPI test..." << std::endl;

    // Replace with your actual SPI bus and chip select
    const char* spiDevice = "/dev/spidev0.0";

    // Create SPI interface
    Thermal::SPI spi(spiDevice, 1000000);  // 1 MHz

    if (!spi.open()) {
        std::cerr << "[spi_tool] Failed to open SPI device\n";
        return 1;
    }

    std::cout << "[spi_tool] Sending test byte 0xA5...\n";

    uint8_t tx = 0xA5;
    uint8_t rx = 0x00;

    if (!spi.transfer(&tx, &rx, 1)) {
        std::cerr << "[spi_tool] SPI transfer failed\n";
        return 1;
    }

    std::cout << "[spi_tool] Received byte: 0x" << std::hex << static_cast<int>(rx) << std::endl;

    spi.close();
    return 0;
}

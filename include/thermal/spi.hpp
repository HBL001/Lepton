#pragma once
#include <cstdint>
#include <string>

namespace Thermal {

class SPI {
public:
    SPI(const std::string& device, uint32_t speedHz);
    ~SPI();

    bool open();
    void close();
    bool transfer(const uint8_t* tx, uint8_t* rx, size_t len);

private:
    std::string device_;
    uint32_t speed_;
    int fd_;
};

}

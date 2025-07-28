#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <cstring>
#include <cstdint>

class I2CDevice {
public:
    I2CDevice(const char* devicePath, uint8_t addr)
        : address(addr), fd(-1) {
        fd = open(devicePath, O_RDWR);
        if (fd < 0)
            throw std::runtime_error("Failed to open I2C device");

        if (ioctl(fd, I2C_SLAVE, address) < 0)
            throw std::runtime_error("Failed to set I2C address");
    }

    ~I2CDevice() {
        if (fd >= 0)
            close(fd);
    }

    void readBytes(uint16_t reg, uint8_t* buffer, size_t length) {
        uint8_t regBuf[2] = { static_cast<uint8_t>(reg >> 8), static_cast<uint8_t>(reg & 0xFF) };
        if (write(fd, regBuf, 2) != 2)
            throw std::runtime_error("Failed to write register address");

        if (read(fd, buffer, length) != static_cast<ssize_t>(length))
            throw std::runtime_error("Failed to read I2C data");
    }

    uint16_t readU16(uint16_t reg) {
        uint8_t buf[2] = {};
        readBytes(reg, buf, 2);
        return (static_cast<uint16_t>(buf[1]) << 8) | buf[0];
    }

private:
    int fd;
    uint8_t address;
};

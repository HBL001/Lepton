/**
 * \file lepton_i2c_if.cpp
 * \brief Implementation of I2C communication for the FLIR Lepton thermal sensor (CCI interface).
 *
 * Uses standard Linux I2C device interface to communicate with the FLIR Lepton over the Command Control Interface (CCI).
 * Compatible with Lepton 2.x/3.x/3.5 and the breakout board 250-0577-00.
 * 
 * (C) 2025 Highland Biosciences Ltd.  Dr. Richard Day.
 * 
 * email: richard_day@highlandbiosciences.com
 * 
 */

#include "leptonI2cIf.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cstdio>
#include <cstring>

#define LEPTON_I2C_DEVICE_FMT "/dev/i2c-%d"
#define LEPTON_CCI_ADDRESS    0x2A  ///< FLIR Lepton default CCI I2C address

LeptonI2cIf::LeptonI2cIf(uint8_t bus_number)
    : bus_number(bus_number),  device_file(-1) {}

LeptonI2cIf::~LeptonI2cIf(void) {
    terminate();
}

bool LeptonI2cIf::initialise(void) {
    char device_path[32];
    snprintf(device_path, sizeof(device_path), LEPTON_I2C_DEVICE_FMT, bus_number);

    device_file = open(device_path, O_RDWR);
    if (device_file < 0) {
        fprintf(stderr, "Lepton I2C: Failed to open %s: %s\n", device_path);
        return false;
    }

    if (ioctl(device_file, I2C_SLAVE, LEPTON_CCI_ADDRESS) < 0) {
        fprintf(stderr, "Lepton I2C: Failed to set I2C_SLAVE to 0x%X\n", LEPTON_CCI_ADDRESS);
        close(device_file);
        device_file = -1;
        return false;
    }

    fprintf(stderr, "Lepton I2C: Initialized on %s", device_path);
    return true;
}

void LeptonI2cIf::terminate(void) {
    if (device_file >= 0) {
        close(device_file);
        device_file = -1;
    }
}

void LeptonI2cIf::write_register(uint16_t reg_address, uint16_t data) {
    uint8_t buffer[4];
    buffer[0] = (reg_address >> 8) & 0xFF;  // Register MSB
    buffer[1] = reg_address & 0xFF;         // Register LSB
    buffer[2] = (data >> 8) & 0xFF;
    buffer[3] = data & 0xFF;

    ssize_t result = write(device_file, buffer, sizeof(buffer));
    if (result != sizeof(buffer)) {
         fprintf(stderr,"Lepton I2C: Failed to write to register 0x%04X", reg_address);
    }
}

uint16_t LeptonI2cIf::read_register(uint16_t reg_address) {
    uint8_t addr_buf[2];
    addr_buf[0] = (reg_address >> 8) & 0xFF;
    addr_buf[1] = reg_address & 0xFF;

    if (write(device_file, addr_buf, sizeof(addr_buf)) != sizeof(addr_buf)) {
        fprintf(stderr, "Lepton I2C: Failed to set read address 0x%04X\n", reg_address);
        return 0;
    }

    uint8_t data_buf[2] = {0};
    if (read(device_file, data_buf, sizeof(data_buf)) != sizeof(data_buf)) {
        fprintf(stderr, "Lepton I2C: Failed to read from register 0x%04X\n", reg_address);
        return 0;
    }

    return (data_buf[0] << 8) | data_buf[1];
}

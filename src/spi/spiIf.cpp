/**
 * @file spiIf.cpp
 * @brief SPI interface implementation for communicating with the FLIR Lepton sensor.
 *
 * This implementation is designed for medical device applications and adheres to robust error-checking and modular design principles.
 * The FLIR Lepton uses SPI Mode 3 (CPOL=1, CPHA=1) and supports half-duplex operation.
 *
 * (C) 2025 Highland Biosciences Ltd.  Dr. Richard Day.
 * email: richard_day@highlandbiosciences.com
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "../include/spiIf.hpp"

Spi_interface::Spi_interface(uint8_t bus_number, uint8_t channel_number, uint8_t mode, uint32_t speed, uint8_t bits_per_word)
{
    initialise(bus_number, channel_number, mode, speed, bits_per_word);
}

Spi_interface::Spi_interface(uint8_t bus_number, uint8_t channel_number)
{
    initialise(bus_number, channel_number, default_mode, default_speed, default_bits_per_word);
}

Spi_interface::~Spi_interface(void)
{
    disable();
}

bool Spi_interface::enable(void)
{
    disable(); // Ensure previous descriptor is closed

    device_fd = open(device_name, O_RDWR);
    if (device_fd < 0) return false;

    // Configure SPI device
    if ((ioctl(device_fd, SPI_IOC_WR_MODE, &spi_mode) < 0) ||
        (ioctl(device_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word) < 0) ||
        (ioctl(device_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0))
    {
        disable();
        return false;
    }

    return true;
}

bool Spi_interface::disable(void)
{
    if (device_fd >= 0)
    {
        close(device_fd);
        device_fd = -1;
    }
    return true;
}

bool Spi_interface::write(const uint8_t* tx_buffer, uint32_t tx_length)
{
    if (device_fd < 0 || !tx_buffer || tx_length == 0) return false;

    transfer_structs[0].tx_buf = (unsigned long)tx_buffer;
    transfer_structs[0].rx_buf = 0;
    transfer_structs[0].len = tx_length;

    return (ioctl(device_fd, SPI_IOC_MESSAGE(1), transfer_structs) >= 0);
}

bool Spi_interface::transfer(const uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t tx_rx_length)
{
    if (device_fd < 0 || !tx_buffer || !rx_buffer || tx_rx_length == 0) return false;

    transfer_structs[0].tx_buf = (unsigned long)tx_buffer;
    transfer_structs[0].rx_buf = (unsigned long)rx_buffer;
    transfer_structs[0].len = tx_rx_length;

    return (ioctl(device_fd, SPI_IOC_MESSAGE(1), transfer_structs) >= 0);
}

bool Spi_interface::write_then_read(const uint8_t* tx_buffer, uint32_t tx_length, uint8_t* rx_buffer, uint32_t rx_length)
{
    if (device_fd < 0 || !tx_buffer || tx_length == 0 || !rx_buffer || rx_length == 0) return false;

    transfer_structs[0].tx_buf = (unsigned long)tx_buffer;
    transfer_structs[0].rx_buf = 0;
    transfer_structs[0].len = tx_length;

    transfer_structs[1].tx_buf = 0;
    transfer_structs[1].rx_buf = (unsigned long)rx_buffer;
    transfer_structs[1].len = rx_length;

    return (ioctl(device_fd, SPI_IOC_MESSAGE(2), transfer_structs) >= 0);
}

void Spi_interface::initialise(uint8_t bus_number, uint8_t channel_number, uint8_t mode, uint32_t speed, uint8_t bits_per_word)
{
    spi_mode = mode;
    spi_speed = speed;
    spi_bits_per_word = bits_per_word;

    memset(&transfer_structs, 0, sizeof(transfer_structs));

    for (int i = 0; i < 2; ++i)
    {
        transfer_structs[i].delay_usecs = 0;
        transfer_structs[i].speed_hz = spi_speed;
        transfer_structs[i].bits_per_word = spi_bits_per_word;
    }

    snprintf(device_name, sizeof(device_name), "/dev/spidev%hhu.%hhu", bus_number, channel_number);
    device_fd = -1;
}

/*!
 * @file spi_if.h
 * @brief Linux userspace SPI interface abstraction layer for medical-grade embedded systems.
 *
 * This header defines a C++ class for safe, low-level SPI communication using the standard
 * Linux `spidev` interface. Designed for use in diagnostic and sensing applications, such as
 * interfacing with FLIR Lepton thermal sensors in medical imaging contexts.
 *
 * The interface ensures correct mode, speed, and transfer framing with full-duplex support.
 * Built-in configuration enables compliance with device-specific electrical requirements.
 *
 * SPI Mode:     Mode 3 (CPOL=1, CPHA=1) – required by FLIR Lepton and similar VoSPI sensors
 * Clock Speed:  11 MHz default (Lepton maximum is 20 MHz)
 * Word Length:  8 bits per word (Lepton requires byte-level framing)
 *
 * @note This class is not thread-safe. Access from multiple threads must be synchronized.
 * @note Use only on systems where `/dev/spidevX.Y` nodes are correctly exposed and privileged access is permitted.
 * 
 * (C) 2025 Highland Biosciences Ltd. 
 */

#ifndef SPI_IF_H
#define SPI_IF_H

#include <cstdint>
#include <linux/types.h>
#include <linux/spi/spidev.h>

/**
 * @class Spi_interface
 * @brief Object-oriented abstraction for Linux spidev-based SPI communication.
 */
class Spi_interface
{
public:
    /**
     * @brief Default SPI mode (SPI_MODE_3) required by FLIR Lepton VoSPI protocol.
     */
    static constexpr uint8_t default_mode = SPI_MODE_3;

    /**
     * @brief Default SPI clock speed in Hz. Lepton operates reliably between 8–20 MHz.
     */
    static constexpr uint32_t default_speed = 11000000;

    /**
     * @brief Default bits per SPI word. Lepton requires 8 bits per transfer unit.
     */
    static constexpr uint8_t default_bits_per_word = 8;

    /**
     * @brief Constructs an SPI interface using full custom configuration.
     *
     * @param bus_number       SPI controller index (e.g. 1 for /dev/spidev1.0)
     * @param channel_number   SPI chip select index
     * @param mode             SPI mode (0–3), should be SPI_MODE_3 for Lepton
     * @param speed            SPI bus speed in Hz
     * @param bits_per_word    Bits per SPI word (usually 8)
     */
    Spi_interface(uint8_t bus_number, uint8_t channel_number,
                  uint8_t mode, uint32_t speed, uint8_t bits_per_word);

    /**
     * @brief Constructs an SPI interface using default configuration (Mode 3, 11 MHz, 8 bits).
     *
     * @param bus_number       SPI controller index
     * @param channel_number   SPI chip select index
     */
    Spi_interface(uint8_t bus_number, uint8_t channel_number);

    /**
     * @brief Closes the SPI interface and releases the file descriptor.
     */
    ~Spi_interface(void);

    /**
     * @brief Opens the SPI device and configures the bus with selected parameters.
     *
     * @return True if successful; false on failure.
     */
    bool enable(void);

    /**
     * @brief Disables the SPI interface by closing the file descriptor.
     *
     * @return True if closed successfully; false if already closed or on error.
     */
    bool disable(void);

    /**
     * @brief Performs a unidirectional write to the SPI device.
     *
     * @param tx_buffer Pointer to transmit buffer
     * @param tx_length Number of bytes to write
     * @return True on success; false on transmission error.
     */
    bool write(const uint8_t* tx_buffer, uint32_t tx_length);

    /**
     * @brief Performs a full-duplex transfer (simultaneous write/read) of fixed size.
     *
     * @param tx_buffer Pointer to buffer containing data to transmit
     * @param rx_buffer Pointer to buffer to store received data
     * @param tx_rx_length Number of bytes to transfer in both directions
     * @return True on success; false on SPI error.
     */
    bool transfer(const uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t tx_rx_length);

    /**
     * @brief Performs a combined write followed by a read (half-duplex transaction).
     *
     * @param tx_buffer Pointer to write buffer
     * @param tx_length Number of bytes to write
     * @param rx_buffer Pointer to read buffer
     * @param rx_length Number of bytes to read
     * @return True on success; false if transaction fails.
     */
    bool write_then_read(const uint8_t* tx_buffer, uint32_t tx_length,
                         uint8_t* rx_buffer, uint32_t rx_length);

private:
    /**
     * @brief Internal initializer that sets up device path and stores configuration.
     */
    void initialise(uint8_t bus_number, uint8_t channel_number,
                    uint8_t mode, uint32_t speed, uint8_t bits_per_word);

    char device_name[100];                  ///< Path to SPI device node, e.g., /dev/spidev1.0
    uint8_t spi_mode;                       ///< SPI mode (0–3)
    uint32_t spi_speed;                     ///< SPI clock frequency in Hz
    uint8_t spi_bits_per_word;              ///< Bits per SPI word (typically 8)
    int device_fd;                          ///< File descriptor for open SPI device

    struct spi_ioc_transfer transfer_structs[2]; ///< Used for write_then_read transactions
};

#endif  // SPI_IF_H

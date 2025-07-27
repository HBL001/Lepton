/*!
 * \file lepton_i2c_if.h
 * \brief I2C interface class for the FLIR Lepton (250-0577-00) thermal imaging sensor.
 *
 * This interface provides low-speed command and control access to the Lepton module over I²C.
 * It is intended for use in safety-critical applications such as medical imaging devices.
 * 
 * Typical uses include:
 *  - Initiating Flat-Field Correction (FFC)
 *  - Reading module status and telemetry
 *  - Configuring AGC, telemetry modes, and operating modes
 * 
 * The Lepton uses:
 *  - I²C address: 0x2A
 *  - 16-bit register addresses
 *  - 16-bit register values
 */

#ifndef LEPTONI2CIF_H
#define LEPTONI2CIF_H

#include <cstdint>

class LeptonI2cIf
{
public:
    static constexpr uint8_t LEPTON_I2C_ADDRESS = 0x2A;

    /*!
     * \brief Constructor for the Lepton I2C interface.
     * \param bus_number I2C bus number (e.g. 1 for /dev/i2c-1)
     * \param log Logger reference for trace/debug output
     */
    LeptonI2cIf(uint8_t bus_number);

    /*!
     * \brief Destructor
     */
    ~LeptonI2cIf(void);

    /*!
     * \brief Initialise I2C device and set slave address.
     * \return true if successful
     */
    bool initialise(void);

    /*!
     * \brief Write 16-bit data to a 16-bit register address.
     * \param reg_address 16-bit Lepton register address
     * \param data 16-bit value to write
     */
    void write_register(uint16_t reg_address, uint16_t data);

    /*!
     * \brief Read 16-bit value from a 16-bit register address.
     * \param reg_address 16-bit Lepton register address
     * \return 16-bit register value
     */
    uint16_t read_register(uint16_t reg_address);

    /*!
     * \brief Close the I2C device file if open.
     */
    void terminate(void);

private:
    const uint8_t bus_number;
    static const int invalid_device_file = -1;
    int device_file;
};

#endif // LEPTONI2CIFH

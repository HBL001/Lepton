/*!
 * \file lepton_if.h
 * \brief Abstract interface for communicating with the FLIR Lepton thermal sensor via I2C (CCI).
 *
 * This interface defines basic operations to access the Lepton's internal registers over I2C using the
 * Command Control Interface (CCI). Implementations of this interface must handle 16-bit addressing and 16-bit data.
 *
 * This abstraction supports mocking for unit tests and allows alternate bus implementations if required.
 */

#ifndef LEPTON_IF_H
#define LEPTON_IF_H

#include <cstdint>

/**
 * \class Lepton_interface
 * \brief Abstract base class defining the I2C interface to the FLIR Lepton thermal sensor.
 *
 * This class provides a pure virtual interface to read from and write to the Lepton's 16-bit command registers.
 * The interface is independent of the underlying I2C hardware implementation.
 */
class Lepton_interface
{
public:
    Lepton_interface() = default;
    virtual ~Lepton_interface() = default;

    /**
     * \brief Initialize communication with the Lepton sensor.
     * \return True if initialization succeeded, false otherwise.
     */
    virtual bool initialise() = 0;

    /**
     * \brief Write a 16-bit value to a 16-bit register.
     * \param reg The 16-bit register address (big endian).
     * \param value The 16-bit value to write (big endian).
     */
    virtual void write_register(uint16_t reg, uint16_t value) = 0;

    /**
     * \brief Read a 16-bit value from a 16-bit register.
     * \param reg The 16-bit register address to read from (big endian).
     * \param[out] value The 16-bit output value read from the register.
     * \return True if read was successful, false otherwise.
     */
    virtual bool read_register(uint16_t reg, uint16_t& value) = 0;

    /**
     * \brief Clean up and close the I2C connection.
     */
    virtual void terminate() = 0;
};

#endif // LEPTON_IF_H

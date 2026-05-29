/*
 * adxl345.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Michał Chmielczyk
 */

#include "adxl345.h"
/**
 * @brief Checks if driver struct and its first level childs are not NULL
 *
 * @param driver ADXL345Driver struct to be checked
 *
 * @return ADXL345_OK if not NULL
 * @return ADXL345_ERROR otherwise
 */
static inline ADXL345_Status adxl_check_driver(ADXL345Driver* driver)
{
    return ((driver != NULL) && (driver->device != NULL) && (driver->iface != NULL))
               ? ADXL345_OK
               : ADXL345_ERROR;
}
/**
 * @brief updates device data structure
 * by changing uint8_t DATAX0/DATAX1..DATAZ0/DATAZ1 to uint16_t DATAX..DATAZ
 * according to adxl345 data sheet
 *
 * @param driver pointer to ADXL345Driver structure
 *
 * @return ADXL345_OK (0U) if pointers are not NULL
 * @return ADXL345_ERROR (1U) otherwise
 *
 * @note checks driver for not NULL
 *
 * @pre expects DATAX0/DATAX1..DATAZ0/DATAZ1 to be correct
 */
static inline ADXL345_Status adxl_shift_all_data(ADXL345Driver* driver)
{
    ADXL345_Status returnStatus = ADXL345_OK;

    if ((driver == NULL) || (driver->device == NULL))
    {
        returnStatus = ADXL345_ERROR;
    }

    if (returnStatus == ADXL345_OK)
    {
        driver->device->DATAX = (((uint16_t)driver->device->DATAX1) << 8U) | driver->device->DATAX0;
        driver->device->DATAY = (((uint16_t)driver->device->DATAY1) << 8U) | driver->device->DATAY0;
        driver->device->DATAZ = (((uint16_t)driver->device->DATAZ1) << 8U) | driver->device->DATAZ0;
    }

    return returnStatus;
}
/**
 * @brief Ensures that provided register address is in ADXL345 bounds.
 *
 * Bounds are checked according to "REGISTER MAP" in ADXL345 data sheet
 *
 * @param[in] reg register address to be validated
 *
 * @return ADXL345_OK (0U) if register is valid
 * @return ADXL345_ERROR (1U) otherwise
 */
static inline ADXL345_Status adxl_valid_register(uint8_t reg)
{
    return ((reg == 0U) || ((reg >= ADXL345_THRESH_TAP) && (reg <= ADXL345_FIFO_STATUS)))
               ? ADXL345_OK
               : ADXL345_ERROR;
}
/**
 * @brief Single value write to target ADXL345 device data register via SPI.
 *
 * Validates driver, interface and register address before
 * data transfer via SPI.
 *
 * @param[in] driver pointer to ADXL345Driver instance.
 * @param[in] reg register address.
 * @param[in] value value to be written.
 *
 * @return ADXL345_OK on success.
 * @return ADXL345_ERROR on invalid input or SPI failure.
 *
 * @note Function handle SPI CS line using interface callbacks.
 */
static ADXL345_Status adxl_write(ADXL345Driver* driver, uint8_t reg, uint8_t value)
{
    ADXL345_Status returnStatus = ADXL345_OK;

    if (adxl_check_driver(driver) != ADXL345_OK)
    {
        returnStatus = ADXL345_ERROR;
    }

    if (returnStatus == ADXL345_OK)
    {
        if (driver->iface->cs_high == NULL || (driver->iface->cs_low) == NULL ||
            (driver->iface->spi_tx) == NULL)
        {
            returnStatus = ADXL345_ERROR;
        }
    }

    if (returnStatus == ADXL345_OK)
    {
        if (adxl_valid_register(reg) != ADXL345_OK)
        {
            returnStatus = ADXL345_ERROR;
        }
    }

    if (returnStatus == ADXL345_OK)
    {
        uint8_t tx[2U] = {reg, value};
        driver->iface->cs_low(driver->device);
        returnStatus = driver->iface->spi_tx(tx, 2U);
        driver->iface->cs_high(driver->device);
    }

    return returnStatus;
}
/**
 * @brief Single value read to target ADXL345 device data register via SPI.
 *
 * Validates driver, interface and register address before
 * data transfer and receive via SPI.
 *
 * @param[in] driver pointer to ADXL345Driver instance.
 * @param[in] reg register address.
 * @param[in] dest pointer to read value destination
 *
 * @return ADXL345_OK on success.
 * @return ADXL345_ERROR on invalid input or SPI failure.
 *
 * @note Function handle SPI CS line using interface callbacks.
 */
static ADXL345_Status adxl_read(ADXL345Driver* driver, uint8_t reg, uint8_t* dest)
{

    ADXL345_Status returnStatus = ADXL345_OK;

    if ((adxl_check_driver(driver) != ADXL345_OK) || (dest == NULL))
    {
        returnStatus = ADXL345_ERROR;
    }

    if (returnStatus == ADXL345_OK)
    {
        if (driver->iface->cs_high == NULL || (driver->iface->cs_low) == NULL ||
            (driver->iface->spi_txrx) == NULL)
        {
            returnStatus = ADXL345_ERROR;
        }
    }

    if (returnStatus == ADXL345_OK)
    {
        if (adxl_valid_register(reg) != ADXL345_OK)
        {
            returnStatus = ADXL345_ERROR;
        }
    }

    if (returnStatus == ADXL345_OK)
    {
        uint8_t txrx[2U] = {ADXL345_SINGLE_BYTE_READ | reg, 0x00};

        driver->iface->cs_low(driver->device);
        returnStatus = driver->iface->spi_txrx(txrx, txrx, 2U);
        driver->iface->cs_high(driver->device);

        *dest = txrx[1U];
    }

    return returnStatus;
}
/**
 * @brief Multiple value read to target ADXL345 device data register via SPI.
 *
 * Validates driver, interface and register address before
 * data transfer and receive via SPI. Performs 6 data reads.
 *
 * @pre Pointer dest must point to uint16_t[3] table.
 *
 * @param[in] driver pointer to ADXL345Driver instance.
 * @param[in] dest pointer to uint16_t[3] table.
 *
 * @return ADXL345_OK on success.
 * @return ADXL345_ERROR on invalid input or SPI failure.
 *
 * @note Function handle SPI CS line using interface callbacks.
 */
static ADXL345_Status adxl_multi_read(ADXL345Driver* driver, uint16_t* dest)
{

    ADXL345_Status returnStatus = ADXL345_OK;

    if (adxl_check_driver(driver) != ADXL345_OK || (dest == NULL))
    {
        returnStatus = ADXL345_ERROR;
    }

    if (returnStatus == ADXL345_OK)
    {

        if (driver->iface->cs_high == NULL || (driver->iface->cs_low) == NULL ||
            (driver->iface->spi_txrx) == NULL)
        {
            returnStatus = ADXL345_ERROR;
        }
    }

    uint8_t txrx[7U] = {ADXL345_MULTI_BYTE_READ | ADXL345_DATAX0, 0U, 0U, 0U, 0U, 0U, 0U};

    if (returnStatus == ADXL345_OK)
    {
        driver->iface->cs_low(driver->device);
        returnStatus = driver->iface->spi_txrx(txrx, txrx, 7U);
        driver->iface->cs_high(driver->device);
    }

    if (returnStatus == ADXL345_OK)
    {
        dest[0U] = (((uint16_t)txrx[2U]) << 8U) | txrx[1U];
        dest[1U] = (((uint16_t)txrx[4U]) << 8U) | txrx[3U];
        dest[2U] = (((uint16_t)txrx[6U]) << 8U) | txrx[5U];
    }
    return returnStatus;
}
/**
 * @brief Reads data from ADXL345 Device specified by driver pointer
 * and stores it in driver->device structure
 *
 * @param[in] driver pointer to target device structure
 *
 * @return ADXL345_OK on success.
 * @return ADXL345_ERROR on invalid input or read failure.
 *
 * @note Validates driver and interface
 */
ADXL345_Status ADXL_MultiReadDevice(ADXL345Driver* driver)
{

    ADXL345_Status returnStatus = ADXL345_OK;

    if (driver == NULL)
    {
        returnStatus = ADXL345_ERROR;
    }

    uint16_t axes[3];

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_multi_read(driver, axes);
    }

    if (returnStatus == ADXL345_OK)
    {
        driver->device->DATAX = axes[0];
        driver->device->DATAY = axes[1];
        driver->device->DATAZ = axes[2];
    }
    return returnStatus;
}
/**
 * @brief Checks if connection with ADXL345 is correct
 *
 * Proper read from ADXL345_DEVID register of device
 * outcomes in same determinant 0xE5 value
 *
 * @param[in] driver ADXL345Driver data structure holding pointers to
 * target device.
 *
 * @return ADXL345_OK on proper device connection
 * @return ADXL345_ERROR on invalid pointers to device
 * or wrong read value
 */
ADXL345_Status ADXL_CheckDevice(ADXL345Driver* driver)
{
    ADXL345_Status returnStatus = adxl_check_driver(driver);

    uint8_t txrx;

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_read(driver, ADXL345_DEVID, &txrx);
    }

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = (txrx == 0xE5) ? ADXL345_OK : ADXL345_ERROR;
    }

    return returnStatus;
}
/**
 * @brief Read data from device in two read per axis mode.
 *
 * After read data is being stored in target data structure.
 *
 * @param[in] driver ADXL345Driver data structure holding pointers to
 * target device.
 *
 * @return ADXL345_OK on successful read and write to structure
 * @return ADXL345_ERROR on any problem
 */
ADXL345_Status ADXL_ReadDevice(ADXL345Driver* driver)
{
    ADXL345_Status returnStatus = adxl_check_driver(driver);

    if (returnStatus == ADXL345_OK)
    {
        uint8_t* ptrDataTab[6U] = {
            &driver->device->DATAX0,
            &driver->device->DATAX1,
            &driver->device->DATAY0,
            &driver->device->DATAY1,
            &driver->device->DATAZ0,
            &driver->device->DATAZ1,
        };

        for (uint8_t i = 0U; i < 6U; i++)
        {
            returnStatus = adxl_read(driver, (ADXL345_DATAX0 + i), ptrDataTab[i]);

            if (returnStatus != ADXL345_OK)
                break;
        }
    }

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_shift_all_data(driver);
    }

    return returnStatus;
}
/**
 * @brief Writes name, DATAX, DATAY, DATAZ to dest string
 *
 * @param[in] driver ADXL345Driver data structure holding pointers to
 * target device.
 * @param[in] dest pointer to write to buffer
 * @param[in] size size of destination buffer
 *
 * @return ADXL345_OK when pointers are valid and write
 * to dest success
 * @return ADXL345_ERROR otherwise
 *
 * @note Ensures proper driver pointers and write to dest pointer
 */
ADXL345_Status ADXL_DeviceDump(ADXL345Driver* driver, char* dest, uint8_t size)
{
    ADXL345_Status returnStatus = ADXL345_OK;

    if ((adxl_check_driver(driver) != ADXL345_OK) || (dest == NULL))
    {
        returnStatus = ADXL345_ERROR;
    }

    if (returnStatus == ADXL345_OK)
    {
        int DumpStatus = snprintf(dest,
                                  size,
                                  "%s: X: %d, Y: %d, Z: %d\r\n",
                                  driver->device->name,
                                  driver->device->DATAX,
                                  driver->device->DATAY,
                                  driver->device->DATAZ);

        returnStatus = (DumpStatus > -1 && DumpStatus < size) ? ADXL345_OK : ADXL345_ERROR;
    }

    return returnStatus;
}
/**
 * @brief Sets or zeroes measurement bit in ADXL345_POWER_CTL register
 * of target device
 *
 * When measure bit is zeroed device goes into standby mode with
 * minimal power consumption
 *
 * @param[in] driver ADXL345Driver data structure holding pointers to
 * target device.
 * @param[in] mode 0U for standby mode, uint8_t>0 for measurement mode
 *
 * @return ADXL345_OK on set success
 * @return ADXL345_ERROR otherwise
 *
 * @note function checks for not NULL driver pointers
 */
ADXL345_Status ADXL_SetMeasure(ADXL345Driver* driver, uint8_t mode)
{
    ADXL345_Status returnStatus = adxl_check_driver(driver);

    uint8_t txrx;

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_read(driver, ADXL345_POWER_CTL, &txrx);
    }

    if (returnStatus == ADXL345_OK)
    {
        txrx &= ~ADXL345_POWER_CTL_measure_bit;

        if (mode > 0)
        {
            txrx |= ADXL345_POWER_CTL_measure_bit;
        }

        returnStatus = adxl_write(driver, ADXL345_POWER_CTL, txrx);
    }

    return returnStatus;
}
/**
 * @brief Sets desired range of device data collection resolution
 *
 * @param[in] driver ADXL345Driver data structure holding pointers to
 * target device.
 * @param[in] range flag one of: RANGE_2G, RANGE_4G, RANGE_8G, RANGE_16G
 *
 * @return ADXL345_OK on success
 * @return ADXL345_ERROR on NULL pointer to driver or r/w error
 *
 * @note function checks for not NULL driver pointers
 */
ADXL345_Status ADXL_SetRange(ADXL345Driver* driver, uint8_t range)
{
    ADXL345_Status returnStatus = ADXL345_ERROR;

    if ((adxl_check_driver(driver) == ADXL345_OK) && (range <= RANGE_16G))
    {
        returnStatus = ADXL345_OK;
    }

    uint8_t txrx;

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_read(driver, ADXL345_DATA_FORMAT, &txrx);
    }

    if (returnStatus == ADXL345_OK)
    {
        txrx = (txrx & ~ADXL345_DATA_FORMAT_range_bits) | range;
        returnStatus = adxl_write(driver, ADXL345_DATA_FORMAT, txrx);
    }

    return returnStatus;
}
/**
 * @brief Sets FULL_RES bit in ADXL345_DATA_FORMAT register
 *
 * @param[in] driver ADXL345Driver data structure holding pointers to
 * target device.
 *
 * @return ADXL345_OK on success
 * @return ADXL345_ERROR on NULL pointer to driver or r/w error
 *
 * @note function checks for not NULL driver pointers
 */
ADXL345_Status ADXL_SetFullResolution(ADXL345Driver* driver)
{
    ADXL345_Status returnStatus = adxl_check_driver(driver);

    uint8_t txrx;

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_read(driver, ADXL345_DATA_FORMAT, &txrx);
    }

    if (returnStatus == ADXL345_OK)
    {
        txrx |= ADXL345_DATA_FORMAT_full_res_bit;
        returnStatus = adxl_write(driver, ADXL345_DATA_FORMAT, txrx);
    }

    return returnStatus;
}
/**
 * @brief Sets or zeroes justify bit in ADXL345_DATA_FORMAT register
 * of target device
 *
 * A setting of 1U in the justify bit selects left-justified (MSB) mode, and
 * a setting of 0U selects right-justified mode with sign extension.
 *
 * @param[in] driver ADXL345Driver data structure holding pointers to
 * target device.
 * @param[in] mode 0U for LSB mode, uint8_t>0 for MSB mode
 *
 * @return ADXL345_OK on set success
 * @return ADXL345_ERROR otherwise
 *
 * @note function checks for not NULL driver pointers
 */
ADXL345_Status ADXL_SetJustify(ADXL345Driver* driver, uint8_t mode)
{
    ADXL345_Status returnStatus = adxl_check_driver(driver);

    uint8_t txrx;

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_read(driver, ADXL345_DATA_FORMAT, &txrx);
    }

    if (returnStatus == ADXL345_OK)
    {
        txrx &= ~ADXL345_DATA_FORMAT_justify_bit;

        if (mode > 0)
        {
            txrx |= ADXL345_DATA_FORMAT_justify_bit;
        }
        returnStatus = adxl_write(driver, ADXL345_DATA_FORMAT, txrx);
    }
    return returnStatus;
}
/**
 * @brief Sets desired offset to X,Y,Z device axis.
 *
 * @param[in] offX offset value for X axis.
 * @param[in] offY offset value for Y axis.
 * @param[in] offZ offset value for Z axis.
 *
 * @return ADXL345_OK on set success.
 * @return ADXL345_ERROR otherwise.
 *
 * @note on failure axis may stay partially changed.
 */
ADXL345_Status ADXL_SetOffset(ADXL345Driver* driver, uint8_t offX, uint8_t offY, uint8_t offZ)
{
    ADXL345_Status returnStatus = adxl_check_driver(driver);

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_write(driver, ADXL345_OFSX, offX);
    }

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_write(driver, ADXL345_OFSY, offY);
    }

    if (returnStatus == ADXL345_OK)
    {
        returnStatus = adxl_write(driver, ADXL345_OFSZ, offZ);
    }
    return returnStatus;
}

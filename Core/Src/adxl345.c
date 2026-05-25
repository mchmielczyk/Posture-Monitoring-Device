/*
 * adxl345.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Michał Chmielczyk
 */

#include "adxl345.h"

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
    ADXL345_Status result = ADXL345_ERROR;

    if ((reg == 0U) || ((reg >= ADXL345_THRESH_TAP) && (reg <= ADXL345_FIFO_STATUS)))
    {
        result = ADXL345_OK;
    }

    return result;
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

    if (driver == NULL || (driver->device) == NULL || (driver->iface) == NULL)
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

    if (driver == NULL || (driver->device) == NULL || (driver->iface) == NULL || (dest == NULL))
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
 * @param[in] reg register address.
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

    if (driver == NULL || (driver->device) == NULL || (driver->iface) == NULL || (dest == NULL))
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
uint8_t ADXL_CheckDevice(ADXL345Driver* driver)
{
    uint8_t txrx;
    adxl_read(Device, Env, ADXL345_DEVID, &txrx);
    if (txrx == 0xE5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
ADXL345_Status ADXL_ReadDevice(ADXL345Driver* driver)
{
    ADXL345_Status returnStatus;
    //
    if (!Device || !Env)
        return ADXL345_ERROR;
    //
    uint8_t* ptrDataTab[6] = {
        &Device->DATAX0,
        &Device->DATAX1,
        &Device->DATAY0,
        &Device->DATAY1,
        &Device->DATAZ0,
        &Device->DATAZ1,
    };
    //
    for (int i = 0; i < 6; i++)
    {
        returnStatus = adxl_read(Device, Env, (ADXL345_DATAX0 + i), ptrDataTab[i]);
        if (returnStatus != ADXL345_OK)
            return returnStatus;
    }
    //
    Device->DATAX = (((uint16_t)Device->DATAX1) << 8) | Device->DATAX0;
    Device->DATAY = (((uint16_t)Device->DATAY1) << 8) | Device->DATAY0;
    Device->DATAZ = (((uint16_t)Device->DATAZ1) << 8) | Device->DATAZ0;
    //
    return ADXL345_OK;
}
ADXL345_Status ADXL_DeviceDump(ADXL345Driver* driver, char* Dest, uint8_t Size)
{
    if (!Device || !Dest)
        return ADXL345_ERROR;
    int DumpStatus = snprintf(Dest,
                              Size,
                              "%s: X: %d, Y: %d, Z: %d\r\n",
                              Device->name,
                              Device->DATAX,
                              Device->DATAY,
                              Device->DATAZ);
    if (DumpStatus > -1 && DumpStatus < Size)
    {
        return ADXL345_OK;
    }
    return ADXL345_ERROR;
}
ADXL345_Status ADXL_SetMeasure(ADXL345Driver* driver, uint8_t mode) //
{
    ADXL345_Status returnStatus;
    if (!Device || !Env)
        return ADXL345_ERROR;
    uint8_t rx;
    returnStatus = adxl_read(Device, Env, ADXL345_POWER_CTL, &rx);
    if (returnStatus != ADXL345_OK)
        return returnStatus;
    uint8_t tx = rx & 0xF7;
    if (mode)
    {
        tx |= (0x01 << 3);
    }
    returnStatus = adxl_write(Device, Env, ADXL345_POWER_CTL, tx);
    return returnStatus;
}
ADXL345_Status ADXL_SetRange(ADXL345Driver* driver, uint8_t Range) //
{
    ADXL345_Status returnStatus;
    if ((!Device || !Env) || (Range < 0 || Range > 3))
        return ADXL345_ERROR;
    uint8_t rx;
    returnStatus = adxl_read(Device, Env, ADXL345_DATA_FORMAT, &rx);
    if (returnStatus != ADXL345_OK)
        return returnStatus;
    uint8_t tx = 0;
    switch (Range)
    {
    case RANGE_2G:
        tx = (rx & 0xFC);
        returnStatus = adxl_write(Device, Env, ADXL345_DATA_FORMAT, tx);
        return returnStatus;
    case RANGE_4G:
        tx = (rx & 0xFC) | 0x01;
        returnStatus = adxl_write(Device, Env, ADXL345_DATA_FORMAT, tx);
        return returnStatus;
    case RANGE_8G:
        tx = (rx & 0xFC) | 0x02;
        returnStatus = adxl_write(Device, Env, ADXL345_DATA_FORMAT, tx);
        return returnStatus;
    case RANGE_16G:
        tx = (rx & 0xFC) | 0x03;
        returnStatus = adxl_write(Device, Env, ADXL345_DATA_FORMAT, tx);
        return returnStatus;
    default:
        return ADXL345_ERROR;
    }
}
ADXL345_Status ADXL_SetFullResolution(ADXL345Driver* driver)
{
    ADXL345_Status returnStatus;
    if (!Device || !Env)
        return ADXL345_ERROR;
    uint8_t rx;
    returnStatus = adxl_read(Device, Env, ADXL345_DATA_FORMAT, &rx);
    if (returnStatus != ADXL345_OK)
        return returnStatus;
    uint8_t tx = rx | (0x01 << 3);
    returnStatus = adxl_write(Device, Env, ADXL345_DATA_FORMAT, tx);
    return returnStatus;
}
ADXL345_Status ADXL_SetJustify(ADXL345Driver* driver, uint8_t mode)
{
    ADXL345_Status returnStatus;
    if (!Device || !Env || (mode < 0))
        return ADXL345_ERROR;
    uint8_t txrx;
    returnStatus = adxl_read(Device, Env, ADXL345_DATA_FORMAT, &txrx);
    if (returnStatus != ADXL345_OK)
        return returnStatus;
    txrx &= (0xFB);
    if (mode)
    {
        txrx |= (0x01 << 2);
    }
    returnStatus = adxl_write(Device, Env, ADXL345_DATA_FORMAT, txrx);
    return returnStatus;
}
ADXL345_Status ADXL_SetOffset(ADXL345Driver* driver, uint8_t offX, uint8_t offY,
                              uint8_t offZ) //
{
    ADXL345_Status returnStatus;
    if (!Device || !Env)
        return ADXL345_ERROR;
    returnStatus = adxl_write(Device, Env, ADXL345_OFSX, offX);
    if (returnStatus != ADXL345_OK)
        return returnStatus;
    returnStatus = adxl_write(Device, Env, ADXL345_OFSY, offY);
    if (returnStatus != ADXL345_OK)
        return returnStatus;
    returnStatus = adxl_write(Device, Env, ADXL345_OFSZ, offZ);
    return returnStatus;
}

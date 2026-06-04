/*
 * adxl345.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Michał Chmielczyk
 */

#include "adxl345.h"
#include "gpio.h"
#include "spi.h"

static volatile uint8_t spiStatus = 0;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    if (hspi == &hspi2)
    {
        spiStatus = 0;
    }
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi)
{
    if (hspi == &hspi2)
    {
        spiStatus = 0;
    }
}
static ADXL345_Status STM32_SPI_Transmit_fn(const ADXL345Driver* driver, uint8_t* tx, uint16_t size)
{
    spiStatus = 1;

    HAL_GPIO_WritePin(driver->device->PORT, driver->device->PIN, RESET);

    HAL_StatusTypeDef halStatus = HAL_SPI_Transmit_DMA(&hspi2, tx, size);

    if (halStatus == HAL_OK)
    {
        uint32_t start = HAL_GetTick();

        while (spiStatus && (HAL_GetTick() - start) < 100)
            ;

        if (spiStatus)
        {
            halStatus = HAL_TIMEOUT;
        }
    }

    HAL_GPIO_WritePin(driver->device->PORT, driver->device->PIN, SET);

    switch (halStatus)
    {
    case HAL_OK:
        return ADXL345_OK;
    case HAL_BUSY:
        return ADXL345_BUSY;
    case HAL_TIMEOUT:
        return ADXL345_TIMEOUT;
    default:
        return ADXL345_ERROR;
    }
}
static ADXL345_Status
STM32_SPI_TransmitReceive_fn(const ADXL345Driver* driver, uint8_t* tx, uint8_t* rx, uint16_t size)
{
    spiStatus = 1;

    HAL_GPIO_WritePin(driver->device->PORT, driver->device->PIN, RESET);

    HAL_StatusTypeDef halStatus = HAL_SPI_TransmitReceive_DMA(&hspi2, tx, rx, size);

    if (halStatus == HAL_OK)
    {
        uint32_t start = HAL_GetTick();

        while (spiStatus && (HAL_GetTick() - start) < 100)
            ;

        if (spiStatus)
        {
            halStatus = HAL_TIMEOUT;
        }
    }

    HAL_GPIO_WritePin(driver->device->PORT, driver->device->PIN, SET);

    switch (halStatus)
    {
    case HAL_OK:
        return ADXL345_OK;
    case HAL_BUSY:
        return ADXL345_BUSY;
    case HAL_TIMEOUT:
        return ADXL345_TIMEOUT;
    default:
        return ADXL345_ERROR;
    }
}
static ADXL345Interface STM32_ENV = {.write = STM32_SPI_Transmit_fn,
                                     .read = STM32_SPI_TransmitReceive_fn};

ADXL345Interface* GetSTM32Interface(void)
{
    return &STM32_ENV;
}

/*
 * adxl345.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Michał Chmielczyk
 */

//sprawdzenie poprawnego devid
//odczyt z akcelerometrow
//zapis do akcelerometrow
//handlowanie braku dostepu do urzadzenia
//return statusu zadania
//rtos safety
#include "adxl345.h"
#include "spi.h"
#include "gpio.h"


volatile uint8_t spiStatus=0;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi2)
	{
		spiStatus=0;
	}
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi == &hspi2)
    {
        spiStatus = 0;
    }
}
void STM32_GPIO_WritePin_fn_low(ADXL345Data *Device)
{
	HAL_GPIO_WritePin(Device->PORT, Device->PIN, RESET);
}
void STM32_GPIO_WritePin_fn_high(ADXL345Data *Device)
{
	HAL_GPIO_WritePin(Device->PORT, Device->PIN, SET);
}
void STM32_SPI_Transmit_fn(uint8_t* tx,uint16_t size)
{
	spiStatus=1;
	HAL_SPI_Transmit_DMA(&hspi2, tx, size);
	while(spiStatus);
}
void STM32_SPI_TransmitReceive_fn(uint8_t* tx,uint8_t*rx,uint16_t size)
{
	spiStatus=1;
	HAL_SPI_TransmitReceive_DMA(&hspi2, tx, rx, size);
	while(spiStatus);
}

ADXL345_Interface STM32_ENV={
		.cs_high=STM32_GPIO_WritePin_fn_high,
		.cs_low=STM32_GPIO_WritePin_fn_low,
		.spi_tx=STM32_SPI_Transmit_fn,
		.spi_txrx=STM32_SPI_TransmitReceive_fn
};

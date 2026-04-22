#include "fake_hal.h"

FAKE_SPI_Handle hspi2;

void FAKE_GPIO_WritePin_fn_low(ADXL345Data *Device)
{
	FAKE_HAL_GPIO_WritePin(Device->PORT, Device->PIN, ADXL345_RESET);
}
void FAKE_GPIO_WritePin_fn_high(ADXL345Data *Device)
{
	FAKE_HAL_GPIO_WritePin(Device->PORT, Device->PIN, ADXL345_SET);
}
void FAKE_SPI_Transmit_fn(uint8_t* tx,uint16_t size)
{
	FAKE_HAL_SPI_Transmit_DMA(&hspi2, tx, size);
}
void FAKE_SPI_TransmitReceive_fn(uint8_t* tx,uint8_t*rx,uint16_t size)
{
	FAKE_HAL_SPI_TransmitReceive_DMA(&hspi2, tx, rx, size);
}

ADXL345_Interface FAKE_ENV={
		.cs_high=FAKE_GPIO_WritePin_fn_high,
		.cs_low=FAKE_GPIO_WritePin_fn_low,
		.spi_tx=FAKE_SPI_Transmit_fn,
		.spi_txrx=FAKE_SPI_TransmitReceive_fn
};

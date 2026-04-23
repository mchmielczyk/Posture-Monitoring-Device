#include "fake_hal.h"


void FAKE_GPIO_WritePin_fn_low(ADXL345Data *Device)
{
	// FAKE_HAL_GPIO_WritePin(Device->PORT, Device->PIN, ADXL345_RESET);
}
void FAKE_GPIO_WritePin_fn_high(ADXL345Data *Device)
{
	// FAKE_HAL_GPIO_WritePin(Device->PORT, Device->PIN, ADXL345_SET);
}
void FAKE_SPI_Transmit_fn(uint8_t *tx, uint16_t size)
{
	//FAKE_HAL_SPI_Transmit_DMA(&hspi2, tx, size);
}
uint8_t FAKE_SPI_TransmitReceive_fn(uint8_t *tx, uint8_t *rx, uint16_t size)
{
	if (tx[0] & (0x01) << 7) // single bit read
	{
		uint8_t targetRegister = tx[0] &= ~((0x01) << 7);
		switch (targetRegister)
		{
		case ADXL345_DEVID:
			rx[1] = 0xE5;
			break;
		case ADXL345_DATAX0:
			rx[1] = 0x02;
			break;
		case ADXL345_DATAX1:
			rx[1] = 0x04;
			break;
		case ADXL345_DATAY0:
			rx[1] = 0x06;
			break;
		case ADXL345_DATAY1:
			rx[1] = 0x08;
			break;
		case ADXL345_DATAZ0:
			rx[1] = 0x0A;
			break;
		case ADXL345_DATAZ1:
			rx[1] = 0x0C;
			break;
		default:
			rx[1] = 0x00;
			break;
		}
	}
	else
	{
	}

	return rx[1];
}

ADXL345_Interface FAKE_ENV = {
	.cs_high = FAKE_GPIO_WritePin_fn_high,
	.cs_low = FAKE_GPIO_WritePin_fn_low,
	.spi_tx = FAKE_SPI_Transmit_fn,
	.spi_txrx = FAKE_SPI_TransmitReceive_fn};

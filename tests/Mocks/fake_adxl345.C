#include "fake_hal.h"


typedef enum
{
	FAKE_ADXL345_OK       = 0x00,
	FAKE_ADXL345_ERROR    = 0x01,
	FAKE_ADXL345_BUSY     = 0x02,
	FAKE_ADXL345_TIMEOUT  = 0x03
} FAKE_ADXL345_Status;

void FAKE_GPIO_WritePin_fn_low(ADXL345Data *Device)
{
	// FAKE_HAL_GPIO_WritePin(Device->PORT, Device->PIN, ADXL345_RESET);
}
void FAKE_GPIO_WritePin_fn_high(ADXL345Data *Device) 
{
	// FAKE_HAL_GPIO_WritePin(Device->PORT, Device->PIN, ADXL345_SET);
}
FAKE_ADXL345_Status FAKE_SPI_Transmit_fn(uint8_t *tx, uint16_t size)
{
	//FAKE_HAL_SPI_Transmit_DMA(&hspi2, tx, size);
}
FAKE_ADXL345_Status FAKE_SPI_TransmitReceive_fn(uint8_t *tx, uint8_t *rx, uint16_t size)
{
	
	if (tx[0]==ADXL345_MULTI_BYTE_READ|ADXL345_DATAX0) // multi bit read
	{
			rx[1]=0x02;
			rx[2]=0x04;
			rx[3]=0x06;
			rx[4]=0x08;
			rx[5]=0x0A;
			rx[6]=0x0C;
			return FAKE_ADXL345_OK;
		}
	if (tx[0] & (0x01) << 7) // single bit read
	{
		uint8_t targetRegister = tx[0] &= ~((0x01) << 7);
		switch (targetRegister)
		{
		case ADXL345_DEVID:
			rx[1] = 0xE5;
			return FAKE_ADXL345_OK;
		case ADXL345_DATAX0:
			rx[1] = 0x02;
			return FAKE_ADXL345_OK;
		case ADXL345_DATAX1:
			rx[1] = 0x04;
			return FAKE_ADXL345_OK;
		case ADXL345_DATAY0:
			rx[1] = 0x06;
			return FAKE_ADXL345_OK;
		case ADXL345_DATAY1:
			rx[1] = 0x08;
			return FAKE_ADXL345_OK;
		case ADXL345_DATAZ0:
			rx[1] = 0x0A;
			return FAKE_ADXL345_OK;
		case ADXL345_DATAZ1:
			rx[1] = 0x0C;
			return FAKE_ADXL345_OK;
		default:
			rx[1] = 0x00;
			return FAKE_ADXL345_ERROR;
		}
	}
	return FAKE_ADXL345_ERROR;
}

ADXL345_Interface FAKE_ENV = {
	.cs_high = FAKE_GPIO_WritePin_fn_high,
	.cs_low = FAKE_GPIO_WritePin_fn_low,
	.spi_tx = FAKE_SPI_Transmit_fn,
	.spi_txrx = FAKE_SPI_TransmitReceive_fn
};
	

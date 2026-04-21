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
//#include "adxl345.h"

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

uint8_t txGlobal[2];
uint8_t txrxGlobal[2];
void static adxl_write(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t reg,uint8_t value)
{
	txGlobal[0] = reg;
	txGlobal[1] = value;
	spiStatus=1;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, txGlobal, 2);
	while(spiStatus);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}
uint8_t static adxl_read(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,uint8_t reg)
{
	txrxGlobal[0] = ADXL345_SINGLE_BYTE_READ|reg;
	txrxGlobal[1] = 0x00;
	spiStatus=1;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_DMA(&hspi2, txrxGlobal,txrxGlobal, 2);
	while(spiStatus);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	return txrxGlobal[1];
}
uint8_t txrxMultiGlobal[7];
void static adxl_multi_read(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint16_t *dest)
{
	txrxMultiGlobal[0]=ADXL345_MULTI_BYTE_READ|ADXL345_DATAX0;
	txrxMultiGlobal[1]=0;
	txrxMultiGlobal[2]=0;
	txrxMultiGlobal[3]=0;
	txrxMultiGlobal[4]=0;
	txrxMultiGlobal[5]=0;
	txrxMultiGlobal[6]=0;
	spiStatus=1;
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_DMA(&hspi2, txrxMultiGlobal,txrxMultiGlobal, 7);
	while(spiStatus);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
	dest[0]=(((uint16_t)txrxMultiGlobal[2])<<8)|txrxMultiGlobal[1];
	dest[1]=(((uint16_t)txrxMultiGlobal[4])<<8)|txrxMultiGlobal[3];
	dest[2]=(((uint16_t)txrxMultiGlobal[6])<<8)|txrxMultiGlobal[5];
}
void ADXL_MultiReadDevice(ADXL345Data *Device)
{
	uint16_t axes[3];
	adxl_multi_read(Device->PORT, Device->PIN,axes);

	Device->DATAX=axes[0];
	Device->DATAY=axes[1];
	Device->DATAZ=axes[2];
}
uint8_t ADXL_CheckDevice(ADXL345Data *Device)
{
	uint8_t txrx = adxl_read(Device->PORT, Device->PIN, ADXL345_DEVID);
	if(txrx)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void ADXL_ReadDevice(ADXL345Data *Device)
{
	Device->DATAX0=adxl_read(Device->PORT, Device->PIN, ADXL345_DATAX0);
	Device->DATAX1=adxl_read(Device->PORT, Device->PIN, ADXL345_DATAX1);
	Device->DATAY0=adxl_read(Device->PORT, Device->PIN, ADXL345_DATAY0);
	Device->DATAY1=adxl_read(Device->PORT, Device->PIN, ADXL345_DATAY1);
	Device->DATAZ0=adxl_read(Device->PORT, Device->PIN, ADXL345_DATAZ0);
	Device->DATAZ1=adxl_read(Device->PORT, Device->PIN, ADXL345_DATAZ1);

	Device->DATAX=(((uint16_t)Device->DATAX1)<<8)|Device->DATAX0;
	Device->DATAY=(((uint16_t)Device->DATAY1)<<8)|Device->DATAY0;
	Device->DATAZ=(((uint16_t)Device->DATAZ1)<<8)|Device->DATAZ0;
}
void ADXL_DeviceDump(ADXL345Data *Device, char *Dest, uint8_t Size)
{
	 snprintf(
			 Dest,
			 Size,
			 "%s: X: %d, Y: %d, Z: %d\r\n",
			 Device->name,
			 Device->DATAX,
			 Device->DATAY,
			 Device->DATAZ
			 );
}
void ADXL_SetMeasure(ADXL345Data *Device, uint8_t mode)
{
	uint8_t rx = adxl_read(Device->PORT, Device->PIN, ADXL345_POWER_CTL);
	uint8_t tx = rx&0xF7;
	if(mode)
	{
		tx|=(0x01<<3);
	}
	adxl_write(Device->PORT, Device->PIN, ADXL345_POWER_CTL, tx);
}
void ADXL_SetRange(ADXL345Data *Device, uint8_t Range)
{
	uint8_t rx = adxl_read(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT);
	uint8_t tx = 0;
	switch(Range)
	{
	case RANGE_2G:
		tx = (rx&0xFC);
		adxl_write(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT, tx);
		break;
	case RANGE_4G:
		tx = (rx&0xFC)|0x01;
		adxl_write(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT, tx);
		break;
	case RANGE_8G:
		tx = (rx&0xFC)|0x01<<1;
		adxl_write(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT, tx);
		break;
	case RANGE_16G:
		tx = (rx&0xFC)|0x03;
		adxl_write(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT, tx);
		break;
	}
}
void ADXL_SetFullResolution(ADXL345Data *Device)
{
	uint8_t rx = adxl_read(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT);
	uint8_t tx = rx|(0x01<<3);
	adxl_write(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT, tx);
}
void ADXL_SetJustify(ADXL345Data *Device, uint8_t mode)
{
	uint8_t txrx = (0xFB)&(adxl_read(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT));
	if(mode)
	{
		txrx|=(0x01<<2);
	}
	adxl_write(Device->PORT, Device->PIN, ADXL345_DATA_FORMAT, txrx);
}
void ADXL_SetOffset(ADXL345Data *Device, uint8_t offX, uint8_t offY, uint8_t offZ)
{
	adxl_write(Device->PORT, Device->PIN, ADXL345_OFSX, offX);
	adxl_write(Device->PORT, Device->PIN, ADXL345_OFSY, offY);
	adxl_write(Device->PORT, Device->PIN, ADXL345_OFSZ, offZ);
}

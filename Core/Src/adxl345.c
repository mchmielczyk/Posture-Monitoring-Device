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

void static adxl_write(ADXL345Data *Device, uint8_t reg,uint8_t value,ADXL345_Interface *Env)
{
	uint8_t tx[2]={reg,value};
	Env->cs_low(Device);
	Env->spi_tx(tx,2);
	Env->cs_high(Device);
}
uint8_t static adxl_read(ADXL345Data *Device,uint8_t reg,ADXL345_Interface *Env)
{
	uint8_t txrx[2]={ADXL345_SINGLE_BYTE_READ|reg,0x00};
	Env->cs_low(Device);
	Env->spi_txrx(txrx,txrx,2);
	Env->cs_high(Device);
	return txrx[1];
}
void static adxl_multi_read(ADXL345Data *Device, uint16_t *dest,ADXL345_Interface *Env)
{
	uint8_t txrx[7]={ADXL345_MULTI_BYTE_READ|ADXL345_DATAX0,0,0,0,0,0,0};
	Env->cs_low(Device);
	Env->spi_txrx(txrx,txrx,7);
	Env->cs_high(Device);
	dest[0]=(((uint16_t)txrx[2])<<8)|txrx[1];
	dest[1]=(((uint16_t)txrx[4])<<8)|txrx[3];
	dest[2]=(((uint16_t)txrx[6])<<8)|txrx[5];
}
void ADXL_MultiReadDevice(ADXL345Data *Device,ADXL345_Interface *Env)
{
	uint16_t axes[3];
	adxl_multi_read(Device,axes,Env);
	Device->DATAX=axes[0];
	Device->DATAY=axes[1];
	Device->DATAZ=axes[2];
}
uint8_t ADXL_CheckDevice(ADXL345Data *Device,ADXL345_Interface *Env)
{
	uint8_t txrx = adxl_read(Device, ADXL345_DEVID, Env);
	if(txrx)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void ADXL_ReadDevice(ADXL345Data *Device,ADXL345_Interface *Env)
{
	Device->DATAX0=adxl_read(Device, ADXL345_DATAX0,Env);
	Device->DATAX1=adxl_read(Device, ADXL345_DATAX1,Env);
	Device->DATAY0=adxl_read(Device, ADXL345_DATAY0,Env);
	Device->DATAY1=adxl_read(Device, ADXL345_DATAY1,Env);
	Device->DATAZ0=adxl_read(Device, ADXL345_DATAZ0,Env);
	Device->DATAZ1=adxl_read(Device, ADXL345_DATAZ1,Env);

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
void ADXL_SetMeasure(ADXL345Data *Device, uint8_t mode,ADXL345_Interface *Env)
{
	uint8_t rx = adxl_read(Device, ADXL345_POWER_CTL,Env);
	uint8_t tx = rx&0xF7;
	if(mode)
	{
		tx|=(0x01<<3);
	}
	adxl_write(Device, ADXL345_POWER_CTL, tx,Env);
}
void ADXL_SetRange(ADXL345Data *Device, uint8_t Range,ADXL345_Interface *Env)
{
	uint8_t rx = adxl_read(Device, ADXL345_DATA_FORMAT,Env);
	uint8_t tx = 0;
	switch(Range)
	{
	case RANGE_2G:
		tx = (rx&0xFC);
		adxl_write(Device, ADXL345_DATA_FORMAT, tx,Env);
		break;
	case RANGE_4G:
		tx = (rx&0xFC)|0x01;
		adxl_write(Device, ADXL345_DATA_FORMAT, tx,Env);
		break;
	case RANGE_8G:
		tx = (rx&0xFC)|0x01<<1;
		adxl_write(Device, ADXL345_DATA_FORMAT, tx,Env);
		break;
	case RANGE_16G:
		tx = (rx&0xFC)|0x03;
		adxl_write(Device, ADXL345_DATA_FORMAT, tx,Env);
		break;
	}
}
void ADXL_SetFullResolution(ADXL345Data *Device,ADXL345_Interface *Env)
{
	uint8_t rx = adxl_read(Device, ADXL345_DATA_FORMAT,Env);
	uint8_t tx = rx|(0x01<<3);
	adxl_write(Device, ADXL345_DATA_FORMAT, tx,Env);
}
void ADXL_SetJustify(ADXL345Data *Device, uint8_t mode,ADXL345_Interface *Env)
{
	uint8_t txrx = (0xFB)&(adxl_read(Device, ADXL345_DATA_FORMAT,Env));
	if(mode)
	{
		txrx|=(0x01<<2);
	}
	adxl_write(Device, ADXL345_DATA_FORMAT, txrx,Env);
}
void ADXL_SetOffset(ADXL345Data *Device, uint8_t offX, uint8_t offY, uint8_t offZ,ADXL345_Interface *Env)
{
	adxl_write(Device, ADXL345_OFSX, offX,Env);
	adxl_write(Device, ADXL345_OFSY, offY,Env);
	adxl_write(Device, ADXL345_OFSZ, offZ,Env);
}

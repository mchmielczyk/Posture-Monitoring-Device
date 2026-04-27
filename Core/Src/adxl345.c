/*
 * adxl345.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Michał Chmielczyk
 */

//sprawdzenie poprawnego devid +
//odczyt z akcelerometrow +
//zapis do akcelerometrow -
//handlowanie braku dostepu do urzadzenia -
//return statusu zadania -
//rtos safety -
//justify bound
//check device bound
// device dump bound
//set meaesure bound
// res offset justify bound
#include "adxl345.h"
ADXL345_Status static adxl_write(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t reg,uint8_t value)
{
	ADXL345_Status returnStatus;
	if(!Device||!Env)return ADXL345_ERROR;
	if((reg!=0&&reg<ADXL345_THRESH_TAP)||reg>ADXL345_FIFO_STATUS)return ADXL345_ERROR;
	uint8_t tx[2]={reg,value};
	Env->cs_low(Device);
	returnStatus = Env->spi_tx(tx,2);
	if(returnStatus!=ADXL345_OK)return returnStatus;
	Env->cs_high(Device);
	return returnStatus;
}
ADXL345_Status static adxl_read(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t reg,uint8_t *dest)
{
	ADXL345_Status returnStatus;
	if(!Device||!dest||!Env)return ADXL345_ERROR;
	if(((reg!=0)&&(reg<ADXL345_THRESH_TAP))||reg>ADXL345_FIFO_STATUS)return ADXL345_ERROR;
	uint8_t txrx[2]={ADXL345_SINGLE_BYTE_READ|reg,0x00};
	Env->cs_low(Device);
	returnStatus = Env->spi_txrx(txrx,txrx,2);
	if(returnStatus!=ADXL345_OK)return returnStatus;
	Env->cs_high(Device);
	*dest=txrx[1];
	return returnStatus;
}
ADXL345_Status static adxl_multi_read(ADXL345Data *Device,ADXL345_Interface *Env, uint16_t *dest)
{
	ADXL345_Status returnStatus;
	if(!Device||!dest||!Env)return ADXL345_ERROR;
	uint8_t txrx[7]={ADXL345_MULTI_BYTE_READ|ADXL345_DATAX0,0,0,0,0,0,0};
	Env->cs_low(Device);
	returnStatus = Env->spi_txrx(txrx,txrx,7);
	if(returnStatus!=ADXL345_OK)return returnStatus;
	Env->cs_high(Device);
	dest[0]=(((uint16_t)txrx[2])<<8)|txrx[1];
	dest[1]=(((uint16_t)txrx[4])<<8)|txrx[3];
	dest[2]=(((uint16_t)txrx[6])<<8)|txrx[5];
	return returnStatus;
}
ADXL345_Status ADXL_MultiReadDevice(ADXL345Data *Device,ADXL345_Interface *Env)
{
	ADXL345_Status returnStatus;
	if(!Device||!Env)return ADXL345_ERROR;
	uint16_t axes[3];
	returnStatus = adxl_multi_read(Device,Env,axes);
	Device->DATAX=axes[0];
	Device->DATAY=axes[1];
	Device->DATAZ=axes[2];
	return returnStatus;
}
uint8_t ADXL_CheckDevice(ADXL345Data *Device,ADXL345_Interface *Env)
{
	uint8_t txrx;
	adxl_read(Device,Env, ADXL345_DEVID,&txrx);
	if(txrx==0xE5)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
ADXL345_Status ADXL_ReadDevice(ADXL345Data *Device,ADXL345_Interface *Env)
{
	ADXL345_Status returnStatus;
	if(!Device||!Env)return ADXL345_ERROR;
	uint8_t * ptrDataTab[6]={
			&Device->DATAX0,
			&Device->DATAX1,
			&Device->DATAY0,
			&Device->DATAY1,
			&Device->DATAZ0,
			&Device->DATAZ1,
	};
	for(int i=0;i<6;i++)
	{
		returnStatus=adxl_read(Device,Env, (ADXL345_DATAX0+i),ptrDataTab[i]);
			if(returnStatus!=ADXL345_OK)return returnStatus;
	}
	Device->DATAX=(((uint16_t)Device->DATAX1)<<8)|Device->DATAX0;
	Device->DATAY=(((uint16_t)Device->DATAY1)<<8)|Device->DATAY0;
	Device->DATAZ=(((uint16_t)Device->DATAZ1)<<8)|Device->DATAZ0;
	return ADXL345_OK;
}
ADXL345_Status ADXL_DeviceDump(ADXL345Data *Device, char *Dest, uint8_t Size)
{
	if(!Device||!Dest)return ADXL345_ERROR;
	int DumpStatus = snprintf(
			 Dest,
			 Size,
			 "%s: X: %d, Y: %d, Z: %d\r\n",
			 Device->name,
			 Device->DATAX,
			 Device->DATAY,
			 Device->DATAZ
			 );
	if(DumpStatus>-1&&DumpStatus<Size)
	{
		return ADXL345_OK;
	}
	return ADXL345_ERROR;
}
ADXL345_Status ADXL_SetMeasure(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t mode)//
{
	uint8_t rx;
	adxl_read(Device,Env, ADXL345_POWER_CTL,&rx);
	uint8_t tx = rx&0xF7;
	if(mode)
	{
		tx|=(0x01<<3);
	}
	adxl_write(Device,Env, ADXL345_POWER_CTL, tx);
}
ADXL345_Status ADXL_SetRange(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t Range)//
{
	ADXL345_Status returnStatus;
	if((!Device||!Env)||(Range<0||Range>3))return ADXL345_ERROR;
	uint8_t rx;
	returnStatus = adxl_read(Device,Env, ADXL345_DATA_FORMAT,&rx);
	if(returnStatus!=ADXL345_OK)return returnStatus;
	uint8_t tx = 0;
	switch(Range)
	{
	case RANGE_2G:
		tx = (rx&0xFC);
		returnStatus = adxl_write(Device,Env, ADXL345_DATA_FORMAT, tx);
		return returnStatus;
	case RANGE_4G:
		tx = (rx&0xFC)|0x01;
		returnStatus = adxl_write(Device,Env, ADXL345_DATA_FORMAT, tx);
		return returnStatus;
	case RANGE_8G:
		tx = (rx&0xFC)|0x02;
		returnStatus = adxl_write(Device,Env, ADXL345_DATA_FORMAT, tx);
		return returnStatus;
	case RANGE_16G:
		tx = (rx&0xFC)|0x03;
		returnStatus = adxl_write(Device,Env, ADXL345_DATA_FORMAT, tx);
		return returnStatus;
	default:
		return ADXL345_ERROR;
	}
}
ADXL345_Status ADXL_SetFullResolution(ADXL345Data *Device,ADXL345_Interface *Env)//
{
	uint8_t rx;
	adxl_read(Device,Env, ADXL345_DATA_FORMAT,&rx);
	uint8_t tx = rx|(0x01<<3);
	adxl_write(Device,Env, ADXL345_DATA_FORMAT, tx);
}
ADXL345_Status ADXL_SetJustify(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t mode)//
{
	uint8_t txrx;
	adxl_read(Device,Env, ADXL345_DATA_FORMAT,&txrx);
	txrx&=(0xFB);
	if(mode)
	{
		txrx|=(0x01<<2);
	}
	adxl_write(Device,Env, ADXL345_DATA_FORMAT, txrx);
}
ADXL345_Status ADXL_SetOffset(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t offX, uint8_t offY, uint8_t offZ)//
{
	adxl_write(Device,Env, ADXL345_OFSX, offX);
	adxl_write(Device,Env, ADXL345_OFSY, offY);
	adxl_write(Device,Env, ADXL345_OFSZ, offZ);
}

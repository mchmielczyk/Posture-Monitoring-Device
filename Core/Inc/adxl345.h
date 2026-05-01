/*
 * adxl345.h
 *
 *  Created on: Nov 25, 2025
 *      Author: Michał Chmielczyk
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include <stdint.h>
#include <stdio.h>

#define ADXL345_DEVID 				0x00 //Device ID
#define ADXL345_THRESH_TAP 			0x1D //Tap threshold
#define ADXL345_OFSX 				0x1E //X-axis offset
#define ADXL345_OFSY 				0x1F //Y-axis offset
#define ADXL345_OFSZ 				0x20 //Z-axis offset
#define ADXL345_DUR 				0x21 //Tap duration
#define ADXL345_Latent 				0x22 //Tap latency
#define ADXL345_Window 				0x23 //Tap window
#define ADXL345_THRESH_ACT 			0x24 //Activity threshold
#define ADXL345_THRESH_INACT 		0x25 //Inactivity threshold
#define ADXL345_TIME_INACT 			0x26 //Inactivity time
#define ADXL345_ACT_INACT_CTL 		0x27 //Axis enable control for activity and inactivity detection
#define ADXL345_THRESH_FF 			0x28 //Free-fall threshold
#define ADXL345_TIME_FF 			0x29 //Free-fall time
#define ADXL345_TAP_AXES 			0x2A //Axis control for tap/double tap
#define ADXL345_ACT_TAP_STATUS 		0x2B //Source of tap/double tap
#define ADXL345_BW_RATE 			0x2C //Data rate and power mode control
#define ADXL345_POWER_CTL 			0x2D //Power-saving features control
#define ADXL345_INT_ENABLE 			0x2E //Interrupt enable control
#define ADXL345_INT_MAP 			0x2F //Interrupt mapping control
#define ADXL345_INT_SOURCE 			0x30 //Source of interrupts
#define ADXL345_DATA_FORMAT 		0x31 //Data format control
#define ADXL345_DATAX0 				0x32 //X-Axis Data 0
#define ADXL345_DATAX1 				0x33 //X-Axis Data 1
#define ADXL345_DATAY0 				0x34 //Y-Axis Data 0
#define ADXL345_DATAY1 				0x35 //Y-Axis Data 1
#define ADXL345_DATAZ0 				0x36 //Z-Axis Data 0
#define ADXL345_DATAZ1 				0x37 //Z-Axis Data 1
#define ADXL345_FIFO_CTL 			0x38 //FIFO control
#define ADXL345_FIFO_STATUS 		0x39 //FIFO status

#define RANGE_2G					0x00 //+-2g Range
#define RANGE_4G					0x01 //+-4g Range
#define RANGE_8G					0x02 //+-8g Range
#define RANGE_16G					0x03 //+-16g Range

#define ADXL345_SET					0x01 //High
#define ADXL345_RESET				0x00 //Low

#define ADXL345_SINGLE_BYTE_READ	(0x01<<7)
#define ADXL345_MULTI_BYTE_READ		((0x01<<7)|(0x01<<6))


typedef enum
{
	ADXL345_OK       = 0x00,
	ADXL345_ERROR    = 0x01,
	ADXL345_BUSY     = 0x02,
	ADXL345_TIMEOUT  = 0x03
} ADXL345_Status;

typedef struct
{
uint8_t 							DATAX0;
uint8_t 							DATAX1;
uint8_t 							DATAY0;
uint8_t 							DATAY1;
uint8_t  							DATAZ0;
uint8_t 							DATAZ1;
uint16_t 							DATAX;
uint16_t 							DATAY;
uint16_t 							DATAZ;
void*								PORT;
uint16_t 							PIN;
char 								name[3];
}ADXL345Data;

typedef void 						(*GPIO_WritePin_fn)(ADXL345Data *Device);
typedef ADXL345_Status 				(*SPI_Transmit_fn)(uint8_t* tx,uint16_t size);
typedef ADXL345_Status 				(*SPI_TransmitReceive_fn)(uint8_t* tx,uint8_t*rx,uint16_t size);
typedef void 						(*ERR_RuntimeError)(const char * message, int parameter,const char * file, int line);

typedef struct{
	GPIO_WritePin_fn 				cs_high;
	GPIO_WritePin_fn 				cs_low;
	SPI_Transmit_fn 				spi_tx;
	SPI_TransmitReceive_fn 			spi_txrx;
	ADXL345_Status 					status_type;
	ERR_RuntimeError				err_runtime;
}ADXL345_Interface;

ADXL345_Status ADXL_ReadDevice(ADXL345Data *Device,ADXL345_Interface *Env);
ADXL345_Status ADXL_SetMeasure(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t mode);
ADXL345_Status ADXL_DeviceDump(ADXL345Data *Device, char *Dest, uint8_t Size);
ADXL345_Status ADXL_SetRange(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t Range);
ADXL345_Status ADXL_SetFullResolution(ADXL345Data *Device,ADXL345_Interface *Env);
ADXL345_Status ADXL_SetJustify(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t mode);
uint8_t ADXL_CheckDevice(ADXL345Data *Device,ADXL345_Interface *Env);
ADXL345_Status ADXL_SetOffset(ADXL345Data *Device,ADXL345_Interface *Env, uint8_t offX, uint8_t offY, uint8_t offZ);
ADXL345_Status ADXL_MultiReadDevice(ADXL345Data *Device,ADXL345_Interface *Env);

#endif /* INC_ADXL345_H_ */

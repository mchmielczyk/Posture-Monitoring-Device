#include "adxl345spy.h"
static uint8_t lastRead;
static uint8_t lastWrite;

uint8_t ADXL345Spy_GetLastRead(void){return lastRead;}
void ADXL345Spy_SetLastRead(uint8_t read){lastRead = read;}
uint8_t ADXL345Spy_GetLastWrite(void){return lastWrite;}
void ADXL345Spy_SetLastWrite(uint8_t write){lastWrite = write;}
void ADXL345Spy_Create(void)
{
	lastRead=-1;
	lastWrite=-1;
}
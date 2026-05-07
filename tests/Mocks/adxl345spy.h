#ifndef _FAKE_HAL_H_
#define _FAKE_HAL_H_
#include "stdint.h"
void ADXL345Spy_Create(void);
uint8_t ADXL345Spy_GetLastRead(void);
uint8_t ADXL345Spy_GetLastWrite(void);
void ADXL345Spy_SetLastRead(uint8_t read);
void ADXL345Spy_SetLastWrite(uint8_t write);
#endif
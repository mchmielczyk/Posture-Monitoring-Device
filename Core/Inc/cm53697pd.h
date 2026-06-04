/*
 * cm53697pd.h
 *
 *  Created on: Dec 5, 2025
 *      Author: Michał
 */

#ifndef INC_CM53697PD_H_
#define INC_CM53697PD_H_

#include "adxl345.h"

extern ADXL345Data AdxlLAR;
extern ADXL345Data AdxlRAR;
extern ADXL345Data AdxlLFO;
extern ADXL345Data AdxlRFO;
extern ADXL345Data AdxlBAC;

extern ADXL345Data* devicesPtr[5];
extern ADXL345Driver* driverPtr[5];

void ADXL_RawData(ADXL345Data* Device[], char* Dest, uint8_t Size);
void DWT_CycleStart(void);
#endif /* INC_CM53697PD_H_ */

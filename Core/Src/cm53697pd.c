/*
 * cm53697pd.c
 *
 *  Created on: Dec 23, 2025
 *      Author: Michał
 */
#include "adxl345.h"

void ADXL_RawData(ADXL345Data *Device[], char*Dest, uint8_t Size)
{
	snprintf(
			 Dest,
			 Size,
			 "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\r\n",
			 Device[0]->DATAX,
			 Device[0]->DATAY,
			 Device[0]->DATAZ,
			 Device[1]->DATAX,
			 Device[1]->DATAY,
			 Device[1]->DATAZ,
			 Device[2]->DATAX,
			 Device[2]->DATAY,
			 Device[2]->DATAZ,
			 Device[3]->DATAX,
			 Device[3]->DATAY,
			 Device[3]->DATAZ,
			 Device[4]->DATAX,
			 Device[4]->DATAY,
			 Device[4]->DATAZ
			 );
}
void DWT_CycleStart(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT=0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/*
 * cm53697pd.c
 *
 *  Created on: Dec 23, 2025
 *      Author: Michał
 */
#include "cm53697pd.h"
#include "adxl345.h"
#include "gpio.h"
#include "spi.h"

ADXL345Data AdxlLAR = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, LAR_CS_GPIO_Port, LAR_CS_Pin, "LAR"};
ADXL345Data AdxlRAR = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, RAR_CS_GPIO_Port, RAR_CS_Pin, "RAR"};
ADXL345Data AdxlLFO = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, LFO_CS_GPIO_Port, LFO_CS_Pin, "LFO"};
ADXL345Data AdxlRFO = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, RFO_CS_GPIO_Port, RFO_CS_Pin, "RFO"};
ADXL345Data AdxlBAC = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, BAC_CS_GPIO_Port, BAC_CS_Pin, "BAC"};

ADXL345Data* devicesPtr[5] = {&AdxlLAR, &AdxlRAR, &AdxlLFO, &AdxlRFO, &AdxlBAC};

ADXL345Driver driverPtr[5];

void ADXL_RawData(ADXL345Data* Device[], char* Dest, uint8_t Size)
{
    snprintf(Dest,
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
             Device[4]->DATAZ);
}
void DWT_CycleStart(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

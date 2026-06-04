#include "../../Core/Inc/adxl345.h"
#include "../spies/adxl345spy.h"
ADXL345_Status FAKE_Write(const ADXL345Driver* driver, uint8_t* tx, uint16_t size)
{
    adxl345spy_SetLastWrite(*tx);
    return ((*tx == 0U) || ((*tx >= ADXL345_THRESH_TAP) && (*tx <= ADXL345_FIFO_STATUS)))
               ? ADXL345_OK
               : ADXL345_ERROR;
}

ADXL345_Status FAKE_Read(const ADXL345Driver* driver, uint8_t* tx, uint8_t* rx, uint16_t size)
{
    ADXL345_Status returnStatus = ADXL345_ERROR;

    adxl345spy_SetLastWrite(tx[0]);

    if (((tx[0] & (ADXL345_MULTI_BYTE_READ | ADXL345_DATAX0)) ==
         (ADXL345_MULTI_BYTE_READ | ADXL345_DATAX0)))
    {

        rx[1] = 2;
        rx[2] = 4;
        rx[3] = 8;
        rx[4] = 16;
        rx[5] = 32;
        rx[6] = 64;
        adxl345spy_SetLastRead(rx[6]);
        returnStatus = ADXL345_OK;
    }
    if ((tx[0] & ADXL345_SINGLE_BYTE_READ) && returnStatus != ADXL345_OK)
    {
        uint8_t reg = tx[0] & ~ADXL345_SINGLE_BYTE_READ;
        switch (reg)
        {
        case ADXL345_DEVID:
            rx[1] = ADXL345_DEVID_correct_code;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_POWER_CTL:
            rx[1] = 0x00;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_DATA_FORMAT:
            rx[1] = ADXL345_DATA_FORMAT_range_bits;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_DATAX0:
            rx[1] = 2;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_DATAX1:
            rx[1] = 4;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_DATAY0:
            rx[1] = 8;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_DATAY1:
            rx[1] = 16;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_DATAZ0:
            rx[1] = 32;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        case ADXL345_DATAZ1:
            rx[1] = 64;
            returnStatus = ADXL345_OK;
            adxl345spy_SetLastRead(rx[1]);
            break;
        default:
            returnStatus = ADXL345_ERROR;
            break;
        }
    }
    return returnStatus;
}

ADXL345Interface FAKE_Iface = {.write = FAKE_Write, .read = FAKE_Read};

ADXL345Interface* getFakeInterface(void)
{
    return &FAKE_Iface;
}
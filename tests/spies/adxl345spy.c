#include "adxl345spy.h"

static uint8_t lastRead;
static uint8_t lastWrite;

void adxl345spy_Create(void)
{
    lastRead = LAST_READ_UNKNOWN;
    lastWrite = LAST_WRITE_UNKNOWN;
}
uint8_t adxl345spy_GetLastRead(void)
{
    return lastRead;
}
uint8_t adxl345spy_GetLastWrite(void)
{
    return lastWrite;
}
void adxl345spy_SetLastRead(uint8_t read)
{
    lastRead = read;
}
void adxl345spy_SetLastWrite(uint8_t write)
{
    lastWrite = write;
}
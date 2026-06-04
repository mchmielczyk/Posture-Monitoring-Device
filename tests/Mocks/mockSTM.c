#include "../../Core/Inc/adxl345.h"
#include "../units/adxl345driverTest.hpp"

ADXL345_Status MOCK_Write(const ADXL345Driver* driver, uint8_t* tx, uint16_t size)
{
    mock_c()->actualCall("write")->withIntParameters("tx", *tx)->withIntParameters("size", size);
    return ADXL345_Status(mock_c()->returnIntValueOrDefault(0));
}
ADXL345_Status MOCK_Read(const ADXL345Driver* driver, uint8_t* tx, uint8_t* rx, uint16_t size)
{
    mock_c()
        ->actualCall("read")
        ->withIntParameters("tx", *tx)
        ->withIntParameters("size", size)
        ->withOutputParameter("rx", rx);
    return ADXL345_Status(mock_c()->returnIntValueOrDefault(0));
}

static ADXL345Interface MOCK_Iface = {.write = MOCK_Write, .read = MOCK_Read};

ADXL345Interface* getMockInterface(void)
{
    return &MOCK_Iface;
}
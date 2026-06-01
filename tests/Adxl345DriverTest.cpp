#include "Adxl345DriverTest.hpp"
extern "C"
{
#include "Mocks/adxl345spy.h"
    extern ADXL345_Interface FAKE_ENV;
}
TEST_GROUP(ADXL345Driver)
{
    ADXL345Data Device;
#define TEST_CS_GPIO_PORT 0x10
#define TEST_CS_PIN 0x20

    void setup()
    {
        Device = (ADXL345Data){0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               0x00,
                               (void*)TEST_CS_GPIO_PORT,
                               TEST_CS_PIN};
        strcpy(Device.name, "LAR");
    }
    void teardown()
    {
    }
};
TEST(ADXL345Driver, DEVIDConnection)
{
    LONGS_EQUAL(0, ADXL_CheckDevice(&Device, &FAKE_ENV));
}
TEST(ADXL345Driver, ReadSingleDeviceProperSingleRead)
{
    ADXL_ReadDevice(&Device, &FAKE_ENV);
    LONGS_EQUAL(2, Device.DATAX0);
    LONGS_EQUAL(4, Device.DATAX1);
    LONGS_EQUAL(6, Device.DATAY0);
    LONGS_EQUAL(8, Device.DATAY1);
    LONGS_EQUAL(10, Device.DATAZ0);
    LONGS_EQUAL(12, Device.DATAZ1);
    LONGS_EQUAL(((uint16_t)0x04 << 8) | 0x02, Device.DATAX);
    LONGS_EQUAL(((uint16_t)0x08 << 8) | 0x06, Device.DATAY);
    LONGS_EQUAL(((uint16_t)0x0c << 8) | 0x0a, Device.DATAZ);
}
TEST(ADXL345Driver, ReadSingleDeviceProperMultiRead)
{
    ADXL_MultiReadDevice(&Device, &FAKE_ENV);
    LONGS_EQUAL(((uint16_t)0x04 << 8) | 0x02, Device.DATAX);
    LONGS_EQUAL(((uint16_t)0x08 << 8) | 0x06, Device.DATAY);
    LONGS_EQUAL(((uint16_t)0x0c << 8) | 0x0a, Device.DATAZ);
}
TEST(ADXL345Driver, ReturnStatusSingleRead)
{
    LONGS_EQUAL(0, (int)ADXL_ReadDevice(&Device, &FAKE_ENV));
}
TEST(ADXL345Driver, NoDeviceOrInterfaceSingleRead)
{
    LONGS_EQUAL(1, (int)ADXL_ReadDevice((ADXL345Data*)0, (ADXL345_Interface*)0));
}
TEST(ADXL345Driver, ReturnStatusMultiRead)
{
    LONGS_EQUAL(0, (int)ADXL_MultiReadDevice(&Device, &FAKE_ENV));
}
TEST(ADXL345Driver, NoDeviceOrInterfaceMultiRead)
{
    LONGS_EQUAL(1, (int)ADXL_MultiReadDevice((ADXL345Data*)0, (ADXL345_Interface*)0));
}
TEST(ADXL345Driver, SetRangeBoundaryCond)
{
    LONGS_EQUAL(0, ADXL_SetRange(&Device, &FAKE_ENV, 0));
    LONGS_EQUAL(0, ADXL_SetRange(&Device, &FAKE_ENV, 3));
    LONGS_EQUAL(1, ADXL_SetRange(&Device, &FAKE_ENV, -1));
    LONGS_EQUAL(1, ADXL_SetRange(&Device, &FAKE_ENV, 4));
    LONGS_EQUAL(1, ADXL_SetRange((ADXL345Data*)0, (ADXL345_Interface*)0, 4));
}
TEST(ADXL345Driver, DeviceDumpProper)
{
    char buffer[100];
    ENUMS_EQUAL_INT(ADXL345_OK, ADXL_DeviceDump(&Device, buffer, sizeof(buffer)));
}
TEST(ADXL345Driver, DeviceDumpBoundaryTooSmallBuffer)
{
    char buffer[5];
    LONGS_EQUAL(1, ADXL_DeviceDump(&Device, buffer, sizeof(buffer)));
}
TEST(ADXL345Driver, DeviceDumpBoundaryWrongDevice)
{
    char buffer[100];
    LONGS_EQUAL(1, ADXL_DeviceDump((ADXL345Data*)0, buffer, sizeof(buffer)));
}
TEST(ADXL345Driver, SetFullResolutionProper)
{
    LONGS_EQUAL(0, ADXL_SetFullResolution(&Device, &FAKE_ENV));
}
TEST(ADXL345Driver, SetFullResolutionBoundaryWrongInterface)
{
    LONGS_EQUAL(1, ADXL_SetFullResolution(&Device, (ADXL345_Interface*)0));
}
TEST(ADXL345Driver, SetJustifyProper)
{
    LONGS_EQUAL(0, ADXL_SetJustify(&Device, &FAKE_ENV, ADXL345_RESET));
}
TEST(ADXL345Driver, SetJustifyBoundaryWrongInterface)
{
    LONGS_EQUAL(1, ADXL_SetJustify(&Device, (ADXL345_Interface*)0, ADXL345_RESET));
}
TEST(ADXL345Driver, SetJustifyBoundaryWrongMode)
{
    LONGS_EQUAL(1, ADXL_SetJustify(&Device, (ADXL345_Interface*)0, -1));
}
TEST(ADXL345Driver, SetMeasureProper)
{
    LONGS_EQUAL(0, ADXL_SetMeasure(&Device, &FAKE_ENV, ADXL345_RESET));
}
TEST(ADXL345Driver, SetMeasureBoundaryWrongInterface)
{
    LONGS_EQUAL(1, ADXL_SetMeasure(&Device, (ADXL345_Interface*)0, ADXL345_RESET));
}
TEST(ADXL345Driver, SetMeasureBoundaryWrongMode)
{
    LONGS_EQUAL(1, ADXL_SetMeasure(&Device, (ADXL345_Interface*)0, -1));
}
TEST(ADXL345Driver, SetOffsetProper)
{
    LONGS_EQUAL(0, ADXL_SetOffset(&Device, &FAKE_ENV, 1, 1, 1));
}
TEST(ADXL345Driver, SetOffsetBoundaryWrongInterface)
{
    LONGS_EQUAL(1, ADXL_SetOffset(&Device, (ADXL345_Interface*)0, 1, 1, 1));
}
TEST(ADXL345Driver, SetOffsetBoundaryWrongData)
{
    LONGS_EQUAL(0, ADXL_SetOffset(&Device, &FAKE_ENV, -1, -1, -1));
}
TEST(ADXL345Driver, CheckDeviceProperCall)
{
    LONGS_EQUAL(0, ADXL_CheckDevice(&Device, &FAKE_ENV));
    LONGS_EQUAL(128, ADXL345Spy_GetLastRead());
}
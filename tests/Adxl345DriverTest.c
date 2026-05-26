#include "Adxl345DriverTest.h"
TEST_GROUP(ADXL345DRIVER);

ADXL345Data Device;
extern ADXL345_Interface FAKE_ENV;
ADXL345Driver FAKE_DRIVER;

#define TEST_CS_GPIO_PORT 0x10
#define TEST_CS_PIN 0x20

TEST_SETUP(ADXL345DRIVER)
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
                           TEST_CS_PIN,
                           "LAR"};
    ADXL345_Interface new_iface = FAKE_ENV;
    ADXL345Driver FAKE_DRIVER = {.device = &Device, .iface = &new_iface, .id = 0};
}
TEST_TEAR_DOWN(ADXL345DRIVER)
{
}
TEST(ADXL345DRIVER, DEVIDConnection)
{
    TEST_ASSERT_EQUAL(1, ADXL_CheckDevice(&FAKE_DRIVER));
}
TEST(ADXL345DRIVER, ReadSingleDeviceProperSingleRead)
{
    ADXL_ReadDevice(&FAKE_DRIVER);
    TEST_ASSERT_EQUAL(2, Device.DATAX0);
    TEST_ASSERT_EQUAL(4, Device.DATAX1);
    TEST_ASSERT_EQUAL(6, Device.DATAY0);
    TEST_ASSERT_EQUAL(8, Device.DATAY1);
    TEST_ASSERT_EQUAL(10, Device.DATAZ0);
    TEST_ASSERT_EQUAL(12, Device.DATAZ1);
    TEST_ASSERT_EQUAL(((uint16_t)0x04 << 8) | 0x02, Device.DATAX);
    TEST_ASSERT_EQUAL(((uint16_t)0x08 << 8) | 0x06, Device.DATAY);
    TEST_ASSERT_EQUAL(((uint16_t)0x0c << 8) | 0x0a, Device.DATAZ);
}
TEST(ADXL345DRIVER, ReadSingleDeviceProperMultiRead)
{
    ADXL_MultiReadDevice(&FAKE_DRIVER);
    TEST_ASSERT_EQUAL(((uint16_t)0x04 << 8) | 0x02, Device.DATAX);
    TEST_ASSERT_EQUAL(((uint16_t)0x08 << 8) | 0x06, Device.DATAY);
    TEST_ASSERT_EQUAL(((uint16_t)0x0c << 8) | 0x0a, Device.DATAZ);
}
TEST(ADXL345DRIVER, ReturnStatusSingleRead)
{
    TEST_ASSERT_EQUAL(0, (int)ADXL_ReadDevice(&FAKE_DRIVER));
}
TEST(ADXL345DRIVER, NoDeviceOrInterfaceSingleRead)
{
    TEST_ASSERT_EQUAL(1, (int)ADXL_ReadDevice((ADXL345Driver*)0));
}
TEST(ADXL345DRIVER, ReturnStatusMultiRead)
{
    TEST_ASSERT_EQUAL(0, (int)ADXL_MultiReadDevice(&FAKE_DRIVER));
}
TEST(ADXL345DRIVER, NoDeviceOrInterfaceMultiRead)
{
    TEST_ASSERT_EQUAL(1, (int)ADXL_MultiReadDevice((ADXL345Driver*)0));
}
TEST(ADXL345DRIVER, SetRangeBoundaryCond)
{
    TEST_ASSERT_EQUAL(0, ADXL_SetRange(&FAKE_DRIVER, 0));
    TEST_ASSERT_EQUAL(0, ADXL_SetRange(&FAKE_DRIVER, 3));
    TEST_ASSERT_EQUAL(1, ADXL_SetRange(&FAKE_DRIVER, -1));
    TEST_ASSERT_EQUAL(1, ADXL_SetRange(&FAKE_DRIVER, 4));
    TEST_ASSERT_EQUAL(1, ADXL_SetRange((ADXL345Driver*)0, 4));
}
TEST(ADXL345DRIVER, DeviceDumpProper)
{
    char buffer[100];
    TEST_ASSERT_EQUAL(0, ADXL_DeviceDump(&FAKE_DRIVER, &buffer, sizeof(buffer)));
}
TEST(ADXL345DRIVER, DeviceDumpBoundaryTooSmallBuffer)
{
    char buffer[5];
    TEST_ASSERT_EQUAL(1, ADXL_DeviceDump(&FAKE_DRIVER, &buffer, sizeof(buffer)));
}
TEST(ADXL345DRIVER, DeviceDumpBoundaryWrongDevice)
{
    char buffer[100];
    TEST_ASSERT_EQUAL(1, ADXL_DeviceDump((ADXL345Driver*)0, &buffer, sizeof(buffer)));
}
TEST(ADXL345DRIVER, DeviceDumpBoundaryWrongInterface)
{
    char buffer[100];
    TEST_ASSERT_EQUAL(1, ADXL_DeviceDump((ADXL345Driver*)0, &buffer, sizeof(buffer)));
}
TEST(ADXL345DRIVER, SetFullResolutionProper)
{
    TEST_ASSERT_EQUAL(0, ADXL_SetFullResolution(&FAKE_DRIVER));
}
TEST(ADXL345DRIVER, SetFullResolutionBoundaryWrongInterface)
{
    TEST_ASSERT_EQUAL(1, ADXL_SetFullResolution((ADXL345Driver*)0));
}
TEST(ADXL345DRIVER, SetJustifyProper)
{
    TEST_ASSERT_EQUAL(0, ADXL_SetJustify(&FAKE_DRIVER, ADXL345_RESET));
}
TEST(ADXL345DRIVER, SetJustifyBoundaryWrongInterface)
{
    TEST_ASSERT_EQUAL(1, ADXL_SetJustify((ADXL345Driver*)0, ADXL345_RESET));
}
TEST(ADXL345DRIVER, SetJustifyBoundaryWrongMode)
{
    TEST_ASSERT_EQUAL(1, ADXL_SetJustify((ADXL345Driver*)0, -1));
}
TEST(ADXL345DRIVER, SetMeasureProper)
{
    TEST_ASSERT_EQUAL(0, ADXL_SetMeasure(&FAKE_DRIVER, ADXL345_RESET));
}
TEST(ADXL345DRIVER, SetMeasureBoundaryWrongInterface)
{
    TEST_ASSERT_EQUAL(1, ADXL_SetMeasure((ADXL345Driver*)0, ADXL345_RESET));
}
TEST(ADXL345DRIVER, SetMeasureBoundaryWrongMode)
{
    TEST_ASSERT_EQUAL(1, ADXL_SetMeasure((ADXL345Driver*)0, -1));
}
TEST(ADXL345DRIVER, SetOffsetProper)
{
    TEST_ASSERT_EQUAL(0, ADXL_SetOffset(&FAKE_DRIVER, 1, 1, 1));
}
TEST(ADXL345DRIVER, SetOffsetBoundaryWrongInterface)
{
    TEST_ASSERT_EQUAL(1, ADXL_SetOffset((ADXL345Driver*)0, 1, 1, 1));
}
TEST(ADXL345DRIVER, SetOffsetBoundaryWrongData)
{
    TEST_ASSERT_EQUAL(0, ADXL_SetOffset(&FAKE_DRIVER, -1, -1, -1));
}
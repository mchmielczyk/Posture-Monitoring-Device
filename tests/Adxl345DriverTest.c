#include "Adxl345DriverTest.h"
TEST_GROUP(ADXL345Driver);

ADXL345Data Device;
extern ADXL345_Interface FAKE_ENV;
#define TEST_CS_GPIO_PORT           0x10
#define TEST_CS_PIN                 0x20


TEST_SETUP(ADXL345Driver)
{
    Device = (ADXL345Data){0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,(void*)TEST_CS_GPIO_PORT,TEST_CS_PIN,"LAR"};
}
TEST_TEAR_DOWN(ADXL345Driver)
{

}
TEST(ADXL345Driver, DEVIDConnection)
{
    TEST_ASSERT_EQUAL(1,ADXL_CheckDevice(&Device,&FAKE_ENV));
}
TEST(ADXL345Driver, ReadSingleDeviceProperSingleRead)
{
    ADXL_ReadDevice(&Device,&FAKE_ENV);
    TEST_ASSERT_EQUAL(2,Device.DATAX0);
    TEST_ASSERT_EQUAL(4,Device.DATAX1);
    TEST_ASSERT_EQUAL(6,Device.DATAY0);
    TEST_ASSERT_EQUAL(8,Device.DATAY1);
    TEST_ASSERT_EQUAL(10,Device.DATAZ0);
    TEST_ASSERT_EQUAL(12,Device.DATAZ1);
    TEST_ASSERT_EQUAL(((uint16_t)0x04<<8)|0x02,Device.DATAX);
    TEST_ASSERT_EQUAL(((uint16_t)0x08<<8)|0x06,Device.DATAY);
    TEST_ASSERT_EQUAL(((uint16_t)0x0c<<8)|0x0a,Device.DATAZ);
}
TEST(ADXL345Driver, ReadSingleDeviceProperMultiRead)
{
    ADXL_MultiReadDevice(&Device,&FAKE_ENV);
    TEST_ASSERT_EQUAL(((uint16_t)0x04<<8)|0x02,Device.DATAX);
    TEST_ASSERT_EQUAL(((uint16_t)0x08<<8)|0x06,Device.DATAY);
    TEST_ASSERT_EQUAL(((uint16_t)0x0c<<8)|0x0a,Device.DATAZ);
}
TEST(ADXL345Driver, ReturnStatusSingleRead)
{
    TEST_ASSERT_EQUAL(0, (int)ADXL_ReadDevice(&Device,&FAKE_ENV));
}
TEST(ADXL345Driver, NoDeviceOrInterfaceSingleRead)
{
    TEST_ASSERT_EQUAL(1, (int)ADXL_ReadDevice((ADXL345Data*)0,(ADXL345_Interface*)0));
}
TEST(ADXL345Driver, ReturnStatusMultiRead)
{
    TEST_ASSERT_EQUAL(0, (int)ADXL_MultiReadDevice(&Device,&FAKE_ENV));
}
TEST(ADXL345Driver, NoDeviceOrInterfaceMultiRead)
{
    TEST_ASSERT_EQUAL(1, (int)ADXL_MultiReadDevice((ADXL345Data*)0,(ADXL345_Interface*)0));
}
TEST(ADXL345Driver, SetRangeBoundaryCond)
{
    TEST_ASSERT_EQUAL(0, ADXL_SetRange(&Device,&FAKE_ENV,0));
    TEST_ASSERT_EQUAL(0, ADXL_SetRange(&Device,&FAKE_ENV,3));
    TEST_ASSERT_EQUAL(1, ADXL_SetRange(&Device,&FAKE_ENV,-1));
    TEST_ASSERT_EQUAL(1, ADXL_SetRange(&Device,&FAKE_ENV,4));
    TEST_ASSERT_EQUAL(1, ADXL_SetRange((ADXL345Data*)0,(ADXL345_Interface*)0,4));
}
TEST(ADXL345Driver, DeviceDumpProper)
{
    char buffer[100];
    TEST_ASSERT_EQUAL(0,ADXL_DeviceDump(&Device,&buffer,sizeof(buffer)));
}
TEST(ADXL345Driver, DeviceDumpBoundaryTooSmallBuffer)
{
    char buffer[5];
    TEST_ASSERT_EQUAL(1,ADXL_DeviceDump(&Device,&buffer,sizeof(buffer))); 
}
TEST(ADXL345Driver, DeviceDumpBoundaryWrongDevice)
{
    char buffer[100];
    TEST_ASSERT_EQUAL(1,ADXL_DeviceDump((ADXL345Data*)0,&buffer,sizeof(buffer)));
}
TEST(ADXL345Driver, DeviceDumpBoundaryWrongInterface)
{
    char buffer[100];
    TEST_ASSERT_EQUAL(1,ADXL_DeviceDump(&Device,(ADXL345_Interface*)0,sizeof(buffer)));
}
TEST(ADXL345Driver, SetFullResolutionProper)
{
    TEST_ASSERT_EQUAL(0,ADXL_SetFullResolution(&Device,&FAKE_ENV));
}
TEST(ADXL345Driver, SetFullResolutionBoundaryWrongInterface)
{
    TEST_ASSERT_EQUAL(1,ADXL_SetFullResolution(&Device,(ADXL345_Interface*)0));
}
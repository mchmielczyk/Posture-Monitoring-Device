#include "Adxl345DriverTest.h"
TEST_GROUP(ADXL345Driver);

ADXL345Data Device;
extern ADXL345_Interface FAKE_ENV;
#define TEST_CS_GPIO_PORT           0x10
#define TEST_CS_PIN                 0x20


TEST_SETUP(ADXL345Driver)
{
    Device = (ADXL345Data){0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,(void*)TEST_CS_GPIO_PORT,TEST_CS_PIN,"LAR"};
    //hspi2 = malloc(sizeof(SPI_HandleTypeDef));
    //hspi2->addr = (uint32_t*)0x40000000;
}
TEST_TEAR_DOWN(ADXL345Driver)
{

}
TEST(ADXL345Driver, DEVIDConnection)
{
    TEST_ASSERT_EQUAL(1,ADXL_CheckDevice(&Device,&FAKE_ENV));
}
TEST(ADXL345Driver, ReadDeviceProper)
{
    ADXL_ReadDevice(&Device,&FAKE_ENV);
    TEST_ASSERT_EQUAL(2,Device.DATAX0);
}
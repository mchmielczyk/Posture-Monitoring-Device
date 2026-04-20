#include "Adxl345DriverTest.h"
TEST_GROUP(ADXL345Driver);
ADXL345Data Device;

TEST_SETUP(ADXL345Driver)
{
    Device = (ADXL345Data*){0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,LAR_CS_GPIO_Port,LAR_CS_Pin,"LAR"};
}
TEST_TEAR_DOWN(ADXL345Driver)
{

}
TEST(ADXL345Driver, DEVIDConnection)
{
    uint8_t res = ADXL_CheckDevice(&Device);
    TEST_ASSERT_EQUAL_HEX(0xE5,res);
}
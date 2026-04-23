#include "Adxl345DriverTest.h"

TEST_GROUP_RUNNER(ADXL345Driver)
{
    RUN_TEST_CASE(ADXL345Driver, DEVIDConnection);
    RUN_TEST_CASE(ADXL345Driver, ReadDeviceProper);
}
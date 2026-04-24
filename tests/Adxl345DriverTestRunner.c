#include "Adxl345DriverTest.h"

TEST_GROUP_RUNNER(ADXL345Driver)
{
    RUN_TEST_CASE(ADXL345Driver, DEVIDConnection);
    RUN_TEST_CASE(ADXL345Driver, ReadSingleDeviceProperSingleRead);
    RUN_TEST_CASE(ADXL345Driver, ReturnStatusSingleRead);
    RUN_TEST_CASE(ADXL345Driver, NoDeviceOrInterfaceSingleRead);
    RUN_TEST_CASE(ADXL345Driver, ReadSingleDeviceProperMultiRead);
}
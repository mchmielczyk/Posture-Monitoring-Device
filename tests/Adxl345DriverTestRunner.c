#include "Adxl345DriverTest.h"

TEST_GROUP_RUNNER(ADXL345Driver)
{
    RUN_TEST_CASE(ADXL345Driver, DEVIDConnection);
    RUN_TEST_CASE(ADXL345Driver, ReadSingleDeviceProperSingleRead);
    RUN_TEST_CASE(ADXL345Driver, ReturnStatusSingleRead);
    RUN_TEST_CASE(ADXL345Driver, NoDeviceOrInterfaceSingleRead);
    RUN_TEST_CASE(ADXL345Driver, ReadSingleDeviceProperMultiRead);
    RUN_TEST_CASE(ADXL345Driver, ReturnStatusMultiRead);
    RUN_TEST_CASE(ADXL345Driver, NoDeviceOrInterfaceMultiRead);
    RUN_TEST_CASE(ADXL345Driver, SetRangeBoundaryCond);
    RUN_TEST_CASE(ADXL345Driver, DeviceDumpProper);
    RUN_TEST_CASE(ADXL345Driver, DeviceDumpBoundaryTooSmallBuffer);
    RUN_TEST_CASE(ADXL345Driver, DeviceDumpBoundaryWrongDevice);
    RUN_TEST_CASE(ADXL345Driver, DeviceDumpBoundaryWrongInterface);
    RUN_TEST_CASE(ADXL345Driver, SetFullResolutionProper);
    RUN_TEST_CASE(ADXL345Driver, SetFullResolutionBoundaryWrongInterface);
    RUN_TEST_CASE(ADXL345Driver, SetJustifyProper)
    RUN_TEST_CASE(ADXL345Driver, SetJustifyBoundaryWrongInterface)
    RUN_TEST_CASE(ADXL345Driver, SetJustifyBoundaryWrongMode)
    RUN_TEST_CASE(ADXL345Driver, SetMeasureProper)
    RUN_TEST_CASE(ADXL345Driver, SetMeasureBoundaryWrongInterface)
    RUN_TEST_CASE(ADXL345Driver, SetMeasureBoundaryWrongMode)
}
#include "Adxl345DriverTest.h"

TEST_GROUP_RUNNER(ADXL345DRIVER)
{
    RUN_TEST_CASE(ADXL345DRIVER, DEVIDConnection);
    RUN_TEST_CASE(ADXL345DRIVER, ReadSingleDeviceProperSingleRead);
    RUN_TEST_CASE(ADXL345DRIVER, ReturnStatusSingleRead);
    RUN_TEST_CASE(ADXL345DRIVER, NoDeviceOrInterfaceSingleRead);
    RUN_TEST_CASE(ADXL345DRIVER, ReadSingleDeviceProperMultiRead);
    RUN_TEST_CASE(ADXL345DRIVER, ReturnStatusMultiRead);
    RUN_TEST_CASE(ADXL345DRIVER, NoDeviceOrInterfaceMultiRead);
    RUN_TEST_CASE(ADXL345DRIVER, SetRangeBoundaryCond);
    RUN_TEST_CASE(ADXL345DRIVER, DeviceDumpProper);
    RUN_TEST_CASE(ADXL345DRIVER, DeviceDumpBoundaryTooSmallBuffer);
    RUN_TEST_CASE(ADXL345DRIVER, DeviceDumpBoundaryWrongDevice);
    RUN_TEST_CASE(ADXL345DRIVER, DeviceDumpBoundaryWrongInterface);
    RUN_TEST_CASE(ADXL345DRIVER, SetFullResolutionProper);
    RUN_TEST_CASE(ADXL345DRIVER, SetFullResolutionBoundaryWrongInterface);
    RUN_TEST_CASE(ADXL345DRIVER, SetJustifyProper)
    RUN_TEST_CASE(ADXL345DRIVER, SetJustifyBoundaryWrongInterface)
    RUN_TEST_CASE(ADXL345DRIVER, SetJustifyBoundaryWrongMode)
    RUN_TEST_CASE(ADXL345DRIVER, SetMeasureProper)
    RUN_TEST_CASE(ADXL345DRIVER, SetMeasureBoundaryWrongInterface)
    RUN_TEST_CASE(ADXL345DRIVER, SetMeasureBoundaryWrongMode)
    RUN_TEST_CASE(ADXL345DRIVER, SetOffsetProper)
    RUN_TEST_CASE(ADXL345DRIVER, SetOffsetBoundaryWrongInterface)
    RUN_TEST_CASE(ADXL345DRIVER, SetOffsetBoundaryWrongData)
}
#ifndef _H_ADXL345DRIVERTEST_
#define _H_ADXL345DRIVERTEST_
#include "../cpputest/include/CppUTest/TestHarness.h"
#include "../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../cpputest/include/CppUTestExt/MockSupport_c.h"
#include "../cpputest/include/CppUTestExt/MockSupport.h"
#include <stdexcept>
extern "C"
{
#include "../../Core/Inc/adxl345.h"
#include "../spies/adxl345spy.h"
}
void TEST_CheckSpyLastReadAndWrite(uint8_t lastRead, uint8_t lastWrite);
#endif //!_H_ADXL345DRIVERTEST_
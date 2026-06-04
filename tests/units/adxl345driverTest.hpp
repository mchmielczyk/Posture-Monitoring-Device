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
#endif //!_H_ADXL345DRIVERTEST_
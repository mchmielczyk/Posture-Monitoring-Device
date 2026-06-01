#include "Adxl345DriverTest.hpp"
extern "C"
{
#include "Mocks/adxl345spy.h"
#include "Mocks/fake_hal.h"
}
TEST_GROUP(ADXL345Spy){void setup(){ADXL345Spy_Create();
}
void teardown()
{
}
}
;
TEST(ADXL345Spy, Create)
{
    LONGS_EQUAL((uint8_t)-1, ADXL345Spy_GetLastRead());
    LONGS_EQUAL((uint8_t)-1, ADXL345Spy_GetLastRead());
}
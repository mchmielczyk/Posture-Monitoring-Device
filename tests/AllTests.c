#include "Adxl345DriverTest.h"
static void RunAllTests(void)
{
    RUN_TEST_GROUP(ADXL345Driver);
}
int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
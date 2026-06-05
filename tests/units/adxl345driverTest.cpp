#include "adxl345driverTest.hpp"

#define FAKE_PORT 0x20
#define FAKE_PIN 0x10

extern ADXL345Interface* getFakeInterface();
extern ADXL345Interface* getMockInterface();

void TEST_CheckSpyLastReadAndWrite(uint8_t lastRead, uint8_t lastWrite)
{
    CHECK_EQUAL(uint8_t(-1), adxl345spy_GetLastRead());
    CHECK_EQUAL(uint8_t(-1), adxl345spy_GetLastWrite());
}

ADXL345Data FAKE_Dev;

ADXL345Driver FAKE_Driver = {.iface = getFakeInterface(), .device = &FAKE_Dev, .id = 0};

ADXL345Driver MOCK_Driver = {.iface = getMockInterface(), .device = &FAKE_Dev, .id = 0};

ADXL345Driver WRONG_Device_FAKE_Driver = {.iface = getFakeInterface(), .device = NULL, .id = 0};

ADXL345Driver WRONG_Interface_FAKE_Driver = {.iface = NULL, .device = &FAKE_Dev, .id = 0};

ADXL345Interface NoReadWriteInterface = {.write = NULL, .read = NULL};

ADXL345Driver WRONG_InterfaceNoRead_FAKE_Driver = {
    .iface = &NoReadWriteInterface, .device = &FAKE_Dev, .id = 0};

ADXL345Driver WRONG_InterfaceNoWrite_FAKE_Driver = {
    .iface = &NoReadWriteInterface, .device = &FAKE_Dev, .id = 0};

TEST_GROUP(ADXL345DRIVERMOCK){void setup(){
    FAKE_Dev = (ADXL345Data){0, 0, 0, 0, 0, 0, 0, 0, 0, (void*)FAKE_PORT, FAKE_PIN, "LAR"};
}
void teardown()
{
    mock().checkExpectations();
    mock().clear();
}
}
;
TEST(ADXL345DRIVERMOCK, ADXL_CheckDevice_OnValidStructure_ReturnOK)
{
    uint8_t devidCode[2] = {0, ADXL345_DEVID_correct_code};
    mock()
        .expectOneCall("read")
        .withParameter("tx", ADXL345_DEVID | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &devidCode, sizeof(devidCode))
        .withParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    ADXL345_Status result = ADXL_CheckDevice(&MOCK_Driver);

    CHECK_EQUAL(ADXL345_OK, result);
}
TEST(ADXL345DRIVERMOCK, ADXL_CheckDevice_OnNoConnection_ReturnError)
{
    uint8_t devidCode[2] = {0, ADXL345_DEVID_correct_code};
    mock()
        .expectOneCall("read")
        .withParameter("tx", ADXL345_DEVID | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &devidCode, sizeof(devidCode))
        .withParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);
    ADXL345_Status result = ADXL_CheckDevice(&MOCK_Driver);

    CHECK_EQUAL(ADXL345_ERROR, result);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetMeasure_OnCorrectStructure_ReturnOk)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_POWER_CTL, ADXL345_SET};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_POWER_CTL | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);

    ADXL345_Status actual = ADXL_SetMeasure(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_OK, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetMeasure_OnLostDeviceConnection_ReturnError)
{
    uint8_t receive[2] = {0};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_POWER_CTL | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("write");

    ADXL345_Status actual = ADXL_SetMeasure(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetMeasure_OnLostDeviceConnectionDuring_ReturnError)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_POWER_CTL, ADXL345_SET};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_POWER_CTL | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);

    ADXL345_Status actual = ADXL_SetMeasure(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetRange_OnCorrectStructure_ReturnOk)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_DATA_FORMAT, ADXL345_SET};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);

    ADXL345_Status actual = ADXL_SetRange(&MOCK_Driver, RANGE_16G);

    CHECK_EQUAL(ADXL345_OK, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetRange_OnLostDeviceConnection_ReturnError)
{
    uint8_t receive[2] = {0};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("write");

    ADXL345_Status actual = ADXL_SetRange(&MOCK_Driver, RANGE_16G);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetRange_OnLostDeviceConnectionDuring_ReturnError)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_DATA_FORMAT, RANGE_16G};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);

    ADXL345_Status actual = ADXL_SetRange(&MOCK_Driver, RANGE_16G);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetFullResolution_OnCorrectStructure_ReturnOk)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_DATA_FORMAT, ADXL345_SET};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);

    ADXL345_Status actual = ADXL_SetFullResolution(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_OK, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetFullResolution_OnLostDeviceConnection_ReturnError)
{
    uint8_t receive[2] = {0};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("write");

    ADXL345_Status actual = ADXL_SetFullResolution(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetFullResolution_OnLostDeviceConnectionDuring_ReturnError)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_DATA_FORMAT, ADXL345_SET};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);

    ADXL345_Status actual = ADXL_SetFullResolution(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetJustify_OnCorrectStructure_ReturnOk)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_DATA_FORMAT, ADXL345_SET};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);

    ADXL345_Status actual = ADXL_SetJustify(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_OK, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetJustify_OnLostDeviceConnection_ReturnError)
{
    uint8_t receive[2] = {0};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("write");

    ADXL345_Status actual = ADXL_SetJustify(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetJustify_OnLostDeviceConnectionDuring_ReturnError)
{
    uint8_t receive[2] = {0};
    uint8_t transfer[2] = {ADXL345_DATA_FORMAT, ADXL345_SET};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATA_FORMAT | ADXL345_SINGLE_BYTE_READ)
        .withOutputParameterReturning("rx", &receive, sizeof(receive))
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfer)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);

    ADXL345_Status actual = ADXL_SetJustify(&MOCK_Driver, ADXL345_SET);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetOffset_OnCorrectStructure_ReturnOk)
{
    uint8_t transferx[2] = {ADXL345_OFSX, 255};
    uint8_t transfery[2] = {ADXL345_OFSY, 255};
    uint8_t transferz[2] = {ADXL345_OFSZ, 255};
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transferx)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfery)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transferz)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);

    ADXL345_Status actual = ADXL_SetOffset(&MOCK_Driver, 255, 255, 255);

    CHECK_EQUAL(ADXL345_OK, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_SetOffset_OnNoConnection_ReturnError)
{
    uint8_t transferx[2] = {ADXL345_OFSX, 255};
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transferx)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("write");

    ADXL345_Status actual = ADXL_SetOffset(&MOCK_Driver, 255, 255, 255);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}

TEST(ADXL345DRIVERMOCK, ADXL_SetOffset_OnNoConnectionDuring_ReturnError)
{
    uint8_t transferx[2] = {ADXL345_OFSX, 255};
    uint8_t transfery[2] = {ADXL345_OFSY, 255};

    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transferx)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("write")
        .withIntParameter("tx", *transfery)
        .withIntParameter("size", 2)
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("write");

    ADXL345_Status actual = ADXL_SetOffset(&MOCK_Driver, 255, 255, 255);

    CHECK_EQUAL(ADXL345_ERROR, actual);
}
TEST(ADXL345DRIVERMOCK, ADXL_ReadDevice_OnProperStructure_ReturnOk)
{
    uint8_t receivex0[2] = {0, 2};
    uint8_t receivex1[2] = {0, 4};
    uint8_t receivey0[2] = {0, 8};
    uint8_t receivey1[2] = {0, 16};
    uint8_t receivez0[2] = {0, 32};
    uint8_t receivez1[2] = {0, 64};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAX0 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivex0, sizeof(receivex0))
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAX1 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivex1, sizeof(receivex1))
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAY0 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivey0, sizeof(receivey0))
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAY1 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivey1, sizeof(receivey1))
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAZ0 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivez0, sizeof(receivez0))
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAZ1 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivez1, sizeof(receivez1))
        .andReturnValue(ADXL345_OK);

    uint8_t result = ADXL_ReadDevice(&MOCK_Driver);

    CHECK_EQUAL(ADXL345_OK, result);
}
TEST(ADXL345DRIVERMOCK, ADXL_ReadDevice_OnLostConnection_ReturnError)
{
    uint8_t receivex0[2] = {0, 2};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAX0 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivex0, sizeof(receivex0))
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("read");

    uint8_t result = ADXL_ReadDevice(&MOCK_Driver);

    CHECK_EQUAL(ADXL345_ERROR, result);
}

TEST(ADXL345DRIVERMOCK, ADXL_ReadDevice_OnLostConnectionDuring_ReturnError)
{
    uint8_t receivex0[2] = {0, 2};
    uint8_t receivex1[2] = {0, 4};
    uint8_t receivey0[2] = {0, 8};
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAX0 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivex0, sizeof(receivex0))
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAX1 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivex1, sizeof(receivex1))
        .andReturnValue(ADXL345_OK);
    mock()
        .expectOneCall("read")
        .withIntParameter("tx", ADXL345_DATAY0 | ADXL345_SINGLE_BYTE_READ)
        .withIntParameter("size", 2)
        .withOutputParameterReturning("rx", &receivey0, sizeof(receivey0))
        .andReturnValue(ADXL345_ERROR);
    mock().expectNoCall("read");

    uint8_t result = ADXL_ReadDevice(&MOCK_Driver);

    CHECK_EQUAL(ADXL345_ERROR, result);
}
TEST(ADXL345DRIVERMOCK, ADXL_MultiReadDevice_OnProperConnection_ReturnOk)
{
    uint8_t tx = ADXL345_MULTI_BYTE_READ | ADXL345_DATAX0;

    uint8_t rx[7U] = {0U, 2U, 4U, 8U, 16U, 32U, 64U};

    mock()
        .expectOneCall("read")
        .withIntParameter("tx", tx)
        .withOutputParameterReturning("rx", rx, sizeof(rx))
        .withIntParameter("size", 7)
        .andReturnValue(ADXL345_OK);

    uint8_t result = ADXL_MultiReadDevice(&MOCK_Driver);

    CHECK_EQUAL(ADXL345_OK, result);

    CHECK_EQUAL(1026, FAKE_Driver.device->DATAX);
    CHECK_EQUAL(4104, FAKE_Driver.device->DATAY);
    CHECK_EQUAL(16416, FAKE_Driver.device->DATAZ);
}

TEST(ADXL345DRIVERMOCK, ADXL_MultiReadDevice_OnNoConnection_ReturnError)
{
    uint8_t tx = ADXL345_MULTI_BYTE_READ | ADXL345_DATAX0;

    uint8_t rx[7U] = {0U, 2U, 4U, 8U, 16U, 32U, 64U};

    mock()
        .expectOneCall("read")
        .withIntParameter("tx", tx)
        .withOutputParameterReturning("rx", rx, sizeof(rx))
        .withIntParameter("size", 7)
        .andReturnValue(ADXL345_ERROR);

    uint8_t result = ADXL_MultiReadDevice(&MOCK_Driver);

    CHECK_EQUAL(ADXL345_ERROR, result);
}

TEST_GROUP(ADXL345DRIVER){void setup(){
    FAKE_Dev = (ADXL345Data){0, 0, 0, 0, 0, 0, 0, 0, 0, (void*)FAKE_PORT, FAKE_PIN, "LAR"};
adxl345spy_Create();
}
void teardown()
{
}
}
;
TEST(ADXL345DRIVER, ADXL_CheckDevice_OnValidConnection_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_CheckDevice(&FAKE_Driver));
    CHECK_EQUAL((ADXL345_DEVID | ADXL345_SINGLE_BYTE_READ), adxl345spy_GetLastWrite());
    CHECK_EQUAL(ADXL345_DEVID_correct_code, adxl345spy_GetLastRead());
}
TEST(ADXL345DRIVER, ADXL_CheckDevice_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_CheckDevice(&WRONG_Device_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_CheckDevice_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_CheckDevice(&WRONG_Interface_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}

TEST(ADXL345DRIVER, ADXL_DeviceDump_OnValidDriverAndData_ReturnOK)
{
    char buffer[50] = {0};
    CHECK_EQUAL(ADXL345_OK, ADXL_DeviceDump(&FAKE_Driver, buffer, 50));
    STRCMP_EQUAL("LAR: X: 0, Y: 0, Z: 0\r\n", buffer);
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_DeviceDump_OnWrongDevice_ReturnError)
{
    char buffer[50] = {0};
    CHECK_EQUAL(ADXL345_ERROR, ADXL_DeviceDump(&WRONG_Device_FAKE_Driver, buffer, 50));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_DeviceDump_OnWrongInterface_ReturnError)
{
    char buffer[50] = {0};
    CHECK_EQUAL(ADXL345_ERROR, ADXL_DeviceDump(&WRONG_Interface_FAKE_Driver, buffer, 50));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_DeviceDump_OnTooSmallBuffer_ReturnError)
{
    char buffer[2] = {0};
    CHECK_EQUAL(ADXL345_ERROR, ADXL_DeviceDump(&FAKE_Driver, buffer, 2));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_DeviceDump_OnWrongBuffer_ReturnError)
{
    char* buffer = NULL;
    CHECK_EQUAL(ADXL345_ERROR, ADXL_DeviceDump(&FAKE_Driver, buffer, 2));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetMeasure_OnValidDriver_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetMeasure(&FAKE_Driver, ADXL345_SET));
}
TEST(ADXL345DRIVER, ADXL_SetMeasure_OnValidData_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetMeasure(&FAKE_Driver, ADXL345_SET));
    CHECK_EQUAL((ADXL345_POWER_CTL), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetMeasure_OnValidInputSet_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetMeasure(&FAKE_Driver, ADXL345_SET));
    CHECK_EQUAL((ADXL345_POWER_CTL), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetMeasure_OnValidInputReset_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetMeasure(&FAKE_Driver, ADXL345_RESET));
    CHECK_EQUAL((ADXL345_POWER_CTL), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetMeasure_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetMeasure(&WRONG_Device_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetMeasure_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetMeasure(&WRONG_Interface_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetMeasure_OnWrongInterfaceNoWrite_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetMeasure(&WRONG_InterfaceNoWrite_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnValidDriver_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetRange(&FAKE_Driver, RANGE_16G));
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnValidData_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetRange(&FAKE_Driver, RANGE_16G));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnValidInput2G_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetRange(&FAKE_Driver, RANGE_2G));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnValidInput4G_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetRange(&FAKE_Driver, RANGE_4G));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnValidInpu8G_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetRange(&FAKE_Driver, RANGE_8G));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnValidInput16G_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetRange(&FAKE_Driver, RANGE_16G));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetRange(&WRONG_Device_FAKE_Driver, RANGE_16G));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetRange(&WRONG_Interface_FAKE_Driver, RANGE_16G));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnWrongRange_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetRange(&WRONG_Interface_FAKE_Driver, 50U));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetRange_OnWrongInterfaceNoWrite_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetRange(&WRONG_InterfaceNoWrite_FAKE_Driver, RANGE_16G));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetFullResolution_OnValidDriver_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetFullResolution(&FAKE_Driver, ADXL345_SET));
}
TEST(ADXL345DRIVER, ADXL_SetFullResolution_OnValidData_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetFullResolution(&FAKE_Driver, ADXL345_SET));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetFullResolution_OnValidInputSet_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetFullResolution(&FAKE_Driver, ADXL345_SET));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetFullResolution_OnValidInputReset_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetFullResolution(&FAKE_Driver, ADXL345_RESET));
    CHECK_EQUAL((ADXL345_DATA_FORMAT), adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetFullResolution_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetFullResolution(&WRONG_Device_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetFullResolution_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetFullResolution(&WRONG_Interface_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetFullResolution_OnWrongInterfaceNoWrite_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR,
                ADXL_SetFullResolution(&WRONG_InterfaceNoWrite_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetJustify_OnValidDriver_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetJustify(&FAKE_Driver, ADXL345_SET));
}
TEST(ADXL345DRIVER, ADXL_SetJustify_OnValidData_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetJustify(&FAKE_Driver, ADXL345_SET));
    CHECK_EQUAL(ADXL345_DATA_FORMAT, adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetJustify_OnValidInputSet_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetJustify(&FAKE_Driver, ADXL345_SET));
}
TEST(ADXL345DRIVER, ADXL_SetJustify_OnValidInputReset_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetJustify(&FAKE_Driver, ADXL345_RESET));
}
TEST(ADXL345DRIVER, ADXL_SetJustify_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetJustify(&WRONG_Device_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetJustify_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetJustify(&WRONG_Interface_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetJustify_OnWrongInterfaceNoWrite_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetJustify(&WRONG_InterfaceNoWrite_FAKE_Driver, ADXL345_SET));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetOffset_OnValidDriver_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetOffset(&FAKE_Driver, 255, 255, 255));
    CHECK_EQUAL(ADXL345_OFSZ, adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetOffset_OnValidData_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetOffset(&FAKE_Driver, 255, 255, 255));
    CHECK_EQUAL(ADXL345_OFSZ, adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetOffset_OnValidInputLowOffset_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetOffset(&FAKE_Driver, 0, 0, 0));
    CHECK_EQUAL(ADXL345_OFSZ, adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetOffset_OnValidInputHighOffset_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_SetOffset(&FAKE_Driver, 255, 255, 255));
    CHECK_EQUAL(ADXL345_OFSZ, adxl345spy_GetLastWrite());
}
TEST(ADXL345DRIVER, ADXL_SetOffset_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetOffset(&WRONG_Device_FAKE_Driver, 255, 255, 255));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetOffset_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetOffset(&WRONG_Interface_FAKE_Driver, 255, 255, 255));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_SetOffset_OnWrongInterfaceNoWrite_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_SetOffset(&WRONG_InterfaceNoWrite_FAKE_Driver, 255, 255, 255));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_ReadDevice_OnProperDriver_ReturnOk)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_ReadDevice(&FAKE_Driver));
}
TEST(ADXL345DRIVER, ADXL_ReadDevice_OnValidData_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_ReadDevice(&FAKE_Driver));
    CHECK_EQUAL(ADXL345_DATAZ1 | ADXL345_SINGLE_BYTE_READ, adxl345spy_GetLastWrite());
    CHECK_EQUAL(64, adxl345spy_GetLastRead());
}
TEST(ADXL345DRIVER, ADXL_ReadDevice_OnProperDriver_ReturnAllAxisRead)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_ReadDevice(&FAKE_Driver));
    CHECK_EQUAL(2, FAKE_Driver.device->DATAX0);
    CHECK_EQUAL(4, FAKE_Driver.device->DATAX1);
    CHECK_EQUAL(8, FAKE_Driver.device->DATAY0);
    CHECK_EQUAL(16, FAKE_Driver.device->DATAY1);
    CHECK_EQUAL(32, FAKE_Driver.device->DATAZ0);
    CHECK_EQUAL(64, FAKE_Driver.device->DATAZ1);
    CHECK_EQUAL(ADXL345_DATAZ1 | ADXL345_SINGLE_BYTE_READ, adxl345spy_GetLastWrite());
    CHECK_EQUAL(64, adxl345spy_GetLastRead());
}
TEST(ADXL345DRIVER, ADXL_ReadDevice_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_ReadDevice(&WRONG_Device_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_ReadDevice_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_ReadDevice(&WRONG_Interface_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_ReadDevice_OnWrongInterfaceNoRead_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_ReadDevice(&WRONG_InterfaceNoRead_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_MultiReadDevice_OnProperDriver_ReturnAllAxisRead)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_MultiReadDevice(&FAKE_Driver));
    CHECK_EQUAL(1026, FAKE_Driver.device->DATAX);
    CHECK_EQUAL(4104, FAKE_Driver.device->DATAY);
    CHECK_EQUAL(16416, FAKE_Driver.device->DATAZ);
}
TEST(ADXL345DRIVER, ADXL_MultiReadDevice_OnValidData_ReturnOK)
{
    CHECK_EQUAL(ADXL345_OK, ADXL_MultiReadDevice(&FAKE_Driver));
    CHECK_EQUAL(ADXL345_DATAX0 | ADXL345_MULTI_BYTE_READ, adxl345spy_GetLastWrite());
    CHECK_EQUAL(64, adxl345spy_GetLastRead());
}
TEST(ADXL345DRIVER, ADXL_MultiReadDevice_OnWrongDevice_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_MultiReadDevice(&WRONG_Device_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_MultiReadDevice_OnWrongDriver_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_MultiReadDevice((ADXL345Driver*)0));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_MultiReadDevice_OnWrongInterface_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_MultiReadDevice(&WRONG_Interface_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
TEST(ADXL345DRIVER, ADXL_MultiReadDevice_OnWrongInterfaceNoRead_ReturnError)
{
    CHECK_EQUAL(ADXL345_ERROR, ADXL_MultiReadDevice(&WRONG_InterfaceNoRead_FAKE_Driver));
    TEST_CheckSpyLastReadAndWrite(-1, -1);
}
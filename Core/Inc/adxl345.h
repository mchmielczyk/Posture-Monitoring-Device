/*
 * adxl345.h
 *
 *  Created on: Nov 25, 2025
 *      Author: Michał Chmielczyk
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include <stdint.h>
#include <stdio.h>

#define ADXL345_DEVID 0x00U          ///< Device ID
#define ADXL345_THRESH_TAP 0x1DU     ///< Tap threshold
#define ADXL345_OFSX 0x1EU           ///< X-axis offset
#define ADXL345_OFSY 0x1FU           ///< Y-axis offset
#define ADXL345_OFSZ 0x20U           ///< Z-axis offset
#define ADXL345_DUR 0x21U            ///< Tap duration
#define ADXL345_Latent 0x22U         ///< Tap latency
#define ADXL345_Window 0x23U         ///< Tap window
#define ADXL345_THRESH_ACT 0x24U     ///< Activity threshold
#define ADXL345_THRESH_INACT 0x25U   ///< Inactivity threshold
#define ADXL345_TIME_INACT 0x26U     ///< Inactivity time
#define ADXL345_ACT_INACT_CTL 0x27U  ///< Axis enable control for activity and inactivity detection
#define ADXL345_THRESH_FF 0x28U      ///< Free-fall threshold
#define ADXL345_TIME_FF 0x29U        ///< Free-fall time
#define ADXL345_TAP_AXES 0x2AU       ///< Axis control for tap/double tap
#define ADXL345_ACT_TAP_STATUS 0x2BU ///< Source of tap/double tap
#define ADXL345_BW_RATE 0x2CU        ///< Data rate and power mode control
#define ADXL345_POWER_CTL 0x2DU      ///< Power-saving features control
#define ADXL345_INT_ENABLE 0x2EU     ///< Interrupt enable control
#define ADXL345_INT_MAP 0x2FU        ///< Interrupt mapping control
#define ADXL345_INT_SOURCE 0x30U     ///< Source of interrupts
#define ADXL345_DATA_FORMAT 0x31U    ///< Data format control
#define ADXL345_DATAX0 0x32U         ///< X-Axis Data 0
#define ADXL345_DATAX1 0x33U         ///< X-Axis Data 1
#define ADXL345_DATAY0 0x34U         ///< Y-Axis Data 0
#define ADXL345_DATAY1 0x35U         ///< Y-Axis Data 1
#define ADXL345_DATAZ0 0x36U         ///< Z-Axis Data 0
#define ADXL345_DATAZ1 0x37U         ///< Z-Axis Data 1
#define ADXL345_FIFO_CTL 0x38U       ///< FIFO control
#define ADXL345_FIFO_STATUS 0x39U    ///< FIFO status

#define RANGE_2G 0x00U  ///< +-2g Range
#define RANGE_4G 0x01U  ///< +-4g Range
#define RANGE_8G 0x02U  ///< +-8g Range
#define RANGE_16G 0x03U ///< +-16g Range

#define ADXL345_SET 0x01U   ///< High
#define ADXL345_RESET 0x00U ///< Low

#define ADXL345_SINGLE_BYTE_READ (0x01U << 7U)
#define ADXL345_MULTI_BYTE_READ ((0x01U << 7U) | (0x01U << 6U))

#define ADXL345_POWER_CTL_measure_bit (0x01U << 3U)    ///< POWER_CTL measure bit
#define ADXL345_DATA_FORMAT_range_bits 0x03U           ///< ADXL345_DATA_FORMAT range bits
#define ADXL345_DATA_FORMAT_full_res_bit (0x01U << 3U) ///< ADXL345_DATA_FORMAT FULL_RES bit
#define ADXL345_DATA_FORMAT_justify_bit (0x01U << 2U)  ///< ADXL345_DATA_FORMAT justify bit

#define ADXL345_DEVID_correct_code 0xE5U ///< ADXL345_DEVID correct device id code

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        ADXL345_OK = 0x00U,
        ADXL345_ERROR = 0x01U,
        ADXL345_BUSY = 0x02U,
        ADXL345_TIMEOUT = 0x03U
    } ADXL345_Status;

    struct ADXL345Driver;

    typedef ADXL345_Status (*write_fn)(const struct ADXL345Driver* driver,
                                       uint8_t* tx,
                                       uint16_t size);
    typedef ADXL345_Status (*read_fn)(const struct ADXL345Driver* driver,
                                      uint8_t* tx,
                                      uint8_t* rx,
                                      uint16_t size);
    typedef void (*ERR_RuntimeError)(const char* message,
                                     int parameter,
                                     const char* file,
                                     int line);

    typedef struct
    {
        write_fn write;
        read_fn read;
        ERR_RuntimeError err_runtime;
    } ADXL345Interface;

    typedef struct
    {
        uint8_t DATAX0;
        uint8_t DATAX1;
        uint8_t DATAY0;
        uint8_t DATAY1;
        uint8_t DATAZ0;
        uint8_t DATAZ1;
        uint16_t DATAX;
        uint16_t DATAY;
        uint16_t DATAZ;
        void* PORT;
        uint16_t PIN;
        char name[4U];
    } ADXL345Data;

    typedef struct ADXL345Driver
    {
        ADXL345Interface* iface;
        ADXL345Data* device;
        uint8_t id;
    } ADXL345Driver;

    ADXL345_Status ADXL_ReadDevice(ADXL345Driver* Driver);
    ADXL345_Status ADXL_SetMeasure(ADXL345Driver* Driver, uint8_t mode);
    ADXL345_Status ADXL_DeviceDump(ADXL345Driver* Driver, char* Dest, size_t Size);
    ADXL345_Status ADXL_SetRange(ADXL345Driver* Driver, uint8_t Range);
    ADXL345_Status ADXL_SetFullResolution(ADXL345Driver* Driver, uint8_t mode);
    ADXL345_Status ADXL_SetJustify(ADXL345Driver* Driver, uint8_t mode);
    ADXL345_Status ADXL_CheckDevice(ADXL345Driver* Driver);
    ADXL345_Status ADXL_SetOffset(ADXL345Driver* Driver, uint8_t offX, uint8_t offY, uint8_t offZ);
    ADXL345_Status ADXL_MultiReadDevice(ADXL345Driver* Driver);

#ifdef __cplusplus
}
#endif

#endif /* INC_ADXL345_H_ */

#ifndef _H_ADXL345SPY_
#define _H_ADXL345SPY_
#include "../../Core/Inc/adxl345.h"
#ifdef __cplusplus
extern "C"
{
#endif

    enum
    {
        LAST_READ_UNKNOWN = -1,
        LAST_WRITE_UNKNOWN = -1,
    };

    void adxl345spy_Create(void);
    uint8_t adxl345spy_GetLastRead(void);
    uint8_t adxl345spy_GetLastWrite(void);
    void adxl345spy_SetLastRead(uint8_t read);
    void adxl345spy_SetLastWrite(uint8_t write);

#ifdef __cplusplus
}
#endif
#endif
typedef struct{} FAKE_Status_TypeDef;
typedef struct{} FAKE_SPI_Handle;

void FAKE_HAL_GPIO_WritePin(void* PORT, uint16_t PIN, int state){}
FAKE_Status_TypeDef FAKE_HAL_SPI_Transmit_DMA(FAKE_SPI_Handle *hspi,uint8_t* tx,uint16_t* size){}
FAKE_Status_TypeDef FAKE_HAL_SPI_TransmitReceive_DMA(FAKE_SPI_Handle *hspi,uint8_t* tx,uint8_t* rx,uint16_t* size){}
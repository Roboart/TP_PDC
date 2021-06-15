#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"

#define SAMPLE_RATE (44100)
#define PIN_I2S_BCLK 26
#define PIN_I2S_LRC 27
#define PIN_I2S_DOUT 25
#define NOT_USED 0
#define I2S_PORT_0 0
#define I2S_PORT_1 1


// 44100Hz, 16bit, stereo
void I2S_Init();
void I2S_Write(char* buffer,int sizeBuffer);
void I2S_Uninstall();

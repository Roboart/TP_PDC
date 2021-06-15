#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <newI2S.h>
#include "driver/i2s.h"
#include "freertos/queue.h"

#define MONO 1
#define STEREO 2
#define POS_INI_DATA_AUDIO 44 //
#define I2S_BUFFER 800
#define READ_CHANNEL_POSITION 22 //

#define I2S_PORT_0 0

char data[I2S_BUFFER]; 
char stereoData[I2S_BUFFER * STEREO]; //para convertirlo a Stereo si el archivo es mono.


void setup() {
  //Inicialización SPI SD CARD
  SD.begin();
  /**********************************************************************************/
  I2S_Init();
  /**********************************************************************************/
  
  /**********************************************************************************/
  File file = SD.open("/0000stereo.wav");  // 44100Hz, 16bit, linear PCM
  /**********************************************************************************/
  file.seek(READ_CHANNEL_POSITION);
  int numChannel = file.read();
  
  /**********************************************************************************/
  file.seek(POS_INI_DATA_AUDIO); 
  while (file.readBytes(data, sizeof(data))) {
    if (numChannel == STEREO){
      I2S_Write(data, sizeof(data));
    } 
    else if (numChannel == MONO) {
      for (int i = 0; i < sizeof(data); ++i) 
      {
        stereoData[4 * (i / 2) + i % 2] = data[i];// cargo en 0,1 - 4,5 para cargar los 16 bit del muestreo
        stereoData[4 * (i / 2) + i % 2 + 2] = data[i];// cargo en 2,3 - 6,7 para cargar los 16 bit del muestreo 
      }
      I2S_Write(stereoData, sizeof(stereoData));
    }
  }
  /**********************************************************************************/
  file.close();
  /**********************************************************************************/
  

  vTaskDelay(1000);


  file = SD.open("/0001mono.wav");  // 44100Hz, 16bit, linear PCM
  file.seek(READ_CHANNEL_POSITION);
  numChannel = file.read();
  file.seek(POS_INI_DATA_AUDIO);
  while (file.readBytes(data, sizeof(data))) {
    if (numChannel == STEREO){
      I2S_Write(data, sizeof(data));
    } 
    else if (numChannel == MONO) {
      for (int i = 0; i < sizeof(data); ++i) 
      {
        stereoData[4 * (i / 2) + i % 2] = data[i];// cargo en 0,1 - 4,5 para cargar los 16 bit del muestreo
        stereoData[4 * (i / 2) + i % 2 + 2] = data[i];// cargo en 2,3 - 6,7 para cargar los 16 bit del muestreo 
      }
      I2S_Write(stereoData, sizeof(stereoData));
    }
  }
  file.close();
  //i2s_driver_uninstall(0); //stop & destroy i2s driver
  i2s_driver_uninstall((i2s_port_t)I2S_PORT_0); //stop & destroy i2s driver
}

void loop() {
}

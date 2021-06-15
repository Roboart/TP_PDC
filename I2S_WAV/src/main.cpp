#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <newI2S.h>
#include "driver/i2s.h"
#include "freertos/queue.h"

#define MONO 1
#define STEREO 2
#define POS_INI_DATA_AUDIO 44 // Posicion donde comienza los datos de audio
#define I2S_BUFFER 800
#define READ_CHANNEL_POSITION 22 //posición dentro del buffer que determina si el audio es mono o stereo


char data[I2S_BUFFER]; 
char stereoData[I2S_BUFFER * STEREO]; //para convertirlo a Stereo si el archivo es mono.


void setup() {
  //Inicialización SPI SD CARD
  SD.begin();
  /**********************************************************************************/
  // Inicialización Protocolo I2S
  I2S_Init();
  /**********************************************************************************/
  
  /**********************************************************************************/
  File file = SD.open("/0000stereo.wav");  // 44100Hz, 16bit, linear PCM
  /**********************************************************************************/
  file.seek(READ_CHANNEL_POSITION); //Saltamo a la posición de memoria que contiene
  int numChannel = file.read();     // el byte para saber si es mono o estero
  
  /**********************************************************************************/
  file.seek(POS_INI_DATA_AUDIO); // Salatamos a la posición donde comienzan los datos de audio
  while (file.readBytes(data, sizeof(data))) {
    if (numChannel == STEREO){
      I2S_Write(data, sizeof(data));
    } 
    else if (numChannel == MONO) {
  /**********************************************************************************/
  //Código con fines didacticos para explicar como construir un archivo mono en stereo  
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
  

  vTaskDelay(1000); //Espero 1 segundo y reproduzco un archivo mono.


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
  }

void loop() {
}

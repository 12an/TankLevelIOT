#include "sensor.h"


#define FRAME_SIZE 11
#define AVG_SIZE 100

//MEDIDAS
uint8_t frame[FRAME_SIZE];
uint16_t samples[AVG_SIZE];
uint32_t sum = 0;
uint8_t indexAvg = 0;
bool bufferFull = false;


void TaskLevelFollow(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  TaskTimerFolow
  folow the time the counting time. and activate diferents events.
*/
  Serial2.begin(115200, SERIAL_8N1, 17, 16);
  static uint8_t indexFrame = 0;

  for (;;)
  {
    
    while (Serial2.available()) {
      uint8_t b = Serial2.read();

      // sincronizar frame
      if (indexFrame == 0 && b != 0xDF)
        continue;

      frame[indexFrame++] = b;

      if (indexFrame == FRAME_SIZE) {

        uint16_t distance =
          frame[6] | (frame[7] << 8);

        // ---- PROMEDIO MOVIL ----

        sum -= samples[indexAvg];     // quitar viejo
        samples[indexAvg] = distance; // poner nuevo
        sum += distance;              // sumar nuevo

        indexAvg++;

        if (indexAvg >= AVG_SIZE) {
          indexAvg = 0;
          bufferFull = true;
        }

        uint16_t average;

        if (bufferFull){
          average = sum / AVG_SIZE;
        }
        else{
          average = sum / indexAvg;
        }
        indexFrame = 0;
        if (xSemaphoreTake(TankMutex, 0 ) == pdTRUE){
          current_galons = average;
          xSemaphoreGive( TankMutex );
        }
      }
    }
  }
}
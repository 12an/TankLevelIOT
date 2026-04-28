#include <math.h>
#include "Model_Data.h"
#include "VolumenMath.h"

#define PI 3.1415926535897932384626433832795

void TaskUpdateVolumen(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  TaskNotificationLed
  If we have the time up to specific amount, we need to create a notification 
  based in a led. that well go outside.
*/
  float A_H = 0;
  for (;;) // A Task shall never return or exit.
  {
    if (xSemaphoreTake(TankMutex, ( TickType_t ) 10 ) == pdTRUE){
        if (ORIENTACION_TANQUE){
          current_galons = (PI * sensor_mm_read * pow((DIAMETRO_TANQUE/2), 2));
          current_galons = current_galons + osffet_vertical_volumen;
        }else{
          A_H = pow((DIAMETRO_TANQUE/2), 2) * acos(((DIAMETRO_TANQUE/2) - sensor_mm_read)/(DIAMETRO_TANQUE/2)) - ((DIAMETRO_TANQUE/2) - sensor_mm_read) * sqrt((2*(DIAMETRO_TANQUE/2) * sensor_mm_read) - pow(sensor_mm_read, 2));
          current_galons = A_H * LARGO_TANQUE;
          
        }
        current_galons = current_galons / 3785410;
        if(ORIENTACION_TANQUE==0){
          current_galons = current_galons + osffet_horizontal_volumen;
        }
        if (current_galons<0){
          current_galons = 0;
        }
        xSemaphoreGive( TankMutex );
      }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
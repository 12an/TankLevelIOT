#include <Arduino.h>
#include "Model_Data.h"
#include "actuators.h"

void TaskNotificationLight(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

/*
  TaskNotificationLed
  If we have the time up to specific amount, we need to create a notification 
  based in a led. that well go outside.
*/

  // initialize digital notification on pin 27 as an output.
  pinMode(HIGHT_LEVEL_LIGHT, OUTPUT);
  pinMode(LOW_LEVEL_LIGHT, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    // the time had reached out we need to notifi
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for half second

    if (xSemaphoreTake(ActuatorsMutex, ( TickType_t ) 10 ) == pdTRUE){
      if(is_hight){
        digitalWrite(HIGHT_LEVEL_LIGHT, HIGH);
        digitalWrite(LOW_LEVEL_LIGHT, LOW);
      }
      if(is_low){
        digitalWrite(LOW_LEVEL_LIGHT, HIGH);
        digitalWrite(HIGHT_LEVEL_LIGHT, LOW);
      }
    xSemaphoreGive( ActuatorsMutex );
    }
    
    vTaskDelay( 500 / portTICK_PERIOD_MS ); // wait for half second
    digitalWrite(HIGHT_LEVEL_LIGHT, LOW);
    digitalWrite(LOW_LEVEL_LIGHT, LOW);
  }
}
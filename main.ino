#include <math.h>
#include <Wire.h>

#include "Model_Data.h"
#include "LCD.h"
#include "sensor.h"


#define configMAX_PRIORITIES 10
SemaphoreHandle_t USBSemaphore = NULL;


// define two tasks for Blink & AnalogRead
SemaphoreHandle_t createMutex(const char* name);
void createTask(TaskFunction_t taskFunc, const char* name, uint16_t stackSize, UBaseType_t priority);

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  LevelFollowMutex       = createMutex("LevelFollowMutex");
  LCDMutex               = createMutex("LCDMutex");
  USBSerial              = createMutex("USBSerialMutex");
  TankMutex              = createMutex("TankMutex");
  ActuatorsMutex         = createMutex("ActuatorsMutex");


  createTask(TaskDisplay,             "TaskDisplay",             5000, 4);
  createTask(TaskLevelFollow,         "TaskLevelFollow",         5000, 4);
  createTask(TaskNotificationLight,   "TaskNotificationLight",   5000, 4);
}

void loop()
{
}


SemaphoreHandle_t createMutex(const char* name) {
    SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
    if (mutex != NULL) {
        xSemaphoreGive(mutex);
    } else {
        Serial.print(String(name) + " Fail Allocating");
        error = true;
    }
    return mutex;
}


void createTask(TaskFunction_t taskFunc, const char* name, uint16_t stackSize, UBaseType_t priority) {
    BaseType_t result = xTaskCreate(taskFunc, name, stackSize, NULL, priority, NULL);
    if (result != pdPASS) {
      if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
        Serial.print(String(name) + " Fail creating task");
        if (result == errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY) {
          Serial.print(String(name) + "Reason: Not enough heap memory.");
        }
        xSemaphoreGive( USBSerialMutex );
      }
    }
      error = true;
    }
}
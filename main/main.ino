#include <math.h>
#include <esp_log.h>

#include "Model_Data.h"
#include "LCD.h"
#include "sensor.h"
#include "LocalNetworkIOT.h"
#include "actuators.h"
#include "ServerWEB.h"
#include "VolumenMath.h"


#define configMAX_PRIORITIES 10
SemaphoreHandle_t USBSemaphore = NULL;


// define two tasks for Blink & AnalogRead
SemaphoreHandle_t createMutex(const char* name);
void createTask(TaskFunction_t taskFunc, const char* name, uint16_t stackSize, UBaseType_t priority);


void printTaskList() {
  // Allocate enough buffer for all tasks
  static char taskListBuffer[1024];

  Serial.println(F("\nTask Name          State  Prio  Stack  Num"));
  Serial.println(F("------------------------------------------------"));

  vTaskList(taskListBuffer);
  Serial.println(taskListBuffer);
}


void printRunTimeStats() {
  static char statsBuffer[1024];

  Serial.println("\nTask Name        Time (ticks)     CPU%");
  Serial.println("----------------------------------------");

  vTaskGetRunTimeStats(statsBuffer);
  Serial.println(statsBuffer);
}



// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  esp_log_level_set("i2c", ESP_LOG_NONE);

  LevelFollowMutex       = createMutex("LevelFollowMutex");
  LCDMutex               = createMutex("LCDMutex");
  USBSerialMutex         = createMutex("USBSerialMutex");
  TankMutex              = createMutex("TankMutex");
  ActuatorsMutex         = createMutex("ActuatorsMutex");

  createTask(TaskDisplay,             "TaskDisplay",             4000, 4);
  createTask(TaskLevelFollow,         "TaskLevelFollow",         6000, 7);
  createTask(TaskNotificationLight,   "TaskNotificationLight",   1400, 2);
  createTask(TaskUpdateVolumen,       "TaskUpdateVolumen",       2200, 8);
  createTask(TaskScanNetwork,         "TaskScanNetwork",         5000, 4);
  createTask(TaskConnectedWifi,       "TaskConnectedWifi",       4000, 5);
  createTask(TaskUpdateServer,        "TaskUpdateServer",        5000, 3);
}

void loop()
{
  
  //printTaskList();
  //printRunTimeStats();
  delay(5000);

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

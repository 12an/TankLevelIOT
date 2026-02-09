#ifndef DATA
#define DATA

#include <Arduino.h>

extern bool error;

extern SemaphoreHandle_t LevelFollowMutex;
extern SemaphoreHandle_t LCDMutex;
extern SemaphoreHandle_t USBSerialMutex;
SemaphoreHandle_t TankMutex = NULL;

#endif
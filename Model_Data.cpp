#include "Model_Data.h"

#define DEBUG true
bool error = false;

// Ping Output
const int HIGHT_LEVEL_LIGHT 42
const int LOW_LEVEL_LIGHT 10
const int RUNNING_OK_LIGHT 11

//Tanque
float current_galons = 0;
const float CAPACITY_GALONS = 0;
const int NIVEL_BAJO_GALONS = 1;
const int NIVEL_ALTO_GALONS = 1;
const bool ORIENTACION_TANQUE = 0; //1 parado (un cilindro parado)
const float DIAMETRO_TANQUE = 1;
const float ALTURA_TANQUE= 1;
bool is_low = false;
bool is_hight = false;



SemaphoreHandle_t LevelFollowMutex = NULL;
SemaphoreHandle_t LCDMutex = NULL;
SemaphoreHandle_t USBSerialMutex = NULL;
SemaphoreHandle_t TankMutex = NULL;
SemaphoreHandle_t ActuatorsMutex = NULL;
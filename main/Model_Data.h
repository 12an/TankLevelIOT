#ifndef MODEL_DATA
#define MODEL_DATA

#define DEBUG true

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

extern bool error;

// Ping Output
extern const int HIGHT_LEVEL_LIGHT;
extern const int LOW_LEVEL_LIGHT;
extern const int RUNNING_OK_LIGHT;

//Tanque
extern float current_galons;
extern const float CAPACITY_GALONS;
extern const int NIVEL_BAJO_GALONS;
extern const int NIVEL_ALTO_GALONS;
extern const bool ORIENTACION_TANQUE;
extern const float DIAMETRO_TANQUE;
extern const float ALTURA_TANQUE;
extern const float LARGO_TANQUE;
extern const float BOLLA_OSFFET;


//tanque Data
extern float sensor_mm_read;
extern float current_galons;
extern float persentage_current;
extern float osffet_vertical_volumen;
extern float osffet_horizontal_volumen;
extern float osffet_sensor_volumen;
extern String status;
extern bool is_low;
extern bool is_hight;

extern SemaphoreHandle_t LevelFollowMutex;
extern SemaphoreHandle_t LCDMutex;
extern SemaphoreHandle_t USBSerialMutex;
extern SemaphoreHandle_t TankMutex;
extern SemaphoreHandle_t ActuatorsMutex;



// ====== WIFI ACCESS POINT ======
extern const char* ssid;
extern const char* password;

extern volatile bool DedicatedNetworkAvailable;

// ====== NETWORK CONFIG ======
extern const bool NetworkConfigured;
extern IPAddress local_IP;
extern IPAddress gateway;
extern IPAddress subnet;
extern IPAddress primaryDNS;
extern IPAddress secondaryDNS;

// ====== DEVICE INFO ======
extern const char* MyHostName;
extern const uint8_t MAC_Address[];

#endif
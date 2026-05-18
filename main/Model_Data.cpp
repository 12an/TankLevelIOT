#include "Model_Data.h"


bool error = false;

// Ping Output
const int HIGHT_LEVEL_LIGHT = 32;
const int LOW_LEVEL_LIGHT =33;
const int RUNNING_OK_LIGHT =25;

//Tanque
const float CAPACITY_GALONS = 5;
const int NIVEL_BAJO_GALONS = 2;
const int NIVEL_ALTO_GALONS = 4;
const bool ORIENTACION_TANQUE = 0; //1 parado (un cilindro parado)
const float DIAMETRO_TANQUE = 568;//295;
const float ALTURA_TANQUE= 916;// Altura tubo senso hasta punta
const float LARGO_TANQUE= 853;//354;
const float BOLLA_OSFFET = 26.97;


//tanque Data
float sensor_mm_read = 0;
float current_galons = 0;
float persentage_current = 0;
float osffet_vertical_volumen = -0.4;
float osffet_horizontal_volumen = -0.8;
float osffet_sensor =  - 54.5;
String status = "";
bool is_low = false;
bool is_hight = false;



SemaphoreHandle_t LevelFollowMutex = NULL;
SemaphoreHandle_t LCDMutex = NULL;
SemaphoreHandle_t USBSerialMutex = NULL;
SemaphoreHandle_t TankMutex = NULL;
SemaphoreHandle_t ActuatorsMutex = NULL;



// ====== WIFI ACCESS POINT ======
const char* ssid     = "Altice-652E20";
const char* password = "60054d1d5d";

volatile bool DedicatedNetworkAvailable = false;

// ====== NETWORK CONFIG ======
const bool NetworkConfigured = false;
IPAddress local_IP(192, 168, 1, 6);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 1, 1);
IPAddress secondaryDNS(0, 0, 0, 0);

// ====== DEVICE INFO ======
const char* MyHostName = "Timer_4";
const uint8_t MAC_Address[] = {0x14, 0x43, 0x2A, 0x1D, 0x1A, 0xD1};
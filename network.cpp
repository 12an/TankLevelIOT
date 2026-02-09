#include <WiFi.h>
#include <esp_wifi.h>

// WIFI acces point
const char* ssid = "Vispero_Timers";
const char* password = "Vispero_Timers1";
bool Dedicate_Network_Avaible = false;

//wifi network config
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(192, 168, 1, 1);
IPAddress primaryDNS(192, 168, 1, 1);
IPAddress secondaryDNS(0, 0, 0, 0);

//wifi client
const char* host = "192.168.1.5";
const uint16_t port = 9525;

//name of the device, unique for each one
const char* MyHostName = "Timer_4";
const unsigned int Device_ID = 4; //this is the identifier of the device to make the mix Timer_ + str(Device_ID)
uint8_t MAC_Address[] = {0x34, 0x43, 0x4A, 0x1D, 0x4A, 0xD4};
IPAddress local_IP(192, 168, 1, 6);


void TaskScanNetwork(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  
  folow the time the counting time. and activate diferents events.
*/
  // config wifi device
  WiFi.setHostname(MyHostName);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_mac(WIFI_IF_STA, MAC_Address);

  for (;;){
    if(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_IDLE_STATUS && WiFi.status() == WL_DISCONNECTED){
      Dedicate_Network_Avaible = false;
      USB_Serial_Printl(String("Device disconected"));      
      int n = WiFi.scanNetworks();
      if (n == 0) {
        vTaskDelay(1000);
        USB_Serial_Printl(String("no network found"));
      } else {
        for (int i = 0; i < n; ++i) {
          // look for the network
          if(strcmp(ssid, WiFi.SSID(i).c_str())==0){
            USB_Serial_Printl(String("Dedicated network found signal: "  + char(WiFi.RSSI(i))));
            if(abs(WiFi.RSSI(i))<=75){
              Dedicate_Network_Avaible = true;
            }
            break;
          }
        }
        vTaskDelay(1000);
        // Delete the scan result to free memory for code below.
        WiFi.scanDelete();
      }
    }
    vTaskDelay(1000);
  }
}


void TaskConectedWifi(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  AnalogReadSerial
  folow the time the counting time. and activate diferents events.
*/
  for (;;){
    if(Dedicate_Network_Avaible && WiFi.status() != WL_CONNECTED && WiFi.status() == WL_DISCONNECTED){
      // Attempt Connection
      USB_Serial_Printl(String("Trying to connect to Dedicated network"));
      WiFi.begin(ssid, password);
      while(WiFi.status() == WL_IDLE_STATUS && WiFi.status() != WL_CONNECT_FAILED && WiFi.status() != WL_CONNECTED){
        vTaskDelay(1000 / portTICK_PERIOD_MS );  // wait for a second
      }
          // config wifi network  
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)){
      USB_Serial_Printl(String("Configuration Failed!"));
    }
    }
    if(WiFi.status() == WL_CONNECTED && WiFi.status() != WL_DISCONNECTED){
      USB_Serial_Printl(String("Device Wifi Conected"));
      vTaskDelay(60000);
    }
    vTaskDelay(1000);
  }
}
#include <WiFi.h>
#include <esp_wifi.h>

#include "LocalNetworkIOT.h"
#include "Model_Data.h"

void TaskScanNetwork(void *pvParameters)
{
  WiFi.setHostname(MyHostName);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  esp_wifi_set_mac(WIFI_IF_STA, MAC_Address);

  for (;;)
  {
     if(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_IDLE_STATUS && WiFi.status() == WL_DISCONNECTED){
      DedicatedNetworkAvailable = false;    
      int n = WiFi.scanNetworks();
      if (n == 0) {

        if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
          Serial.print("no network found");
          xSemaphoreGive( USBSerialMutex );
        }
      } else {
        for (int i = 0; i < n; ++i) {
          // look for the network
          if(strcmp(ssid, WiFi.SSID(i).c_str())==0){
            if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
              Serial.print("Dedicated network found signal: "  + char(WiFi.RSSI(i)));
              xSemaphoreGive( USBSerialMutex );
            }
            if(abs(WiFi.RSSI(i))<=75){
              DedicatedNetworkAvailable = true;
            }
            break;
          }
        }

        // Delete the scan result to free memory for code below.
        WiFi.scanDelete();
      }
    }

    if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
        Serial.print(".............................................");
        xSemaphoreGive( USBSerialMutex );
      }
    vTaskDelay(pdMS_TO_TICKS(10000)); // cada 10 segundos
  }
}


void TaskConnectedWifi(void *pvParameters)
{
  for (;;)
  {
    if(DedicatedNetworkAvailable && WiFi.status() != WL_CONNECTED && WiFi.status() == WL_DISCONNECTED){
      // Attempt Connection
      if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
        Serial.print("Connecting to dedicated network...\n");
        xSemaphoreGive( USBSerialMutex );
      }
      WiFi.begin(ssid, password);
      vTaskDelay(10000 / portTICK_PERIOD_MS );  // wait for a second
      while(WiFi.status() == WL_IDLE_STATUS && WiFi.status() != WL_CONNECT_FAILED && WiFi.status() != WL_CONNECTED){
        vTaskDelay(5000 / portTICK_PERIOD_MS );  // wait for a second
      }
          // config wifi network  
    if (NetworkConfigured && WiFi.status() == WL_CONNECTED)
      {
        WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
        if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
          Serial.print("WiFi configuration failed!\n");
          xSemaphoreGive( USBSerialMutex );
        }
      }
    }
    if (WiFi.status() == WL_CONNECTED)
      {
        if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
          Serial.print("WiFi connected | IP: " + WiFi.localIP().toString() + "\n");
          xSemaphoreGive( USBSerialMutex );
        }
      }
      else
      {
        if (xSemaphoreTake(USBSerialMutex, ( TickType_t ) 100 ) == pdTRUE && DEBUG){
          Serial.print("WiFi connection failed\n");
          xSemaphoreGive( USBSerialMutex );
          vTaskDelay(10000);
        }
      }
    vTaskDelay(1000);
  }
}
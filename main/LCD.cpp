#include <U8g2lib.h>
#include <Arduino.h>
#include "Model_Data.h"
#include "LCD.h"


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // [full framebuffer, size = 1024 bytes]

float current_galons_view;
int persentage_current_view;


void TaskDisplay(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
/*
  TaskDisplay
  HMI
*/

  u8g2.begin();
  vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for half second
  u8g2.setColorIndex(1);  // set the color to white
  u8g2.setBitmapMode(1);

  for (;;){
    u8g2.clearBuffer();  // clear buffer for storing display content in RAM
    main_screen();
    u8g2.sendBuffer(); // send buffer from RAM to display controller

    vTaskDelay(150 / portTICK_PERIOD_MS );  // one tick delay (15ms) in between reads for stability
  }
}

void main_screen(){
  char buf[10];
  char per[10];

  if (xSemaphoreTake(TankMutex, ( TickType_t ) 10 ) == pdTRUE){
    current_galons_view = current_galons;
    persentage_current = (current_galons_view / CAPACITY_GALONS) * 100;
    persentage_current_view = persentage_current;
    xSemaphoreGive( TankMutex );
  }
  
  //fixet text galones
  u8g2.setFont(u8g2_font_courR08_tf);

  //persentage Level
  u8g2.drawStr(2, 60, "GALONES:");
  
  
  sprintf(buf, "%.1f", current_galons_view);
  u8g2.drawStr(SCREEN_WIDTH/2 + 15, 60, buf);  

  // galones float
  u8g2.setFont(u8g2_font_courB24_tf);
  if(persentage_current_view / 100 >= 1){
    
    u8g2.drawStr(25, 30, (String(persentage_current_view) + "%").c_str()); 
  }
  else if (persentage_current_view / 10 >= 1){
    
    u8g2.drawStr(40, 30, (String(persentage_current_view) + "%").c_str()); 
  }else{
    u8g2.drawStr(50, 30, (String(persentage_current_view) + "%").c_str()); 
  }
   
  //bajo, normal, alto
  u8g2.setFont(u8g2_font_crox3cb_tf);

  if (xSemaphoreTake(ActuatorsMutex, ( TickType_t ) 10 ) == pdTRUE){
    if(current_galons_view > NIVEL_ALTO_GALONS){
      u8g2.drawStr(SCREEN_WIDTH/2 + 3, 50, "ALTO");
      status = "ALTO";
      is_hight = true;
    }
    else if (current_galons_view < NIVEL_BAJO_GALONS){
      u8g2.drawStr(SCREEN_WIDTH/2 + 3, 50, "BAJO");
      status = "BAJO";
      is_hight = false;
      is_low = true;
    }else{
      u8g2.drawStr(SCREEN_WIDTH/2 - 21, 50, "NORMAL"); 
      status = "NORMAL";
      is_hight = false;
      is_low = false;
    }
    xSemaphoreGive( ActuatorsMutex );
  }
}

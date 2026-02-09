#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>

#include "Model_Data.h"

void TaskDisplay(void *pvParameters);
static void main_screen();

#endif
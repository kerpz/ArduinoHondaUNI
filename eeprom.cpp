#include <Arduino.h>
// eeprom
#include <EEPROM.h>

#include "eeprom.h"

uint8_t obd_select = 2;

uint8_t ect_alarm = 98;   // celcius
uint8_t vss_alarm = 100;  // kph / + 10
uint8_t th_threshold = 4; // celcius / + 10

uint8_t beep_enable = 1;
uint8_t elm_enable = 0;
uint8_t display_enable = 1;
uint8_t afr_enable = 1;

// 1024 bytes for arduino nano

void loadConfig()
{
  obd_select = EEPROM.read(0);
  ect_alarm = EEPROM.read(1);
  vss_alarm = EEPROM.read(2);
  th_threshold = EEPROM.read(3);
}

void saveConfig()
{
  EEPROM.write(0, obd_select);
  EEPROM.write(1, ect_alarm);
  EEPROM.write(2, vss_alarm);
  EEPROM.write(3, th_threshold);
}

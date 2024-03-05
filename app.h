#ifndef APP_H
#define APP_H

#include "eeprom.h"

#define APPNAME "Honda UNI   v1.0"

// OUT
#define PIN_LOCK 4
#define PIN_UNLOCK 5
#define PIN_AC 6
#define PIN_BUZZER 13
// IN
#define PIN_BUTTON 7
#define PIN_DOOR 8
// ANALOG
#define PIN_VOLT 14
#define PIN_FP 15
#define PIN_AFR 16
#define PIN_TH 17

// ecu data
extern int rpm;
extern int ect;
extern int iat;
extern int maps;
extern int baro;
extern uint8_t tps;
extern int sft;
extern int lft;
extern int inj;
extern int ign;
extern int lmt;
extern int iacv;
extern int knoc;
extern float volt;
extern float o2;
extern uint8_t vss;

extern bool sw_aircon;
extern bool sw_brake;
extern bool sw_vtec;

// extra sensor
extern float volt2;
extern float th;
extern float afr;
extern float fp;
extern bool cp;

extern int rpmtop;

extern uint8_t vsstop;
extern uint8_t vssavg;

// extern uint32_t trip_run_time;
// extern uint32_t trip_idle_time;
extern uint32_t trip_distance;

extern int dtcErrors[14];
extern int dtcCount;

extern uint16_t run_time;

void appSetup();
void appLoop();

#endif
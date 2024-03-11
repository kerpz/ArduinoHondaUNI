#ifndef APP_H
#define APP_H

#include "eeprom.h"
#include "button.h"
#include "elm.h"
#include "display.h"

#define APPNAME "Honda UNI   v1.0"

// UNUSED
// dislplay spi
#define PIN_SCK 2
#define PIN_MOSI 3
#define PIN_CS 4
// security (out)
#define PIN_LOCK 5
#define PIN_UNLOCK 6
#define PIN_SIREN 7
// IN
#define PIN_DOOR 8
#define PIN_BUTTON 9
// elm serial (out)
#define PIN_RX 10
#define PIN_TX 11
// dlc serial (half duplex)
#define PIN_DLC 12
//  OUT
#define PIN_BUZZER 13
// ANALOG
#define PIN_VOLT 14
#define PIN_FP 15
#define PIN_AFR 16
#define PIN_TH 17
// #define PIN_SDA 18
// #define PIN_SCL 19
// #define PIN_ A6
// #define PIN_ A7

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
extern float afr;
extern float fp;
extern float th;
// bool cp = 0;

extern int rpmtop;
extern int volttop;
extern int mapstop;
extern uint8_t tpstop;
extern int ecttop;
extern int iattop;

extern uint32_t trip_run_time;
extern uint32_t trip_idle_time;
extern uint32_t trip_distance;
extern uint32_t trip_vss_sum;

extern uint8_t vsstop;
extern uint8_t vssavg;

extern uint8_t dlcdata[20]; // dlc data buffer
extern uint8_t dlcTimeout;
extern uint8_t dlcChecksumError;

extern int dtcErrors[14];
extern int dtcCount;

extern uint16_t run_time;

int dlcCommand(uint8_t cmd, uint8_t num, uint8_t loc, uint8_t len);

void resetEcu();
void readEcuData();
/*
int readRPM();
int readVSS();
float readECT();
float readIAT();
int readMAP();
int readBARO();
int readTPS();
float readO2();
float readVOLT();
int readALTFR();
float readELD();
float readEGR();
int readSFT();
int readLFT();
int readINJ();
float readIGN();
float readIGN2();
int readIACV();
int readKNOC();
bool readAIRCON();
bool readBRAKE();
bool readVTEC();
*/

// extra sensors
float readVoltage();
float readAirFuelRatio();
float readThermistor();
float readFuelPressure();

// controls
void pushPinHi(uint8_t pin, uint32_t delayms);

void appSetup();
void appLoop();

#endif
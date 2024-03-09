/*
 Author:
 - Philip Bordado (kerpz@yahoo.com)

 Hardware:
 - Arduino Nano (Compatible board)
 - HC-05 Bluetooth module
 - Resistors 680k ohms and 220k ohms
 - Piezo Buzzer
 - LCD 16x2 I2C
 Optional:
 - 100 psi transducer for fuel pressure (0.5v - 4.5v)
 - AEM AFR UEGO
 - 10K NTC Thermistor

 Dev Tools:
 - Arduino 1.8.19 or VScode with extensions: arduino, c/c++, c/c++ extension pack, c/c++ themes
 - SoftwareSerialWithHalfDuplex (Library)
   https://github.com/nickstedman/SoftwareSerialWithHalfDuplex

 Formula:
 - IMAP = RPM * MAP / IAT / 2
 - MAF = (IMAP/60)*(VE/100)*(Eng Disp)*(MMA)/(R)
   Where: VE = 80% (Volumetric Efficiency), R = 8.314 J/°K/mole, MMA = 28.97 g/mole (Molecular mass of air)
   http://www.lightner.net/obd2guru/IMAP_AFcalc.html
   http://www.installuniversity.com/install_university/installu_articles/volumetric_efficiency/ve_computation_9.012000.htm
 - AFR = (o2 * 2) + 10
   Where: o2 in Volts

 Arduino Pin Mapping / Configuration:
 * Pulse Input - 50k ohms over 5.1v zener
 $ Switch Out - 1k ohms on NPN transistor (2n3904)

 - 00 = Serial RX
 - 01 = Serial TX
 - 02 = * Injector Input
 - 03 = * Injector Input
 - 04 = $ Door Lock | * RPM Input
 - 05 = $ Door Unlock | * VSS Input
 - 06 = $ AC Compressor
 - 07 = Navigation Button
 - 08 = * Door Sensor
 - 09 = AEM AFR UEGO (Serial In)
 - 10 = Bluetooth RX
 - 11 = Bluetooth TX
 - 12 = K-Line (Serial In)
 - 13 = Piezo buzzer (+)

 - 14 = (A0) Voltage divider (Input Signal)
 - 15 = (A1) 100 PSI Fuel Pressure
 - 16 = (A2) AEM AFR UEGO (ADC)
 - 17 = (A3) NTC Thermistor
 - 18 = (A4) I2C / LCD I2C
 - 19 = (A5) I2C / LCD I2C
 - 20 = (A6)
 - 21 = (A7)
*/

#include "eeprom.h"
#include "display.h"
#include "button.h"
#include "app.h"

void execEvery(int ms)
{
  static unsigned long msTick = millis();
  static uint8_t sTick;

  if (millis() - msTick >= ms)
  { // run every N ms
    msTick = millis();

    if (sTick >= 59)
    {
      sTick = 0;
      run_time++;
    }
    else
    {
      sTick++;
    }
  }
}

void setup()
{
  delay(10);

  Serial.begin(115200);

  Serial.println();
  Serial.println(APPNAME);

  loadConfig();

  buttonSetup();
  appSetup();

  delay(100);
}

void loop()
{
  // execEvery(1000);

  if (display_enable)
    displayLoop();

  buttonLoop();
  appLoop();
}

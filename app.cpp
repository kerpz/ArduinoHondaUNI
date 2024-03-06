#include <Arduino.h>
#include <SoftwareSerialWithHalfDuplex.h>

#include "app.h"

SoftwareSerialWithHalfDuplex dlcSerial(PIN_DLC, PIN_DLC, false, false);

// ecu data
int rpm = 0;
int ect = 0;
int iat = 0;
int maps = 0;
int baro = 0;
uint8_t tps = 0;
int sft = 0;
int lft = 0;
int inj = 0;
int ign = 0;
int lmt = 0;
int iacv = 0;
int knoc = 0;
float volt = 0;
float o2 = 0;
uint8_t vss = 0;

bool sw_aircon = false;
bool sw_brake = false;
bool sw_vtec = false;

// extra sensor
float volt2 = 0;
float th = 0;
float afr = 0;
float fp = 0;
bool cp = 0;

int rpmtop = 0;
int volttop = 0;
int mapstop = 0;
uint8_t tpstop = 0;
int ecttop = 0;
int iattop = 0;

uint32_t trip_run_time = 0;
uint32_t trip_idle_time = 0;
uint32_t trip_distance = 0;
uint32_t trip_vss_sum = 0;

uint8_t vsstop = 0;
uint8_t vssavg = 0;

// voltage divider
// float R1 = 30000.0;
// float R2 = 7500.0;
float R1 = 680000.0; // Resistance of R1 (680kohms)
float R2 = 220000.0; // Resistance of R2 (220kohms)
float R3 = 10000.0;  // Thermistor divider

uint8_t dlcdata[20] = {0}; // dlc data buffer
uint8_t dlcTimeout = 0, dlcChecksumError = 0;
bool dlcWait = false;

int dtcErrors[14] = {0};
int dtcCount = 0;

uint16_t run_time = 0;

// --- begin ECU functions ---
void dlcInit()
{
  dlcSerial.write(0x68);
  dlcSerial.write(0x6a);
  dlcSerial.write(0xf5);
  dlcSerial.write(0xaf);
  dlcSerial.write(0xbf);
  dlcSerial.write(0xb3);
  dlcSerial.write(0xb2);
  dlcSerial.write(0xc1);
  dlcSerial.write(0xdb);
  dlcSerial.write(0xb3);
  dlcSerial.write(0xe9);
  delay(300);
}

int dlcCommand(uint8_t cmd, uint8_t num, uint8_t loc, uint8_t len)
{
  uint8_t crc = (0xFF - (cmd + num + loc + len - 0x01)); // checksum FF - (cmd + num + loc + len - 0x01)

  unsigned long timeOut = millis() + 200; // timeout @ 200 ms

  memset(dlcdata, 0, sizeof(dlcdata));

  dlcSerial.listen();

  dlcSerial.write(cmd); // header/cmd read memory ??
  dlcSerial.write(num); // num of bytes to send
  dlcSerial.write(loc); // address
  dlcSerial.write(len); // num of bytes to read
  dlcSerial.write(crc); // checksum

  // reply: 00 len+3 data...
  int i = 0;
  while (i < (len + 3) && millis() < timeOut)
  {
    if (dlcSerial.available())
    {
      dlcdata[i] = dlcSerial.read();
      // if (dlcdata[i] != 0x00 && dlcdata[i+1] != (len+3)) continue; // ignore ?
      i++;
    }
  }

  if (i < (len + 3))
  { // timeout
    dlcTimeout++;
    if (dlcTimeout > 255)
      dlcTimeout = 0;
    return 0; // failed
  }

  // checksum
  crc = 0;
  for (i = 0; i < len + 2; i++)
  {
    crc = crc + dlcdata[i];
  }
  crc = 0xFF - (crc - 1);
  if (crc != dlcdata[len + 2])
  { // checksum failed
    dlcChecksumError++;
    if (dlcChecksumError > 255)
      dlcChecksumError = 0;
    return 0; // failed
  }

  return 1; // success
}

// Read DTC Error
void scanDtcError()
{
  uint8_t i;

  if (dlcCommand(0x20, 0x05, 0x40, 0x10))
  { // row 5
    for (i = 0; i < 14; i++)
    {
      if (dlcdata[i + 2] >> 4)
      {
        dtcErrors[i] = i * 2;
        dtcCount++;
      }
      if (dlcdata[i + 2] & 0xf)
      {
        // haxx
        // if (errnum == 23) errnum = 22;
        // if (errnum == 24) errnum = 23;
        dtcErrors[i] = (i * 2) + 1;
        // haxx
        if (dtcErrors[i] == 23)
          dtcErrors[i] = 22;
        if (dtcErrors[i] == 24)
          dtcErrors[i] = 23;
        dtcCount++;
      }
    }
  }
}

// Reset ECU
void resetEcu()
{
  // 21 04 01 DA / 01 03 FC
  dlcCommand(0x21, 0x04, 0x01, 0x00); // reset ecu
}

// Read ECU Data
void readEcuData()
{
  float f;

  // char h_initobd2[12] = {0x68,0x6a,0xf5,0xaf,0xbf,0xb3,0xb2,0xc1,0xdb,0xb3,0xe9}; // 200ms - 300ms delay
  // byte h_cmd1[6] = {0x20,0x05,0x00,0x10,0xcb}; // row 1
  // byte h_cmd2[6] = {0x20,0x05,0x10,0x10,0xbb}; // row 2
  // byte h_cmd3[6] = {0x20,0x05,0x20,0x10,0xab}; // row 3
  // byte h_cmd4[6] = {0x20,0x05,0x76,0x0a,0x5b}; // ecu id

  if (dlcCommand(0x20, 0x05, 0x00, 0x10))
  { // row 1
    if (obd_select == 1)
      rpm = 1875000 / (dlcdata[2] * 256 + dlcdata[3] + 1); // OBD1
    if (obd_select == 2)
      rpm = (dlcdata[2] * 256 + dlcdata[3]) / 4; // OBD2
    // in odb1 rpm is -1
    if (rpm < 0)
    {
      rpm = 0;
    }

    vss = dlcdata[4]; // 0 - 255 kph

    // discrete sensors
    // dlcdata[10]
    sw_aircon = bitRead(dlcdata[10], 2);
    // dlcdata[11]
    // dlcdata[12]
    sw_vtec = bitRead(dlcdata[12], 3);
    // dlcdata[13]
    // dlcdata[14]
    // dlcdata[15]
    // dlcdata[17]
  }

  delay(1);

  if (dlcCommand(0x20, 0x05, 0x10, 0x10))
  { // row2
    f = dlcdata[2];
    ect = 155.04149 - f * 3.0414878 + pow(f, 2) * 0.03952185 - pow(f, 3) * 0.00029383913 + pow(f, 4) * 0.0000010792568 - pow(f, 5) * 0.0000000015618437;
    f = dlcdata[3];
    iat = 155.04149 - f * 3.0414878 + pow(f, 2) * 0.03952185 - pow(f, 3) * 0.00029383913 + pow(f, 4) * 0.0000010792568 - pow(f, 5) * 0.0000000015618437;
    maps = dlcdata[4] * 0.716 - 5; // 101 kPa @ off|wot // 10kPa - 30kPa @ idle
    // maps = maps * 0.145038;        // to psi // 14.7 psi @ off|wot, 1.4 - 4.4 psi @ idle
    baro = dlcdata[5] * 0.716 - 5; // kPa
    // baro = baro * 0.145038;        // to psi
    tps = (dlcdata[6] - 24) / 2; // (%) 0 - 100

    f = dlcdata[7];
    o2 = f / 51.3; // (V) o2

    /*
    // 0v to 1v / stock sensor
    // 0v to 5v / AEM UEGO / linear
    afr = (f * 2) + 10; // afr for AEM UEGO
    */

    f = dlcdata[9];
    volt = f / 10.45; // (V) battery
    // alt_fr = dlcdata[10] / 2.55 // (%) alternator load
    // eld = 77.06 - dlcdata[11] / 2.5371; // (Amps) electrical load
  }

  delay(1);

  if (dlcCommand(0x20, 0x05, 0x20, 0x10))
  {                                     // row3
    sft = (dlcdata[2] / 128 - 1) * 100; // (%) -30 to 30
    lft = (dlcdata[3] / 128 - 1) * 100; // (%) -30 to 30

    inj = (dlcdata[6] * 256 + dlcdata[7]) / 250; // (ms) 0 to 16

    // ign = (dlcdata[8] - 128) / 2;
    f = dlcdata[8];
    ign = (f - 24) / 4; // (degrees)

    // lmt = (dlcdata[9] - 128) / 2;
    f = dlcdata[9];
    lmt = (f - 24) / 4;

    iacv = dlcdata[10] / 2.55; // (%) 0 - 100
  }

  delay(1);

  if (dlcCommand(0x20, 0x05, 0x30, 0x10))
  { // row4
    // dlcdata[7] to dlcdata[12] unknown
    knoc = dlcdata[14] / 51; // 0 to 5
  }
}
// --- end ECU functions ---

// --- begin extra sensors ---
long readVcc()
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  /*
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif

    delay(2);            // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC))
      ; // measuring

    uint8_t low = ADCL;  // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both

    long vcc = (high << 8) | low;
    vcc = 1125300L / vcc; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    */
  long vcc = 5000; // use a multimeter instead?
  return vcc;
}

float readVoltage()
{
  float f;

  // read voltage sensor (volt2)
  f = readVcc() / 1000;                    // V read from ref. or 5.0
  f = (analogRead(PIN_VOLT) * f) / 1024.0; // V
  f = f / (R2 / (R1 + R2));                // voltage divider

  return f;
}

float readAirFuelRatio()
{
  // air fuel ratio, x=afr(10-20), y=volts(0-5)
  // y = mx + b // slope intercept
  // x = (y - b) / m // derived for x
  // m = y2 - y1 / x2 - x1 = 0.5
  // y = 0.5x + 0 @ x = 10, b = -5

  // where:
  // y = volts
  // m = slope
  // b = y intercept
  // x = afr

  // x = (y + 5) / 0.5

  // read afr sensor (afr)
  float f = readVcc() / 1000;             // V read from ref. or 5.0
  f = (analogRead(PIN_AFR) * f) / 1024.0; // V
  f = (f + 5) / 0.5;                      // afr
  // f = 2 * f + 10;

  return f;
}

float readThermistor()
{
  // read thermal sensor (th)
  int b = 3950;
  float f = analogRead(PIN_TH);

  f = R3 * (1023.0 / f - 1.0);
  f = log(f);
  f = 1.0 / (0.001129148 + (0.000234125 * f) + (0.0000000876741 * f * f * f)); // K
  f = f - 273.15;                                                              // convert to C
  // boundary correctors
  if (f < 0)
    f = 0;
  if (f > 99)
    f = 99;

  /*
  f = 1023.0 / f  - 1.0;
  f = R3 / f;                 // resistance
  // th steinhart;
  f = f / 10000.0;            // (R/Ro)
  f = log(f);                 // ln(R/Ro)
  f /= b;                     // 1/B * ln(R/Ro)
  f += 1.0 / (25 + 273.15);   // + (1/To)
  f = 1.0 / f;                // Invert
  f -= 273.15;                // convert to C
  */

  return f;
}

float readFuelPressure()
{
  // fuel pressure, x=psi(0-100), y=volts(0.5-4.5)
  // y = mx + b
  // x = (y - b) / m // derived for x
  // m = y2 - y1 / x2 - x1 = 0.04
  // y = 0.04x + 0.5 @ x = 0, b = -5

  // where:
  // y = volts
  // m = slope
  // b = y intercept
  // x = psi

  // x = (y - 0.5) / 0.04

  // fuel pressur sensor (fp)
  float f = readVcc() / 1000;            // V read from ref. or 5.0
  f = (analogRead(PIN_FP) * f) / 1024.0; // V
  f = (f - 0.5) / 0.04;                  // psi
  f = f * 6.89476;                       // kPa
  // boundary correctors
  if (f < 0)
    f = 0;
  if (f > 1034)
    f = 1034;

  return f;
}
// --- end extra sensors ---

void pushPinHi(uint8_t pin, uint32_t delayms)
{
  digitalWrite(pin, HIGH);
  delay(delayms);
  digitalWrite(pin, LOW);
}

void appSetup()
{
  pinMode(PIN_BUZZER, OUTPUT); // Piezo Buzzer
  // pinMode(PIN_AC, OUTPUT);     // Air Condition

  pinMode(PIN_DOOR, INPUT); // Door

  pinMode(PIN_VOLT, INPUT); // Volt meter
  pinMode(PIN_FP, INPUT);   // 100psi Fuel Pressure
  pinMode(PIN_AFR, INPUT);  // AEM UEGO AFR
  pinMode(PIN_TH, INPUT);   // 10k Thermistor

  dlcSerial.begin(9600);

  // initial beep
  for (int i = 0; i < 3; i++)
  {
    pushPinHi(PIN_BUZZER, 50); // beep 50ms
    delay(80);
  }

  delay(100);
}

void appLoop()
{
  static unsigned long msTick = millis();
  if (millis() - msTick >= 250)
  { // run every 250 ms
    msTick = millis();

    // readEcuData();

    volt2 = readVoltage();
    fp = readFuelPressure();
    afr = readAirFuelRatio();
    // th = readThermistor();

    // register top values
    if (rpm > rpmtop)
      rpmtop = rpm;
    if (vss > vsstop)
      vsstop = vss;
    if (ect > ecttop)
      ecttop = ect;
    if (iat > iattop)
      iattop = iat;
    if (volt > volttop)
      volttop = volt;
    if (maps > mapstop)
      mapstop = maps;

    // trip computer essentials
    if (rpm > 0)
    {
      if (vss > 0)
      { // running time
        trip_run_time++;
        trip_vss_sum += vss;
        vssavg = (trip_vss_sum / trip_run_time);

        float f;
        // f = vssavg;
        // f = ((f * 1000) / 14400) * running_time; // @ 250ms
        // distance = round(f);

        // formula: distance = speed * fps / 3600
        // where: distance = kilometer(s), speed = km/h, fps in second(s)
        f = vss;
        f = f * 0.25 / 3600; // @ 250ms / km
        f = f * 1000;        // km to meters
        trip_distance = trip_distance + round(f);

        // time = distance / speed
      }
      else
      { // idle time
        trip_idle_time++;
      }
    }

    if (ect > ect_alarm) // engine temp alarm
      digitalWrite(PIN_BUZZER, HIGH);
    else if (vss > 100 && vss < 105) // car speed alarm/notification
      digitalWrite(PIN_BUZZER, HIGH);
    else
      digitalWrite(PIN_BUZZER, LOW);

    // aircon controller (replace the OEM one)
    /*
    if (th <= th_threshold)
    {
      digitalWrite(PIN_AC, LOW);
      cp = 0;
    }
    else if (th >= (th_threshold + 10))
    {
      digitalWrite(PIN_AC, HIGH);
      cp = 1;
    }
    */

    // IMAP = RPM * MAP / IAT / 2
    // MAF = (IMAP/60)*(VE/100)*(Eng Disp)*(MMA)/(R)
    // Where: VE = 80% (Volumetric Efficiency), R = 8.314 J/°K/mole, MMA = 28.97 g/mole (Molecular mass of air)
    // int imap = rpm * maps / (iat + 273) / 2;
    // ve = 75, ed = 1.595, afr = 14.7
    // maf = (imap / 60) * (80 / 100) * 1.595 * 28.9644 / 8.314472;

    // (gallons of fuel) = (grams of air) / (air/fuel ratio) / 6.17 / 454
    // gof = maf / afr / 6.17 / 454;

    // gear = vss / (rpm + 1) * 150 + 0.3;
  }
}
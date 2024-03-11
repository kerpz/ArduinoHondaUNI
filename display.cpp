#include <Arduino.h>
// 128x64
#include "U8glib.h"

#include "display.h"

// SPI Com:
// SCK = en = 2
// MOSI = rw = 3
// CS = di = 4
U8GLIB_ST7920_128X64_4X u8g(PIN_SCK, PIN_MOSI, PIN_CS);

#include "display.h"

uint8_t pag_select = 1; // lcd page

void disaply_page_1()
{
  char tmp_string[8] = {0};

  u8g.setFont(u8g_font_fixed_v0);
  // u8g.setFontRefHeightExtendedText();
  // u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();

  u8g.drawBox(0, 0, 90, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 0, "Main Display");
  u8g.setColorIndex(1);

  u8g.drawBox(0, 10, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 10, "TKPH");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", vsstop);
  u8g.drawStr(27, 10, tmp_string);

  // u8g.drawStr(50, 10, "0");

  u8g.drawBox(0, 20, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 20, "AKPH");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", vssavg);
  u8g.drawStr(27, 20, tmp_string);

  u8g.drawFrame(46, 10, 44, 19);
  u8g.setScale2x2();

  sprintf(tmp_string, "%03d", ect);
  u8g.drawStr(24, 5, tmp_string);
  u8g.undoScale();
  u8g.drawStr(83, 9, "o");
  // u8g.drawStr(83, 19, "+");

  u8g.drawFrame(1, 30, 89, 34);
  u8g.setFont(u8g_font_DS_Digital_V1);
  u8g.setFontPosTop();
  sprintf(tmp_string, "%03d", vss);
  u8g.drawStr(10, 35, tmp_string);

  u8g.setFont(u8g_font_fixed_v0);
  u8g.setFontPosTop();

  u8g.drawStr(62, 50, "KPH");

  u8g.drawLine(92, 0, 92, 63);

  u8g.drawBox(94, 0, 33, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(99, 0, "VOLT");
  u8g.setColorIndex(1);

  // dtostrf(volt2, 2, 1, tmp_string);
  dtostrf(volt, 2, 1, tmp_string);
  // sprintf(tmp_string, "%04d", volt);
  u8g.drawStr(99, 11, tmp_string);

  u8g.drawBox(94, 21, 33, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(102, 21, "RPM");
  u8g.setColorIndex(1);

  sprintf(tmp_string, "%04d", rpm);
  u8g.drawStr(99, 32, tmp_string);

  u8g.drawBox(94, 42, 33, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(99, 42, "TRIP");
  u8g.setColorIndex(1);

  sprintf(tmp_string, "%05d", trip_distance);
  // dtostrf(trip_distance / 1000, 2, 1, tmp_string);
  u8g.drawStr(96, 53, tmp_string);
  // u8g.drawStr(116, 53, "98");
}

void disaply_page_2()
{
  char tmp_string[8] = {0};

  u8g.setFont(u8g_font_fixed_v0);
  // u8g.setFontRefHeightExtendedText();
  // u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();

  u8g.drawBox(0, 0, 127, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 0, "Display Page 2");
  u8g.setColorIndex(1);

  u8g.drawFrame(0, 10, 127, 53);

  // col 1
  u8g.drawBox(4, 12, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 12, "RPM");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", rpm);
  u8g.drawStr(32, 12, tmp_string);

  u8g.drawBox(4, 22, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 22, "VSS");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", vss);
  u8g.drawStr(32, 22, tmp_string);

  u8g.drawBox(4, 32, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 32, "MAP");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", maps);
  u8g.drawStr(32, 32, tmp_string);

  u8g.drawBox(4, 42, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 42, "IAT");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", iat);
  u8g.drawStr(32, 42, tmp_string);

  u8g.drawBox(4, 52, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 52, "ECT");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", ect);
  u8g.drawStr(32, 52, tmp_string);

  // box: x,y,w,h
  // str: x,y
  // col 2
  u8g.drawBox(70, 12, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 12, "TPS");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", tps);
  u8g.drawStr(98, 12, tmp_string);

  u8g.drawBox(70, 22, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 22, "IACV");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", iacv);
  u8g.drawStr(98, 22, tmp_string);

  u8g.drawBox(70, 32, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 32, "SFT");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", sft);
  u8g.drawStr(98, 32, tmp_string);

  u8g.drawBox(70, 42, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 42, "LFT");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", lft);
  u8g.drawStr(98, 42, tmp_string);

  u8g.drawBox(70, 52, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 52, "BARO");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", baro);
  u8g.drawStr(98, 52, tmp_string);
}

void disaply_page_3()
{
  char tmp_string[8] = {0};

  u8g.setFont(u8g_font_fixed_v0);
  // u8g.setFontRefHeightExtendedText();
  // u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();

  u8g.drawBox(0, 0, 127, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 0, "Display Page 3");
  u8g.setColorIndex(1);

  u8g.drawFrame(0, 10, 127, 53);

  // col 1
  u8g.drawBox(4, 12, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 12, "O2");
  u8g.setColorIndex(1);
  dtostrf(o2, 2, 1, tmp_string);
  u8g.drawStr(32, 12, tmp_string);

  u8g.drawBox(4, 22, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 22, "VOLT");
  u8g.setColorIndex(1);
  dtostrf(volt, 2, 1, tmp_string);
  u8g.drawStr(32, 22, tmp_string);

  u8g.drawBox(4, 32, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 32, "LMT");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", lmt);
  u8g.drawStr(32, 32, tmp_string);

  u8g.drawBox(4, 42, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 42, "IGN");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", ign);
  u8g.drawStr(32, 42, tmp_string);

  u8g.drawBox(4, 52, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 52, "INJ");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", inj);
  u8g.drawStr(32, 52, tmp_string);

  // box: x,y,w,h
  // str: x,y
  // col 2
  u8g.drawBox(70, 12, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 12, "AC");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%01d", sw_aircon);
  u8g.drawStr(98, 12, tmp_string);

  u8g.drawBox(70, 22, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 22, "BRK");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%01d", sw_brake);
  u8g.drawStr(98, 22, tmp_string);

  u8g.drawBox(70, 32, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 32, "VTS");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%01d", sw_vtec);
  u8g.drawStr(98, 32, tmp_string);

  u8g.drawBox(70, 42, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 42, "KNOC");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", knoc);
  u8g.drawStr(98, 42, tmp_string);

  // u8g.drawBox(70, 52, 25, 9);
  // u8g.setColorIndex(0);
  // u8g.drawStr(71, 52, "DCE");
  // u8g.setColorIndex(1);
  // sprintf(tmp_string, "%03d", dlcChecksumError);
  // u8g.drawStr(98, 52, tmp_string);
}

void disaply_page_4()
{
  char tmp_string[8] = {0};

  u8g.setFont(u8g_font_fixed_v0);
  // u8g.setFontRefHeightExtendedText();
  // u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();

  u8g.drawBox(0, 0, 127, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 0, "Display Page 4");
  u8g.setColorIndex(1);

  u8g.drawFrame(0, 10, 127, 53);

  // col 1
  u8g.drawBox(4, 12, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 12, "VOLT2");
  u8g.setColorIndex(1);
  dtostrf(volt2, 2, 1, tmp_string);
  u8g.drawStr(32, 12, tmp_string);

  u8g.drawBox(4, 22, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 22, "AFR");
  u8g.setColorIndex(1);
  dtostrf(afr, 2, 1, tmp_string);
  u8g.drawStr(32, 22, tmp_string);

  u8g.drawBox(4, 32, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 32, "FP");
  u8g.setColorIndex(1);
  dtostrf(fp, 2, 1, tmp_string);
  u8g.drawStr(32, 32, tmp_string);

  u8g.drawBox(4, 42, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(5, 42, "TH");
  u8g.setColorIndex(1);
  dtostrf(th, 2, 1, tmp_string);
  u8g.drawStr(32, 42, tmp_string);

  // u8g.drawBox(4, 52, 25, 9);
  // u8g.setColorIndex(0);
  // u8g.drawStr(5, 52, "ECT");
  // u8g.setColorIndex(1);
  // sprintf(tmp_string, "%03d", ect);
  // u8g.drawStr(32, 52, tmp_string);

  // box: x,y,w,h
  // str: x,y
  // col 2
  u8g.drawBox(70, 12, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 12, "OBD");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%01d", obd_select);
  u8g.drawStr(98, 12, tmp_string);

  u8g.drawBox(70, 22, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 22, "DOOR");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%04d", 0);
  u8g.drawStr(98, 22, tmp_string);

  u8g.drawBox(70, 32, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 32, "SRN");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%01d", 0);
  u8g.drawStr(98, 32, tmp_string);

  u8g.drawBox(70, 42, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 42, "DTO");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", dlcTimeout);
  u8g.drawStr(98, 42, tmp_string);

  u8g.drawBox(70, 52, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 52, "DCE");
  u8g.setColorIndex(1);
  sprintf(tmp_string, "%03d", dlcChecksumError);
  u8g.drawStr(98, 52, tmp_string);
}

void displaySetup()
{
}

void displayLoop()
{
  static unsigned long msTick = millis();
  if (millis() - msTick >= 250)
  { // refresh rate 250 ms
    msTick = millis();

    readEcuData();

    volt2 = readVoltage();
    afr = readAirFuelRatio();
    th = readThermistor();
    fp = readFuelPressure();

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

    // render display
    u8g.firstPage();
    do
    {
      if (pag_select == 1)
        disaply_page_1();
      else if (pag_select == 2)
        disaply_page_2();
      else if (pag_select == 3)
        disaply_page_3();
      else if (pag_select == 4)
        disaply_page_4();
    } while (u8g.nextPage());
  }
}
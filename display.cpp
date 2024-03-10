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
  u8g.drawStr(1, 0, "ECU:disconnect");
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
  u8g.drawStr(1, 0, "ECU:disconnect");
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
  u8g.drawStr(71, 32, "DTO");
  u8g.setColorIndex(1);
  // sprintf(tmp_string, "%03d", sft);
  sprintf(tmp_string, "%04d", dlcTimeout);
  u8g.drawStr(98, 32, tmp_string);

  u8g.drawBox(70, 42, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 42, "DCE");
  u8g.setColorIndex(1);
  // sprintf(tmp_string, "%03d", lft);
  sprintf(tmp_string, "%04d", dlcChecksumError);
  u8g.drawStr(98, 42, tmp_string);

  u8g.drawBox(70, 52, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(71, 52, "VOLT");
  u8g.setColorIndex(1);
  dtostrf(volt, 2, 1, tmp_string);
  u8g.drawStr(98, 52, tmp_string);
}

void displaySetup()
{
}

void displayLoop()
{
  u8g.firstPage();
  do
  {
    if (pag_select == 1)
      disaply_page_1();
    else if (pag_select == 2)
      disaply_page_2();
  } while (u8g.nextPage());
}
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

char *dtostrf(double val, signed char width, unsigned char prec, char *s);

void draw_page_1()
{
  char tmp_string[8];

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
  u8g.drawStr(96, 0, "VOLT");
  u8g.setColorIndex(1);

  dtostrf(volt2, 2, 1, tmp_string);
  // sprintf(tmp_string, "%02.1f", volt2);
  u8g.drawStr(102, 11, tmp_string);

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

void draw_page_2()
{
  char tmp_string[8];

  u8g.setFont(u8g_font_fixed_v0);
  // u8g.setFontRefHeightExtendedText();
  // u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();

  u8g.drawBox(0, 0, 90, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 0, "X:Sec Y:RPM,KPH");
  u8g.setColorIndex(1);

  u8g.drawBox(0, 10, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 10, "TKPH");
  u8g.setColorIndex(1);
  itoa(vsstop, tmp_string, 10);
  u8g.drawStr(28, 10, tmp_string);

  // u8g.drawStr(50, 10, "0");

  u8g.drawBox(0, 20, 25, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(1, 20, "AKPH");
  u8g.setColorIndex(1);
  itoa(vsstop, tmp_string, 10);
  u8g.drawStr(28, 20, tmp_string);

  u8g.drawFrame(57, 10, 33, 19);
  u8g.setScale2x2();

  itoa(ect, tmp_string, 10);
  u8g.drawStr(30, 5, tmp_string);
  u8g.undoScale();
  u8g.drawStr(83, 11, "0");

  u8g.drawFrame(1, 30, 89, 34);

  u8g.drawLine(92, 0, 92, 63);

  u8g.drawBox(94, 0, 33, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(96, 0, "SPEED");
  u8g.setColorIndex(1);

  itoa(vss, tmp_string, 10);
  u8g.drawStr(102, 11, tmp_string);

  u8g.drawBox(94, 21, 33, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(102, 21, "RPM");
  u8g.setColorIndex(1);

  itoa(rpm, tmp_string, 10);
  u8g.drawStr(99, 32, tmp_string);

  u8g.drawBox(94, 42, 33, 9);
  u8g.setColorIndex(0);
  u8g.drawStr(99, 42, "TRIP");
  u8g.setColorIndex(1);

  ltoa(trip_distance, tmp_string, 10);
  u8g.drawStr(94, 53, tmp_string);
  // u8g.drawStr(116, 53, "98");
}

void draw_page_3()
{
  char tmp_string[8];

  u8g.setFont(u8g_font_DS_Digital_V1);
  u8g.setFontPosTop();

  // u8g.setScale2x2();
  itoa(vss, tmp_string, 10);
  u8g.drawStr(0, 3, tmp_string);
  // u8g.undoScale();

  u8g.setFont(u8g_font_fixed_v0);
  u8g.setFontPosTop();
  u8g.drawStr(52, 19, "KPH");

  u8g.setScale2x2();
  itoa(rpm, tmp_string, 10);
  u8g.drawStr(35, 0, tmp_string);
  u8g.undoScale();
  u8g.drawStr(98, 19, "RPM");

  u8g.setScale2x2();
  u8g.drawStr(43, 15, "28");
  u8g.undoScale();
  u8g.drawStr(110, 31, "0");

  u8g.drawStr(0, 31, "A-KPH = 135.0");
  u8g.drawStr(0, 39, "TRIP = 0063.6");
  u8g.drawStr(0, 47, "BAT V = 12.00");
  u8g.drawStr(0, 55, "22:35:33 THU 09JUN16");

  u8g.drawBox(120, 30, 8, 33);
}

void displaySetup()
{
}

void displayLoop()
{
  static unsigned long msTick = millis();
  if (millis() - msTick >= 250)
  { // run every 250 ms
    msTick = millis();

    u8g.firstPage();
    do
    {
      if (pag_select == 1)
        draw_page_1();
      else if (pag_select == 2)
        draw_page_2();
      else if (pag_select == 3)
        draw_page_3();
    } while (u8g.nextPage());
  }
}
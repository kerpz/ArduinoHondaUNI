#include <Arduino.h>
#include <SoftwareSerialWithHalfDuplex.h>

#include "elm.h"

SoftwareSerialWithHalfDuplex btSerial(10, 11); // RX, TX

bool elm_mode;
bool elm_memory;
bool elm_echo;
bool elm_space;
bool elm_linefeed;
bool elm_header;
uint8_t elm_protocol; // auto

void bt_write(char *str)
{
  char c = *str;
  while (*str != '\0')
  {
    if (!elm_linefeed && *str == 10)
      *str++; // skip linefeed for all reply
    if (c == '4' && !elm_space && *str == 32)
      *str++; // skip space for obd reply
    btSerial.write(*str++);
  }
}

void elmSetup()
{
  btSerial.begin(9600); // HC-05
  // btSerial.begin(38400); // HC-06
}

void elmLoop()
{
  char btdata1[20] = {0}; // bt data in buffer
  char btdata2[20] = {0}; // bt data out buffer
  int i = 0;

  // btSerial.listen();
  while (btSerial.available())
  {
    btdata1[i] = toupper(btSerial.read());
    // Serial.print(btdata1[i]);
    delay(1); // this is required

    if (btdata1[i] == '\r')
    { // terminate at \r
      btdata1[i] = '\0';

      // byte len = strlen(btdata1);
      if (!strcmp(btdata1, "ATD"))
      { // defaults
        elm_echo = false;
        elm_space = true;
        elm_linefeed = true;
        elm_header = false;
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (!strcmp(btdata1, "ATI"))
      { // print id / general
        sprintf_P(btdata2, PSTR("Honda OBD v1.0\r\n>"));
      }
      else if (!strcmp(btdata1, "ATZ"))
      { // reset all / general
        elm_echo = false;
        elm_space = true;
        elm_linefeed = true;
        elm_header = false;
        sprintf_P(btdata2, PSTR("Honda OBD v1.0\r\n>"));
      }
      else if (i == 4 && strstr(btdata1, "ATE"))
      { // echo on/off / general
        elm_echo = (btdata1[3] == '1' ? true : false);
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i == 4 && strstr(btdata1, "ATL"))
      { // linfeed on/off / general
        elm_linefeed = (btdata1[3] == '1' ? true : false);
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i == 4 && strstr(btdata1, "ATM"))
      { // memory on/off / general
        // elm_memory = (btdata1[3] == '1' ? true : false);
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i == 4 && strstr(btdata1, "ATS"))
      { // space on/off / obd
        elm_space = (btdata1[3] == '1' ? true : false);
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i == 4 && strstr(btdata1, "ATH"))
      { // headers on/off / obd
        // elm_header = (btdata1[3] == '1' ? true : false);
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i == 5 && strstr(btdata1, "ATSP"))
      { // set protocol to ? and save it / obd
        // elm_protocol = atoi(btdata1[4]);
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (!strcmp(btdata1, "ATDP"))
      { // display protocol / obd
        sprintf_P(btdata2, PSTR("AUTO\r\n>"));
      }
      else if (!strcmp(btdata1, "ATRV"))
      { // read voltage in float / volts
        // btSerial.print("12.0V\r\n>");
        sprintf_P(btdata2, PSTR("%.1fV\r\n>"), volt2);
      }
      // kerpz custom AT cmd
      else if (i == 6 && strstr(btdata1, "ATSHP"))
      { // set hobd protocol
        if (btdata1[5] == '1')
        {
          obd_select = 1;
        }
        if (btdata1[5] == '2')
        {
          obd_select = 2;
        }
        saveConfig();
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (!strcmp(btdata1, "ATDHP"))
      { // display hobd protocol
        sprintf_P(btdata2, PSTR("HOBD%d\r\n>"), obd_select);
      }
      else if (strstr(btdata1, "ATSAP"))
      { // set arduino pin value (1=hi,0=lo,T=toggle)
        byte pin = ((btdata1[5] > '9') ? (btdata1[5] & ~0x20) - 'A' + 10 : (btdata1[5] - '0') * 16) +
                   ((btdata1[6] > '9') ? (btdata1[6] & ~0x20) - 'A' + 10 : (btdata1[6] - '0'));
        if (btdata1[7] == 'T')
        {
          digitalWrite(pin, !digitalRead(pin));
        }
        else
        {
          digitalWrite(pin, btdata1[7]);
        }

        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (strstr(btdata1, "ATDAP"))
      { // display arduino pin value (1=hi,0=lo)
        byte pin = ((btdata1[5] > '9') ? (btdata1[5] & ~0x20) - 'A' + 10 : (btdata1[5] - '0') * 16) +
                   ((btdata1[6] > '9') ? (btdata1[6] & ~0x20) - 'A' + 10 : (btdata1[6] - '0'));
        sprintf_P(btdata2, PSTR("%d\r\n>"), digitalRead(pin));
      }
      else if (strstr(btdata1, "ATPAP"))
      { // push arduino pin high for 1sec // used for locking/unlocking door
        byte pin = ((btdata1[5] > '9') ? (btdata1[5] & ~0x20) - 'A' + 10 : (btdata1[5] - '0') * 16) +
                   ((btdata1[6] > '9') ? (btdata1[6] & ~0x20) - 'A' + 10 : (btdata1[6] - '0'));
        // pushPinHi(pin, 1000);

        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }

      // https://en.wikipedia.org/wiki/OBD-II_PIDs
      // sprintf_P(cmd_str, PSTR("%02X%02X\r"), mode, pid);
      // sscanf(btdata1, "%02X%02X", mode, pid)
      else if (i == 2 && btdata1[0] == '0' && btdata1[1] == '4')
      { // mode 04
        // clear dtc / stored values
        // reset dtc/ecu honda
        // 21 04 01 DA / 01 03 FC
        // dlcCommand(0x21, 0x04, 0x01, 0x00, dlcdata); // reset ecu
        // resetEcu();
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i == 2 && btdata1[0] == '0' && btdata1[1] == '3')
      { // mode 03
        // request dtc
        // do scan then report the errors
        // 43 01 33 00 00 00 00 = P0133
        // sprintf_P(btdata2, PSTR("SEARCHING...\r\n"));
        // sprintf_P(btdata2, PSTR("UNABLE TO CONNECT\r\n>"));
        sprintf_P(btdata2, PSTR("43 01 33 00 00 00 00\r\n>"));
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i <= 5 && btdata1[0] == '0' && btdata1[1] == '1')
      { // mode 01
        // multi pid 010C0B0D040E05
        if (strstr(&btdata1[2], "00"))
        {
          sprintf_P(btdata2, PSTR("41 00 BE 3E B0 11\r\n>"));
        }
        else if (strstr(&btdata1[2], "01"))
        {
          // dtc / AA BB CC DD / A7 = MIL on/off, A6-A0 = DTC_CNT
        }
        // else if (strstr(&btdata1[2], "02")) { // freeze dtc / 00 61 ???
        //   if (dlcCommand(0x20, 0x05, 0x98, 0x02, dlcdata)) {
        //     sprintf_P(btdata2, PSTR("41 02 %02X %02X\r\n>"), dlcdata[2], dlcdata[3]);
        //   }
        // }
        // else if (strstr(&btdata1[2], "03")) { // fuel system status / 01 00 ???
        // }
        // else if (strstr(&btdata1[2], "04"))
        //{ // engine load (%)
        //  byte v = 0;
        //  v = (v * 255) / 100; // conversion to byte range
        //  sprintf_P(btdata2, PSTR("41 04 %02X\r\n>"), v);
        //}
        else if (strstr(&btdata1[2], "05"))
        { // ect (°C)
          uint8_t v = ect;
          v = v + 40; // conversion
          sprintf_P(btdata2, PSTR("41 05 %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "06"))
        {                              // short FT (%)
          uint8_t v = sft;             // -100 too rich, 99.2 too lean
          v = ((v + 100) * 128) / 100; // conversion
          sprintf_P(btdata2, PSTR("41 06 %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "07"))
        {                              // long FT (%)
          uint8_t v = lft;             // -100 too rich, 99.2 too lean
          v = ((v + 100) * 128) / 100; // conversion
          sprintf_P(btdata2, PSTR("41 07 %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "0A"))
        {                  // fuel pressure
          uint8_t v = 255; // 255 kPa / 37 psi
          v = v / 3;       // conversion
          sprintf_P(btdata2, PSTR("41 0A %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "0B"))
        {                   // map (kPa)
          uint8_t v = maps; // 101 kPa @ off|wot // 10kPa - 30kPa @ idle
          sprintf_P(btdata2, PSTR("41 0B %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "0C"))
        { // rpm
          int v = rpm;
          v = v * 4;                                                                 // conversion
          sprintf_P(btdata2, PSTR("41 0C %02X %02X\r\n>"), highByte(v), lowByte(v)); //((A*256)+B)/4
        }
        else if (strstr(&btdata1[2], "0D"))
        { // vss (km/h)
          uint8_t v = vss;
          sprintf_P(btdata2, PSTR("41 0D %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "0E"))
        { // timing advance (°)
          float f = ign;
          uint8_t v = (f + 64) * 2; // conversion
          sprintf_P(btdata2, PSTR("41 0E %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "0F"))
        { // iat (°C)
          uint8_t v = iat;
          v = v + 40; // conversion
          sprintf_P(btdata2, PSTR("41 0F %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "11"))
        { // tps (%)
          uint8_t v = tps;
          v = (v * 255) / 100; // conversion to byte range
          sprintf_P(btdata2, PSTR("41 11 %02X\r\n>"), v);
        }
        else if (strstr(&btdata1[2], "13"))
        {                                            // o2 sensor present
          sprintf_P(btdata2, PSTR("41 13 80\r\n>")); // 10000000 / assume bank 1 present
        }
        else if (strstr(&btdata1[2], "14"))
        { // o2 (V)
          // sprintf_P(btdata2, PSTR("41 14 %02X FF\r\n>"), dlcdata[2]);
        }
        else if (strstr(&btdata1[2], "1C"))
        {
          sprintf_P(btdata2, PSTR("41 1C 01\r\n>")); // obd2
        }
        else if (strstr(&btdata1[2], "20"))
        {
          sprintf_P(btdata2, PSTR("41 20 00 00 20 01\r\n>")); // pid 33 and 40
        }
        // else if (strstr(&btdata1[2], "2F"))
        //{ // fuel level (%)
        //   uint8_t v = 75;
        //   v = (v * 255) / 100; // conversion to byte range
        //   sprintf_P(btdata2, PSTR("41 2F %02X\r\n>"), v);
        // }
        // else if (strstr(&btdata1[2], "33"))
        //{                  // baro (kPa)
        //   uint8_t v = baro; // 101 kPa
        //   sprintf_P(btdata2, PSTR("41 33 %02X\r\n>"), v);
        // }
        else if (strstr(&btdata1[2], "40"))
        {
          sprintf_P(btdata2, PSTR("41 40 48 00 00 00\r\n>")); // pid 42 and 45
        }
        else if (strstr(&btdata1[2], "42"))
        { // ecu voltage (V)
          float f = volt2;
          unsigned int u = f * 1000; // ((A*256)+B)/1000
          sprintf_P(btdata2, PSTR("41 42 %02X %02X\r\n>"), highByte(u), lowByte(u));
        }
        else if (strstr(&btdata1[2], "45"))
        { // iacv / relative throttle position (%)
          uint8_t v = iacv;
          // v = (v * 255) / 100; // conversion to byte range
          sprintf_P(btdata2, PSTR("41 45 %02X\r\n>"), v);
        }
      }

      if (strlen(btdata2) == 0)
        sprintf_P(btdata2, PSTR("NO DATA\r\n>"));

      bt_write(btdata2); // send reply

      break;
    }
    else if (btdata1[i] != 32 || btdata1[i] != 10)
    { // ignore space and newline
      ++i;
    }
  }
}
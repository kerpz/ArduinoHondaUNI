#include <Arduino.h>
#include <SoftwareSerialWithHalfDuplex.h>

#include "elm.h"

SoftwareSerialWithHalfDuplex btSerial(PIN_RX, PIN_TX); // RX, TX

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
  // btSerial.begin(9600); // HC-05
  btSerial.begin(38400); // HC-06
}

void elmLoop()
{
  char btdata1[20] = {0}; // bt data in buffer
  char btdata2[20] = {0}; // bt data out buffer
  int i = 0;

  btSerial.listen();
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
        sprintf_P(btdata2, "%s\r\n>", APPNAME);
      }
      else if (!strcmp(btdata1, "ATZ"))
      { // reset all / general
        elm_echo = false;
        elm_space = true;
        elm_linefeed = true;
        elm_header = false;
        sprintf_P(btdata2, "%s\r\n>", APPNAME);
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
        // EEPROM.write(0, obd_select);
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
        pushPinHi(pin, 1000);

        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }

      // https://en.wikipedia.org/wiki/OBD-II_PIDs
      // sprintf_P(cmd_str, PSTR("%02X%02X\r"), mode, pid);
      // sscanf(btdata1, "%02X%02X", mode, pid)
      else if (i == 2 && btdata1[0] == '0' && btdata1[1] == '4')
      { // mode 04
        // clear dtc / stored values
        // reset dtc/ecu honda
        resetEcu();
        sprintf_P(btdata2, PSTR("OK\r\n>"));
      }
      else if (i == 2 && btdata1[0] == '0' && btdata1[1] == '3')
      { // mode 03
        // do scan then report the errors
        // 43 01 33 00 00 00 00 = P0133
        // sprintf_P(btdata2, PSTR("43 01 33 00 00 00 00\r\n>"), a);
        // sprintf_P(btdata2, PSTR("OK\r\n>"));
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
          // if (dlcCommand(0x20, 0x05, 0x0B, 0x01)) {
          //  byte v = ((dlcdata[2] >> 5) & 1) << 7; // get bit 5 on dlcdata[2], set it to a7
          //  sprintf_P(btdata2, PSTR("41 01 %02X 00 00 00\r\n>"), v);
          //}
        }
        /*
        else if (strstr(&btdata1[2], "02")) { // freeze dtc / 00 61 ???
          if (dlcCommand(0x20, 0x05, 0x98, 0x02)) {
            sprintf_P(btdata2, PSTR("41 02 %02X %02X\r\n>"), dlcdata[2], dlcdata[3]);
          }
        }
        else if (strstr(&btdata1[2], "03")) { // fuel system status / 01 00 ???
          //if (dlcCommand(0x20, 0x05, 0x0F, 0x01)) { // flags
          //  byte a = dlcdata[2] & 1; // get bit 0 on dlcdata[2]
          //  a = (dlcdata[2] == 1 ? 2 : 1); // convert to comply obd2
          //  sprintf_P(btdata2, PSTR("41 03 %02X 00\r\n>"), a);
          // }
          if (dlcCommand(0x20, 0x05, 0x9a, 0x02)) {
            sprintf_P(btdata2, PSTR("41 03 %02X %02X\r\n>"), dlcdata[2], dlcdata[3]);
          }
        }
        else if (strstr(&btdata1[2], "04")) { // engine load (%)
          if (dlcCommand(0x20, 0x05, 0x9c, 0x01)) {
            sprintf_P(btdata2, PSTR("41 04 %02X\r\n>"), dlcdata[2]);
          }
        }
        */
        else if (strstr(&btdata1[2], "05"))
        { // ect (°C)
          if (dlcCommand(0x20, 0x05, 0x10, 0x01))
          {
            float f = dlcdata[2];
            f = 155.04149 - f * 3.0414878 + pow(f, 2) * 0.03952185 - pow(f, 3) * 0.00029383913 + pow(f, 4) * 0.0000010792568 - pow(f, 5) * 0.0000000015618437;
            dlcdata[2] = round(f) + 40; // A-40
            sprintf_P(btdata2, PSTR("41 05 %02X\r\n>"), dlcdata[2]);
          }
        }
        else if (strstr(&btdata1[2], "06"))
        { // short FT (%)
          if (dlcCommand(0x20, 0x05, 0x20, 0x01))
          {
            sprintf_P(btdata2, PSTR("41 06 %02X\r\n>"), dlcdata[2]);
          }
        }
        else if (strstr(&btdata1[2], "07"))
        { // long FT (%)
          if (dlcCommand(0x20, 0x05, 0x22, 0x01))
          {
            sprintf_P(btdata2, PSTR("41 07 %02X\r\n>"), dlcdata[2]);
          }
        }
        else if (strstr(&btdata1[2], "0A"))
        { // fuel pressure
          //  btSerial.print("41 0A EF\r\n");
          byte b = fp / 3;
          sprintf_P(btdata2, PSTR("41 0A %02X\r\n>"), b);
        }
        else if (strstr(&btdata1[2], "0B"))
        { // map (kPa)
          if (dlcCommand(0x20, 0x05, 0x12, 0x01))
          {
            int i = dlcdata[2] * 0.716 - 5; // 101 kPa @ off|wot // 10kPa - 30kPa @ idle
            sprintf_P(btdata2, PSTR("41 0B %02X\r\n>"), i);
          }
        }
        else if (strstr(&btdata1[2], "0C"))
        { // rpm
          if (dlcCommand(0x20, 0x05, 0x00, 0x02))
          {
            int i = 0;
            if (obd_select == 1)
            {
              i = (1875000 / (dlcdata[2] * 256 + dlcdata[3] + 1)) * 4;
            } // OBD1
            if (obd_select == 2)
            {
              i = (dlcdata[2] * 256 + dlcdata[3]);
            } // OBD2
            // in odb1 rpm is -1
            if (i < 0)
            {
              i = 0;
            }
            sprintf_P(btdata2, PSTR("41 0C %02X %02X\r\n>"), highByte(i), lowByte(i)); //((A*256)+B)/4
          }
        }
        else if (strstr(&btdata1[2], "0D"))
        { // vss (km/h)
          if (dlcCommand(0x20, 0x05, 0x02, 0x01))
          {
            sprintf_P(btdata2, PSTR("41 0D %02X\r\n>"), dlcdata[2]);
          }
        }
        else if (strstr(&btdata1[2], "0E"))
        { // timing advance (°)
          if (dlcCommand(0x20, 0x05, 0x26, 0x01))
          {
            byte b = ((dlcdata[2] - 24) / 2) + 128;
            // byte b = (ign + 64) * 2;
            sprintf_P(btdata2, PSTR("41 0E %02X\r\n>"), b);
          }
        }
        else if (strstr(&btdata1[2], "0F"))
        { // iat (°C)
          if (dlcCommand(0x20, 0x05, 0x11, 0x01))
          {
            float f = dlcdata[2];
            f = 155.04149 - f * 3.0414878 + pow(f, 2) * 0.03952185 - pow(f, 3) * 0.00029383913 + pow(f, 4) * 0.0000010792568 - pow(f, 5) * 0.0000000015618437;
            dlcdata[2] = round(f) + 40; // A-40
            sprintf_P(btdata2, PSTR("41 0F %02X\r\n>"), dlcdata[2]);
          }
        }
        else if (strstr(&btdata1[2], "11"))
        { // tps (%)
          if (dlcCommand(0x20, 0x05, 0x14, 0x01))
          {
            byte b = (dlcdata[2] - 24) / 2;
            sprintf_P(btdata2, PSTR("41 11 %02X\r\n>"), b);
          }
        }
        else if (strstr(&btdata1[2], "13"))
        {                                            // o2 sensor present
          sprintf_P(btdata2, PSTR("41 13 80\r\n>")); // 10000000 / assume bank 1 present
        }
        else if (strstr(&btdata1[2], "14"))
        { // o2 (V)
          if (dlcCommand(0x20, 0x05, 0x15, 0x01))
          {
            sprintf_P(btdata2, PSTR("41 14 %02X FF\r\n>"), dlcdata[2]);
          }
        }
        else if (strstr(&btdata1[2], "1C"))
        {
          sprintf_P(btdata2, PSTR("41 1C 01\r\n>")); // obd2
        }
        else if (strstr(&btdata1[2], "20"))
        {
          sprintf_P(btdata2, PSTR("41 20 00 00 20 01\r\n>")); // pid 33 and 40
        }
        // else if (strstr(&btdata1[2], "2F")) { // fuel level (%)
        //   sprintf_P(btdata2, PSTR("41 2F FF\r\n>")); // max
        // }
        else if (strstr(&btdata1[2], "33"))
        { // baro (kPa)
          if (dlcCommand(0x20, 0x05, 0x13, 0x01))
          {
            int i = dlcdata[2] * 0.716 - 5; // 101 kPa
            sprintf_P(btdata2, PSTR("41 0B %02X\r\n>"), i);
          }
        }
        else if (strstr(&btdata1[2], "40"))
        {
          sprintf_P(btdata2, PSTR("41 40 48 00 00 00\r\n>")); // pid 42 and 45
        }
        else if (strstr(&btdata1[2], "42"))
        { // ecu voltage (V)
          if (dlcCommand(0x20, 0x05, 0x17, 0x01))
          {
            float f = dlcdata[2];
            f = f / 10.45;
            unsigned int u = f * 1000; // ((A*256)+B)/1000
            sprintf_P(btdata2, PSTR("41 42 %02X %02X\r\n>"), highByte(u), lowByte(u));
          }
        }
        else if (strstr(&btdata1[2], "45"))
        { // iacv / relative throttle position (%)
          if (dlcCommand(0x20, 0x05, 0x28, 0x01))
          { // dlcdata[2]
            // byte b = dlcdata[2] / 2.55;
            // b = b * 2.55; // conversion to byte range
            sprintf_P(btdata2, PSTR("41 45 %02X\r\n>"), dlcdata[2]);
          }
        }
      }

      // direct honda PID access
      // 1 byte access (21AA) // 21 = 1 byte, AA = address
      else if (btdata1[0] == '2' && btdata1[1] == '1')
      {
        byte addr = ((btdata1[2] > '9') ? (btdata1[2] & ~0x20) - 'A' + 10 : (btdata1[2] - '0') * 16) +
                    ((btdata1[3] > '9') ? (btdata1[3] & ~0x20) - 'A' + 10 : (btdata1[3] - '0'));
        if (dlcCommand(0x20, 0x05, addr, 0x01))
        {
          sprintf_P(btdata2, PSTR("60 %02X %02X\r\n>"), addr, dlcdata[2]);
        }
      }
      // 2 bytes access (22AA) // 22 = 2 bytes, AA = address
      else if (btdata1[0] == '2' && btdata1[1] == '2')
      {
        byte addr = ((btdata1[2] > '9') ? (btdata1[2] & ~0x20) - 'A' + 10 : (btdata1[2] - '0') * 16) +
                    ((btdata1[3] > '9') ? (btdata1[3] & ~0x20) - 'A' + 10 : (btdata1[3] - '0'));
        if (dlcCommand(0x20, 0x05, addr, 0x02))
        {
          sprintf_P(btdata2, PSTR("60 %02X %02X %02X\r\n>"), addr, dlcdata[2], dlcdata[3]);
        }
      }
      // 4 byte access (24AA) // 24 = 4 bytes, AA = address
      else if (btdata1[0] == '2' && btdata1[1] == '4')
      {
        byte addr = ((btdata1[2] > '9') ? (btdata1[2] & ~0x20) - 'A' + 10 : (btdata1[2] - '0') * 16) +
                    ((btdata1[3] > '9') ? (btdata1[3] & ~0x20) - 'A' + 10 : (btdata1[3] - '0'));
        if (dlcCommand(0x20, 0x05, addr, 0x04))
        {
          sprintf_P(btdata2, PSTR("60 %02X %02X %02X %02X %02X\r\n>"), addr, dlcdata[2], dlcdata[3], dlcdata[4], dlcdata[5]);
        }
      }
      // 2 byte access (25) // for timeout errors
      else if (btdata1[0] == '2' && btdata1[1] == '5')
      {
        sprintf_P(btdata2, PSTR("60 %02X\r\n>"), dlcTimeout);
        // dlcTimeout++;
        // if (dlcTimeout > 255) dlcTimeout = 0;
      }
      // 2 byte access (26) // for checksum errors
      else if (btdata1[0] == '2' && btdata1[1] == '6')
      {
        sprintf_P(btdata2, PSTR("60 %02X\r\n>"), dlcChecksumError);
      }

      if (strlen(btdata2) == 0)
      {
        sprintf_P(btdata2, PSTR("NO DATA\r\n>"));
      }
      bt_write(btdata2); // send reply

      break;
    }
    else if (btdata1[i] != 32 || btdata1[i] != 10)
    { // ignore space and newline
      ++i;
    }
  }
}
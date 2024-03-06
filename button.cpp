#include <Arduino.h>
#include "button.h"

int isButtonPressed = -1; // staes: -1 = floating, 0 = released, 1 = pressed

void button_isr()
{
  isButtonPressed = digitalRead(PIN_BUTTON);
  Serial.println("button changed: " + String(isButtonPressed));
}

void buttonSetup()
{
  pinMode(PIN_BUTTON, INPUT_PULLUP); // Button
  attachInterrupt(PIN_BUTTON, button_isr, CHANGE);
}

void buttonLoop()
{
  static unsigned long buttonTick = 0;

  if (isButtonPressed == 0) // on released
  {
    // pushPinHi(PIN_BUZZER, 30); // beep 30ms
    if (millis() - buttonTick >= 5000)
    { // long press 5 secs
      // scanDtcError();
      // resetECU();
    }
    else if (millis() - buttonTick >= 3000)
    { // long press 3 secs
      if (obd_select == 1)
        obd_select = 2;
      else
        obd_select = 1;
      saveConfig();
    }
    else if (millis() - buttonTick >= 5)
    { // short press 5 ms
      pag_select++;
      if (pag_select > 6)
      {
        pag_select = 1;
      }
      // saveConfig();
    }
    buttonTick = 0;       // reset timer
    isButtonPressed = -1; // floating state
  }
  else if (isButtonPressed == 1) // on pressed
  {
    buttonTick = millis(); // start timer
  }
}

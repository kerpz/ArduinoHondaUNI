#include <Arduino.h>
#include "button.h"

bool button = false;

void button_isr()
{
  button = !button;
  Serial.println("button changed: " + String(button));
}

void buttonSetup()
{
  attachInterrupt(PIN_BUTTON, button_isr, CHANGE);
}

void buttonLoop()
{
  /*
  static unsigned long buttonsTick = 0;
  static bool button_press_old = HIGH;

  int button_press_new = digitalRead(PIN_BUTTON); // change to multiple button

  if (button_press_new != button_press_old)
  { // change state
    if (button_press_new == HIGH)
    { // on released
      // pushPinHi(PIN_BUZZER, 30); // beep 30ms
      if (millis() - buttonsTick >= 5000)
      { // long press 5 secs
        // scanDtcError();
        // resetECU();
      }
      else if (millis() - buttonsTick >= 3000)
      { // long press 3 secs
        obd_select++;
        if (obd_select > 2)
        {
          obd_select = 1;
        }
        // EEPROM.write(0, obd_select);
      }
      else if (millis() - buttonsTick >= 5)
      { // short press 5 ms
        pag_select++;
        if (pag_select > 6)
        {
          pag_select = 1;
        }
        // EEPROM.write(1, pag_select);
      }
      buttonsTick = 0; // reset timer
      isButtonPressed = false;
    }
    else
    {                         // on pressed
      buttonsTick = millis(); // start timer
      isButtonPressed = true;
    }
    button_press_old = button_press_new;
  }
  */
}

ArduinoHondaUNI
===========

An arduino code that reads Honda OBD Protocol via the 3 pin DLC connector.

Please refer to the screenshots below.

Applications
--------
* Datalogging
* Diagnostics
* Trip Computer
* Custom Cluster
* Etc


Supports
--------
* Honda ECU's before 2002


Files
-----
* ArduinoHondaUNI - top level code.
* app - implements Honda OBD protocol
* eeprom - implements storage
* display - implements display
* elm - implements the translation to elm protocol (serial)
* button - implements single button


Wiring for ECU connection
--------------------
    Honda 3 Pin DLC           Arduino Nano
    Gnd --------------------- Gnd
    +12 --------------------- Vin
    K-line ------------------ Pin12

Wiring for button
--------------------
    Tact Switch               Arduino Nano
    (+) --------------------- Pin17 (A3)
    (-) --------------------- Gnd

Wiring for buzzer (beep)
--------------------
    Piezo Buzzer              Arduino Nano
    (+) --------------------- Pin13
    (-) --------------------- Gnd

Wiring for ELM (Bluetooth hc-05/hc-06)
--------------------
    HC-05 Bluetooth           Arduino Nano
    Rx ---------------------- Pin11
    Tx ---------------------- Pin10

Wiring for display option 1 (lcd1602 i2c)
--------------------
    LCD 16x2                  Arduino Nano
    Sda --------------------- Pin18 (A4)
    Scl --------------------- Pin19 (A5)
    Vcc --------------------- 5v
    Gnd --------------------- Gnd

Wiring for display option 2 (lcd128x64 spi)
--------------------
    LCD 128x64                Arduino Nano
    D4 ---------------------- Pin7
    D5 ---------------------- Pin6
    D6 ---------------------- Pin5

Wiring for display option 3 (lcd1602 Parallel)
--------------------
    LCD 16x2                  Arduino Uno               
    RS ---------------------- Pin9
    Enable ------------------ Pin8
    D4 ---------------------- Pin7
    D5 ---------------------- Pin6
    D6 ---------------------- Pin5
    D7 ---------------------- Pin4
    VO ---------------------- 10k Potentiometer (+5V to Gnd)

Wiring for voltage sensor
--------------------
    Voltage Divider           Arduino Nano
    +12V divider circuit ---- Pin14 (A0)
    (680k ohms and 220k ohms)
    

User Manual
-----
* Button
  - Short press to change page
  - Hold 3 seconds to change mode (obd1/obd2)
  - Hold 5 seconds to reset the ecu
* Display
* ELM Bluethoth
  - using an app (Torque or similar) connect via bluetooth (mode obd2)
  - direct honda PID access
    - 1 byte access pid "21AA" where: 2 = custom header, 1 = 1 byte, AA = address
    - 2 bytes access pid "22AA" where: 2 = custom header, 2 = 2 bytes, AA = address
    - 4 byte access pid "24AA" where: 2 = custom header, 4 = 4 bytes, AA = address
* Unit Measurement
  - rpm
  - kph
  - km
  - psi
  - volt
  - degree

Note
-----
* Tested on P2T ODB2 stock and P30 ODB1 chipped

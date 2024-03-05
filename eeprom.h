#ifndef EEPROM_H
#define EEPROM_H

extern uint8_t obd_select; // 1 = obd1 or 2 = obd2

extern uint8_t ect_alarm;    // celcius
extern uint8_t vss_alarm;    // kph / + 10
extern uint8_t th_threshold; // celcius / + 10

// peripheral devices
extern uint8_t beep_enable;
extern uint8_t elm_enable;
extern uint8_t display_enable;
extern uint8_t afr_enable;

void loadConfig();
void saveConfig();

#endif
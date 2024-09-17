#ifndef LCD_H
#define LCD_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void check_pin();
void printEnteredPin();
void updateLCD();

#endif //LCD_H
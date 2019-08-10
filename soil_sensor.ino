/**
 * @file 
 * Single soil probe sensor for individually evaluating a plant
 * @copyright
 * Copyright (c) 2019 Joshua Scoggins 
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_seesaw.h"
#include <LiquidCrystal.h>

Adafruit_seesaw ss;
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
constexpr int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// comment next line to enable serial console output
#define NO_SERIAL

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
#ifndef NO_SERIAL
  Serial.begin(9600);
  Serial.println("Bringing up display");
#endif
  lcd.begin(16,2);
  if (!ss.begin(0x36)) {
    lcd.print("ERROR! no seesaw");    
    while(1);
  } 
  
  lcd.setCursor(0,0);
  lcd.print("Moisture: ");
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
}

// the loop function runs over and over again forever
void loop() {  
  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);
  lcd.setCursor(10,0);
  lcd.print("      ");
  lcd.setCursor(10,0);
  lcd.print(capread);
  lcd.setCursor(6,1);  
  lcd.print("          ");
  lcd.setCursor(6,1);  
  lcd.print(tempC);


  delay(500);
  
}

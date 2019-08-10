/*
 * garden_management
 * Copyright (c) 2019, Joshua Scoggins
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// Single soil probe sensor for individually evaluating a plant
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
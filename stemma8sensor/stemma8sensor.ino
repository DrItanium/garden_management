/**
 * @file 
 * hook up an i2c expander to 8 stemma soil sensors and a OLED display
 * basis of this code is the SSD1306 OLED display example. Code has been 
 * adapted to be a little more robust in case of accidental disconnect.
 * 
 * This is being used in my garden to track the states of 8 different plants
 * (mostly peppers). I wish the stemma sensor returned values greater than 1016
 * but that seems to be a limitation of the seesaw library.
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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_seesaw.h"

Adafruit_seesaw ss[8];
bool ssActive[8];
constexpr auto TCAADDR = 0x70;
constexpr auto SEESAW_ADDR = 0x36;
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void tcaselect(uint8_t i) {
  if (i > 7) {
    return;
  }
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

// grrr, can't use a template here since I get compiler errors
using SeesawFunc = void (*)(Adafruit_seesaw&, int);
void foreachSeesaw(SeesawFunc fn) {
    for (int i = 0; i < 8; ++i) {
        tcaselect(i);
        fn(ss[i], i);
    }
}

void setup()   { 
  foreachSeesaw([](Adafruit_seesaw& ss, int index) { ssActive[index] = ss.begin(SEESAW_ADDR); });
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  delay(2000);
  display.clearDisplay();
}


void loop() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  foreachSeesaw([](Adafruit_seesaw& ss, int i) {
      display.print("Sensor ");
      display.print(i);
      display.print(": ");
      if (ssActive[i]) {
        auto moisture = ss.touchRead(0);
        if (moisture == 0xFFFF) {
          display.println("no connect!");
        } else {
          display.println(moisture);
        }
      } else {
        ssActive[i] = ss.begin(SEESAW_ADDR);
        display.println("inactive!!!");
      }
  });
  display.display();
  delay(500);
}

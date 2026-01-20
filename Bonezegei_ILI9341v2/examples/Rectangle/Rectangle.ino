/*
  Author: Bonezegei (Jofel Batutay)
  Date: July 2023 

  Rectangle Sample For Bonezegei
  This library only support 18bit color format (0xRRGGBB) example RED = 0xFF0000, BLUE = 0x0000FF
  The LCD is Connected to the VSPI of ESP32
  ------------------------
  |  LCD PIN   |  ESP32  |
  | MOSI       |   23    |
  | SCK        |   18    |
  | MISO       |   19    | Optional ( not Necessarily Connected to the LCD when mode is diplay Only)
  | CS         |   4     | Can be Assign to other pin
  | RST        |   16    | Can be Assign to other pin
  | DC         |   17    | Can be Assign to other pin
  ------------------------
    LED of the LCD can be connected to the 3.3V with 100 Ohm resitor

  [ IMPORTANT ] Install with Bonezegei_Utility this library is dependent on that utility. 
*/

#include <Bonezegei_ILI9341v2.h>
#include <Bonezegei_Utility.h>

#define LCD_DC 17
#define LCD_CS 4
#define LCD_RST 16
Bonezegei_ILI9341v2 lcd(LCD_RST, LCD_CS, LCD_DC);

Bonezegei_Display *display;


void setup() {

  Serial.begin(115200);

  display = &lcd;
  display->begin();
  display->setRotation(1);
  display->clear(COLOR_WHITE);
  delay(100);

  display->setFont(FONT_ARIAL_14);
  display->drawText(Point(10, 10), "Bonezegei ILI9341 Rectangle", COLOR_BLACK);

  Rect r1 = Rect(10, 30, 60, 80);
  display->drawRectangle(r1, COLOR_TEAL);

  Rect r2 = Rect(10, 90, 60, 140);
  //param1 true to set the rectangle filled
  display->drawRectangle(true,r2, COLOR_TEAL);

  display->setFont(FONT_ARIAL_8);
  display->drawText(Point(10, 160), "For more details about rectangle:", COLOR_GRAY);
  display->drawText(Point(10, 170), "visit: https//bonezegei.com", COLOR_BLUE);
}

void loop() {
}

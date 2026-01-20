/*
  Author: Bonezegei (Jofel Batutay)
  Date: July 2023 

  Sample 1 For Bonezegei
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
  display->drawText(Point(10, 10), "Bonezegei ILI9341 Font Clip", COLOR_BLACK);

  Rect r = Rect(10, 30, 310, 230);
  display->setFont(FONT_ARIAL_12);
  display->drawRectangle(r, COLOR_GRAY);
  //param1 set to true to set the word to new line if it does not fit to the space between the last wor to the right end of the rectangle
  //       if param 1 is set to false or the param1 is rectangle the word will be cut 
  //param2 rectangle
  //param3 text
  //param4 color
  display->drawText(true,r, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu", COLOR_BLACK);

}

void loop() {
}

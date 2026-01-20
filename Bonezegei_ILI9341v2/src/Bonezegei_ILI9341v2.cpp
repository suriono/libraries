/*
  This Library is written for ILI9341 LCD
  Author: Bonezegei (Jofel Batutay)
  Date: July 2023 
*/

#include "Bonezegei_ILI9341v2.h"


Bonezegei_ILI9341v2::Bonezegei_ILI9341v2() {
}

Bonezegei_ILI9341v2::Bonezegei_ILI9341v2(int8_t rst, int8_t cs, int8_t dc) {
  _rst = rst;
  _cs = cs;
  _dc = dc;
}

void Bonezegei_ILI9341v2::begin() {
  setFontParam(arial_8ptBitmaps, arial_8ptDescriptors);
  vspi = new SPIClass(VSPI);
  vspi->begin();
  vspi->setFrequency(ILI9341_SPISPEED);
  vspi->setDataMode(SPI_MODE0);
  vspi->setBitOrder(MSBFIRST);

  pinMode(_dc, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_rst, OUTPUT);
  digitalWrite(_cs, HIGH);
  digitalWrite(_dc, HIGH);
  digitalWrite(_rst, HIGH);

  digitalWrite(_rst, LOW);
  delay(200);
  digitalWrite(_rst, HIGH);
  delay(2);
  sendCommand(0x11);
  delay(2);
  sendCommand(0x3a);
  delay(2);
  sendCommand(0x06);
  delay(2);
  sendCommand(0x29);
  delay(2);
  sendCommand(0x3A);
  sendData(0x66);
  delay(2);  //16 bit= 0x55   18bit = 0x66
  setRotation(1);
  clear(0);
}


void Bonezegei_ILI9341v2::setRotation(uint8_t rotation) {

  switch (rotation) {
    case 0:
      sendCommand(0x36);
      sendData(0x00);
      break;
    case 1:
      sendCommand(0x36);
      sendData(0x80);
      break;
    case 2:
      sendCommand(0x36);
      sendData(0x40);
      break;
    case 3:
      sendCommand(0x36);
      sendData(0xC0);
      break;
    default:
      sendCommand(0x36);
      sendData(0x00);
  }
}

void Bonezegei_ILI9341v2::drawPixel(uint16_t x, uint16_t y, uint32_t color) {
  digitalWrite(_cs, LOW);

  digitalWrite(_dc, LOW);
  spiWriteByteNL(vspi->bus(), 0x2b);
  digitalWrite(_dc, HIGH);
  spiWriteShortNL(vspi->bus(), x);
  spiWriteShortNL(vspi->bus(), 320);

  digitalWrite(_dc, LOW);
  spiWriteByteNL(vspi->bus(), 0x2a);
  digitalWrite(_dc, HIGH);
  spiWriteShortNL(vspi->bus(), y);
  spiWriteShortNL(vspi->bus(), 240);

  digitalWrite(_dc, LOW);
  spiWriteByteNL(vspi->bus(), 0x2c);
  digitalWrite(_dc, HIGH);
  spi24(color);
  digitalWrite(_cs, HIGH);
}

void Bonezegei_ILI9341v2::sendCommand(uint8_t cmd) {
  digitalWrite(_cs, LOW);
  digitalWrite(_dc, LOW);
  spiWriteByteNL(vspi->bus(), cmd);
  digitalWrite(_dc, HIGH);
  digitalWrite(_cs, HIGH);
}

void Bonezegei_ILI9341v2::sendData(uint8_t dat) {
  digitalWrite(_cs, LOW);
  spiWriteByteNL(vspi->bus(), dat);
  digitalWrite(_cs, HIGH);
}

void Bonezegei_ILI9341v2::col(uint16_t st, uint16_t en) {
  sendCommand(0x2a);
  sendData(st >> 8);
  sendData(st);
  sendData(en >> 8);
  sendData(en);
}

void Bonezegei_ILI9341v2::row(uint16_t st, uint16_t en) {
  sendCommand(0x2b);
  sendData(st >> 8);
  sendData(st);
  sendData(en >> 8);
  sendData(en);
}

void Bonezegei_ILI9341v2::spi24(uint32_t dat) {
  spiWriteShortNL(vspi->bus(), dat >> 8);
  spiWriteByteNL(vspi->bus(), dat);
}

void Bonezegei_ILI9341v2::spi16(uint16_t dat) {
  spiWriteShortNL(vspi->bus(), dat);
}

void Bonezegei_ILI9341v2::clear(uint32_t color) {
  vspi->setFrequency(ILI9341_SPISPEED);
  col(0, 240);
  row(0, 320);
  sendCommand(0x2c);
  digitalWrite(_cs, LOW);
  for (int a = 0; a < 76800; a++) {
    spi24(color);
  }
  digitalWrite(_cs, HIGH);
}

void Bonezegei_ILI9341v2::drawFilledRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {

  int total = (1 + (x2 - x1)) * (1 + (y2 - y1));
  vspi->setFrequency(ILI9341_SPISPEED);
  col(y1, y2);
  row(x1, x2);
  sendCommand(0x2c);
  digitalWrite(_cs, LOW);
  for (int a = 0; a < total; a++) {
    spi24(color);
  }
  digitalWrite(_cs, HIGH);
}

void Bonezegei_ILI9341v2::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
  drawFilledRectangle(x1, y1, x2, y1, color);
  drawFilledRectangle(x1, y2, x2, y2, color);
  drawFilledRectangle(x1, y1, x1, y2, color);
  drawFilledRectangle(x2, y1, x2, y2, color);
}

void Bonezegei_ILI9341v2::drawRectangleStrip(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
  unsigned char mod = 2;
  for (int a = 0; a < (x2 - x1); a++) {
    if ((a % mod) == 0) drawPixel(x1 + a, y1, color);
  }
  for (int a = 0; a < (x2 - x1); a++) {
    if ((a % mod) == 0) drawPixel(x1 + a, y2, color);
  }
  for (int a = 0; a < (y2 - y1); a++) {
    if ((a % mod) == 0) drawPixel(x1, y1 + a, color);
  }
  for (int a = 0; a < (y2 - y1); a++) {
    if ((a % mod) == 0) drawPixel(x2, y1 + a, color);
  }
}

void Bonezegei_ILI9341v2::drawCircle(uint16_t x1, uint16_t y1, uint16_t r, uint32_t color) {

  //if (x1 < 0) return;
  //if (y1 < 0) return;
  //if (r <= 0) return;

  float c = 4 * 3.1416 * r;
  for (int a = 0; a < ((int)c); a++) {
    drawPixel(ceil(x1 + (cos(a) * r)), ceil(y1 + (sin(a) * r)), color);
  }
}

void Bonezegei_ILI9341v2::drawBitmap(uint16_t x1, uint16_t y1, int xbytes, int yheight, const char bitmap[], uint32_t color) {
  int cnt = 0;
  for (int b = 0; b < yheight; b++) {
    for (int a = 0; a < xbytes; a++) {
      setChar8(x1 + (a * 8), y1 + b, bitmap[cnt], 8, color);
      cnt++;
    }
  }
}


void Bonezegei_ILI9341v2::drawRoundFilledRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t r, uint32_t color) {
  int x, y, xd;

  if (x2 < x1) {
    x = x2;
    x2 = x1;
    x1 = x;
  }
  if (y2 < y1) {
    y = y2;
    y2 = y1;
    y1 = y;
  }

  if (r <= 0) return;

  xd = 3 - (r << 1);
  x = 0;
  y = r;

  drawFilledRectangle(x1 + r, y1, x2 - r, y2, color);

  while (x <= y) {
    if (y > 0) {
      drawFilledRectangle(x2 + x - r, y1 - y + r, x2 + x - r, y + y2 - r, color);
      drawFilledRectangle(x1 - x + r, y1 - y + r, x1 - x + r, y + y2 - r, color);
    }
    if (x > 0) {
      drawFilledRectangle(x1 - y + r, y1 - x + r, x1 - y + r, x + y2 - r, color);
      drawFilledRectangle(x2 + y - r, y1 - x + r, x2 + y - r, x + y2 - r, color);
    }
    if (xd < 0) {
      xd += (x << 2) + 6;
    } else {
      xd += ((x - y) << 2) + 10;
      y--;
    }
    x++;
  }
}


void Bonezegei_ILI9341v2::setChar8(uint16_t x, uint16_t y, char ch, int bits, uint32_t color) {
  int b = 0;
  for (int a = 7; a > (7 - bits); a--) {
    if (ch & (1 << a)) drawPixel(x + b, y, color);
    b++;
  }
}

void Bonezegei_ILI9341v2::drawChar(int x, int y, char ch, uint32_t color, const char fd[], const int dsc[95][3]) {
  int b = ch - 32;
  int xLimit = 320;
  //int yLimit = 240;

  if (xRun >= (xLimit - 8)) {
    xRun = 0;
    yRun += dsc[b][1] + 2;
  }

  if (dsc[b][0] <= 8) {
    for (int a = 0; a < dsc[b][1]; a++) {
      setChar8(xRun, yRun + a, fd[(dsc[b][2] + a)], dsc[b][0], color);
    }
  } else {
    for (int a = 0; a < (dsc[b][1] * 2); a++) {
      if ((a % 2) == 0) setChar8(xRun, yRun + (a / 2), fd[(dsc[b][2] + a)], 8, color);
      else setChar8(xRun + 8, yRun + (a / 2), fd[(dsc[b][2] + a)], dsc[b][0] - 7, color);
    }
  }
  xRun += dsc[b][0] + 2;
}

void Bonezegei_ILI9341v2::drawText(int x, int y, const char *str, uint32_t color, const char fd[], const int dsc[95][3]) {
  xRun = x;
  yRun = y;
  vspi->setFrequency(ILI9341_SPISPEED);
  while (*str) {
    drawChar(x, y, *str, color, fd, dsc);
    str += 1;
  }
}

void Bonezegei_ILI9341v2::drawText(int x, int y, const char *str, uint32_t color) {
  xRun = x;
  yRun = y;
  vspi->setFrequency(ILI9341_SPISPEED);
  while (*str) {
    drawChar(x, y, *str, color, font.fnt, font.descriptor);
    str += 1;
  }
}


void Bonezegei_ILI9341v2::setFontParam(const char fd[], const int dsc[95][3]) {
  font.fnt = (char *)fd;
  for (int b = 0; b < 95; b++) {
    for (int a = 0; a < 3; a++) {
      font.descriptor[b][a] = dsc[b][a];
    }
  }
}

void Bonezegei_ILI9341v2::setFont(FONT_TYPE ft) {

  switch (ft) {

    case FONT_ARIAL_8:
      setFontParam(arial_8ptBitmaps, arial_8ptDescriptors);
      break;
    case FONT_ARIAL_10:
      setFontParam(arial_10ptBitmaps, arial_10ptDescriptors);
      break;
    case FONT_ARIAL_12:
      setFontParam(arial_12ptBitmaps, arial_12ptDescriptors);
      break;
    case FONT_ARIAL_14:
      setFontParam(arial_14ptBitmaps, arial_14ptDescriptors);
      break;
    case FONT_ARIAL_16:
      setFontParam(arial_16ptBitmaps, arial_16ptDescriptors);
      break;

    case FONT_ARIAL_8_BOLD:
      setFontParam(arial_8ptBoldBitmaps, arial_8ptBoldDescriptors);
      break;
	case FONT_ARIAL_10_BOLD:
      setFontParam(arial_10ptBoldBitmaps, arial_10ptBoldDescriptors);
      break;
	case FONT_ARIAL_12_BOLD:
      setFontParam(arial_12ptBoldBitmaps, arial_12ptBoldDescriptors);
      break;
	  
    default:
      setFontParam(arial_8ptBitmaps, arial_8ptDescriptors);
  }
}


/*
Clipped
*/

void Bonezegei_ILI9341v2::setCharClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, uint16_t x, uint16_t y, char ch, int bits, uint32_t color) {
  int b = 0;
  for (int a = 7; a > (7 - bits); a--) {
    if (ch & (1 << a)) {
      int xb = x + b;
      if ((xb > cx1) && (xb < cx2) && (y > cy1) && (y < cy2)) drawPixel(xb, y, color);
    }
    b++;
  }
}


void Bonezegei_ILI9341v2::drawCharClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, int x, int y, char ch, uint32_t color, const char fd[], const int dsc[95][3]) {
  int b = ch - 32;
  int xLimit = cx2;
  //int yLimit = 240;

  if (xRun >= (xLimit - 8)) {
    xRun = cx1 + 1;
    yRun += dsc[b][1] + 2;
  }

  if (dsc[b][0] <= 8) {
    for (int a = 0; a < dsc[b][1]; a++) {
      setCharClipped(cx1, cy1, cx2, cy2, xRun, yRun + a, fd[(dsc[b][2] + a)], dsc[b][0], color);
    }
  } else {
    for (int a = 0; a < (dsc[b][1] * 2); a++) {
      if ((a % 2) == 0) setCharClipped(cx1, cy1, cx2, cy2, xRun, yRun + (a / 2), fd[(dsc[b][2] + a)], 8, color);
      else setCharClipped(cx1, cy1, cx2, cy2, xRun + 8, yRun + (a / 2), fd[(dsc[b][2] + a)], dsc[b][0] - 7, color);
    }
  }
  xRun += dsc[b][0] + 2;
}


void Bonezegei_ILI9341v2::drawTextClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, int x, int y, const char *str, uint32_t color, const char fd[], const int dsc[95][3]) {
  xRun = x;
  yRun = y;
  vspi->setFrequency(ILI9341_SPISPEED);
  while (*str) {
    drawCharClipped(cx1, cy1, cx2, cy2, x, y, *str, color, fd, dsc);
    str += 1;
  }
}

void Bonezegei_ILI9341v2::drawTextClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, int x, int y, const char *str, uint32_t color) {
  xRun = x;
  yRun = y;
  vspi->setFrequency(ILI9341_SPISPEED);
  while (*str) {
    drawCharClipped(cx1, cy1, cx2, cy2, x, y, *str, color, font.fnt, font.descriptor);
    str += 1;
  }
}

void Bonezegei_ILI9341v2::drawTextClippedNL(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, int x, int y, const char *str, uint32_t color) {
  xRun = x;
  yRun = y;
  int nextWordStart = 0;

  char buffWord[32];
  int buffWordCount = 0;

  vspi->setFrequency(ILI9341_SPISPEED);

  int strCount = 0;

  while (str[strCount]) {

    if (strCount == nextWordStart) {
      buffWordCount = 0;
      //for(int a=0; a<32; a++)buffWord[a]=0;
      for (int a = nextWordStart; a < (nextWordStart + 32); a++) {
        if (str[a] > 32) {
          buffWord[buffWordCount] = str[a];

          buffWordCount++;
        } else {
          a = nextWordStart + 32;
          buffWord[buffWordCount] = 0;
          // Serial.print(" Word:");
          int w = getStringWidth((char *)buffWord);
          //drawRectangle(xRun, yRun, xRun + w, yRun + font.descriptor[0][1], COLOR_TEAL);

          if ((w + 8) >= (cx2 - xRun)) {
             xRun = x + 1;
            yRun += font.descriptor[0][1] + 2;
          }
        }
        nextWordStart++;
      }

      /*       Serial.print(" Word:");
      String b = "";
      b += buffWord;
      Serial.println(b); */
    }

    drawCharClipped(cx1, cy1, cx2, cy2, x, y, str[strCount], color, font.fnt, font.descriptor);

    strCount++;
  }
}



void Bonezegei_ILI9341v2::drawBitmapClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, uint16_t x1, uint16_t y1, int xbytes, int yheight, const char bitmap[], uint32_t color) {
  int cnt = 0;
  for (int b = 0; b < yheight; b++) {
    for (int a = 0; a < xbytes; a++) {

      setCharClipped(cx1, cy1, cx2, cy2, x1 + (a * 8), y1 + b, bitmap[cnt], 8, color);
      cnt++;
    }
  }
}


void Bonezegei_ILI9341v2::drawPixelClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, uint16_t x, uint16_t y, uint32_t color) {


  if ((x > cx1) && (x < cx2) && (y > cy1) && (y < cy2)) drawPixel(x, y, color);
}

void Bonezegei_ILI9341v2::drawFilledRectangleClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
/*   int w = x2 - x1;
  int h = y2 - y1;
  for (int b = 0; b < h; b++) {
    for (int a = 0; a < w; a++) {
      drawPixelClipped(cx1, cy1, cx2, cy2, x1 + a, y1 + b, color);
    }
  }
   */
   

  
  int x1Lim = 0;
  int x2Lim = 0;
  int y1Lim = 0; 
  int y2Lim = 0; 
   
  if(x1<cx1){
	  x1Lim = cx1;}
  else{
	  x1Lim = x1;}
  if(x2>cx2){
	  x2Lim = cx2;}
  else{
	  x2Lim = x2;}
  if(y1<cy1){
	  y1Lim = cy1;}
  else{
	  y1Lim = y1;}
  if(y2>cy2){
	  y2Lim = cy2;}
  else{
	  y2Lim = y2;}	
  
  int total = (1 + (x2Lim - x1Lim)) * (1 + (y2Lim - y1Lim));
	
  vspi->setFrequency(ILI9341_SPISPEED);
  col(y1Lim, y2Lim);
  row(x1Lim, x2Lim);
  sendCommand(0x2c);
  digitalWrite(_cs, LOW);
  for (int a = 0; a < total; a++) {
    spi24(color);
  }
  digitalWrite(_cs, HIGH);
  
  
}

void Bonezegei_ILI9341v2::drawRectangleClipped(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
  drawFilledRectangleClipped(cx1, cy1, cx2, cy2, x1, y1, x2, y1, color);
  drawFilledRectangleClipped(cx1, cy1, cx2, cy2, x1, y2, x2, y2, color);
  drawFilledRectangleClipped(cx1, cy1, cx2, cy2, x1, y1, x1, y2, color);
  drawFilledRectangleClipped(cx1, cy1, cx2, cy2, x2, y1, x2, y2, color);
}


uint16_t Bonezegei_ILI9341v2::getStringTotalHeight(uint16_t cx1, uint16_t cy1, uint16_t cx2, uint16_t cy2, int x, int y, const char *str){
	  xRun = x;
  yRun = y;
  int nextWordStart = 0;

  char buffWord[32];
  int buffWordCount = 0;

  vspi->setFrequency(ILI9341_SPISPEED);

  int strCount = 0;
  int h=0;

  while (str[strCount]) {

    if (strCount == nextWordStart) {
      buffWordCount = 0;
      //for(int a=0; a<32; a++)buffWord[a]=0;
      for (int a = nextWordStart; a < (nextWordStart + 32); a++) {
        if (str[a] > 32) {
          buffWord[buffWordCount] = str[a];

          buffWordCount++;
        } else {
          a = nextWordStart + 32;
          buffWord[buffWordCount] = 0;

          int w = getStringWidth((char *)buffWord);

          if ((w + 8) >= (cx2 - xRun)) {
            xRun = x + 1;
            yRun += font.descriptor[0][1] + 2;
			h += font.descriptor[0][1] + 2;
          }
        }
        nextWordStart++;
      }

    }
	  int b = str[strCount] - 32;
	  int xLimit = cx2;
	  //int yLimit = 240;

	  if (xRun >= (xLimit - 8)) {
		xRun = cx1 + 1;
		yRun += font.descriptor[b][1] + 2;
	  }

	  if (font.descriptor[b][0] <= 8) {

	  } else {

	  }
	  xRun += font.descriptor[b][0] + 2;
	

    strCount++;
  }
  
  return h;
}

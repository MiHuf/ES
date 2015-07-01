#ifndef NOKIALCD_H
#define NOKIALCD_H

#include <inttypes.h>

#define LCD_RESX 84
#define LCD_RESY 48
#define LCD_BANKS 6
#define LCD_BUFSIZE 504

class NokiaLCD
{
public:
  NokiaLCD();
  void init(uint8_t pinSCE, uint8_t pinRES, uint8_t pinDC);
  bool setPixel(uint8_t pX, uint8_t pY, uint8_t onoff);
  bool writeChar(uint8_t pX, uint8_t pY, const char ch);
  bool writeLine(uint8_t pX, uint8_t pY, const char *str);
  void clearBuffer();
  void displayBuffer();
  
private:
  uint8_t pinSCE_;
  uint8_t pinDC_;
  uint8_t buffer[LCD_BUFSIZE];
  
  void writeCommand(uint8_t cmd);
  void writeData(uint8_t data);
};

#endif // NOKIALCD_H

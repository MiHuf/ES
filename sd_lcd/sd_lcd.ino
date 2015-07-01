#include "nokialcd.h"
#include <SPI.h>
#include <SD.h>

NokiaLCD LCD;
uint8_t enable = 0;

bool getImageDims(char *file, uint8_t &width, uint8_t &height)
{
  bool result = false;
  if (SD.exists(file))
  {
    File img = SD.open(file, FILE_READ);
    if (img)
    {
      char dims[10] = { 0 };
      for (uint8_t i = 0; i < 10; i++)
      {
        dims[i] = img.read();
        if (dims[i] == '\n' || dims[i] == -1)
        {
          dims[i] = 0;
          break;
        }
      }
      if (sscanf(dims, "%d,%d", &width, &height) == 2) { result = true; }
      img.close();
    }
  }
  return result;
}

bool displayImage(char *file, uint8_t pX, uint8_t pY)
{
  uint8_t width, height;
  if (!getImageDims(file, width, height)) { return false; }
  
  bool result = false;
  if (SD.exists(file))
  {
    File img = SD.open(file, FILE_READ);
    if (img)
    {
      while (img.read() != '\n'); // Skip dimensions
      uint16_t index = 0;
      char ch;
      while ((ch = img.read()) != '\n')
      {
        switch (ch)
        {
          case '0':
          {
            LCD.setPixel((index % width) + pX, (index / width) + pY, 0);
            index++;
            break;
          }
          case '1':
          {
            LCD.setPixel((index % width) + pX, (index / width) + pY, 1);
            index++;
            break;
          }
          default: { break; }
        }
      }
      LCD.displayBuffer();
      result = true;
      img.close();
    }
  }
  return result;
}

void setup()
{
  SD.begin(4);
  
  LCD.init(10, 2, 3);
  const char *welcome = "Welcome!";
  uint8_t xPos = (LCD_RESX / 2) - (strlen(welcome) * 6 / 2);
  uint8_t yPos = (LCD_RESY / 2) - 4;
  LCD.writeLine(xPos, yPos, welcome);
  LCD.displayBuffer();
  delay(5000);
  
  Serial.begin(9600);
}

void loop()
{
//  for (uint8_t col = 0; col < LCD_RESX; col++)
//  {
//    for (int8_t row = 0; row < LCD_RESY; row++)
//    {
//      LCD.setPixel(col, row, enable ? 1 : 0);
//    }
//    LCD.displayBuffer();
//    delay(20);
//  }
//  enable = enable ? 0 : 1;

  LCD.clearBuffer();
  LCD.displayBuffer();
  
  uint8_t width, height;
  if (getImageDims("smile3.img", width, height))
  {    
    if (displayImage("smile3.img", LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2))
    {
      Serial.println("smile3.img should be on display");
    }
  }
  
  delay(2500);
  LCD.clearBuffer();
  LCD.displayBuffer();
  
  if (getImageDims("tams.img", width, height))
  {    
    if (displayImage("tams.img", LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2))
    {
      Serial.println("tams.img should be on display");
    }
  }
  
  delay(2500);
  LCD.clearBuffer();
  LCD.displayBuffer();
  
  if (getImageDims("smile2.img", width, height))
  {    
    if (displayImage("smile2.img", LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2))
    {
      Serial.println("smile2.img should be on display");
    }
  }
  
  delay(2500);
  LCD.clearBuffer();
  LCD.displayBuffer();
  
  if (getImageDims("smile1.img", width, height))
  {
    if (displayImage("smile1.img", LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2))
    {
      Serial.println("smile1.img should be on display");
    }
  }
  
  delay(5000);
  
  for (uint8_t col = 0; col < LCD_RESX; col++)
  {
    for (int8_t row = 0; row < LCD_RESY; row++)
    {
      LCD.setPixel(col, row, enable ? 1 : 0);
    }
    LCD.displayBuffer();
    delay(20);
  }
  
  delay(1000);
}

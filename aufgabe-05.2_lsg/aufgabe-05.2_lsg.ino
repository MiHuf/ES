// Aufgabe 5.2, Stand von 2015-06-17
// Lösung von Michael Hufschmidt , 6436122 , michael@hufschmidt-web.de,
//            Tim Welge          , 6541929 , tw@ens-fiti.de
//            Rania Wittenberg   , 6059892 , rania_wittenberg@hotmail.com

#include <SPI.h>

// Pin Constants
const int pinCS0 = 10;                      // SPI: Chip Select 0  <<< used here
// const int pinCS1 = 4;                    // SPI: Chip Select 1
// const int pinCS2 = 52;                   // SPI: Chip Select 2
const int pinDC = 5;                        // SPI: D/C = Data / Control
const int pinRST = 6;                       // SPI: RST = Reset
const int pinLcdLed = 4;                    // LED on LCD
const int pinLed = 13;                      // internal LED
const int pinMosi = 109;                    // SPI-Block: MOSI
const int pinClock = 110;                   // SPI-Block: Clock

static const byte byteBits[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
      
unsigned char font[95][6] = {               // charset 6x8_ascii_data.txt
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // space
{ 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00 }, // !
{ 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 }, // "
{ 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00 }, // #
{ 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00 }, // $
{ 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 }, // %
{ 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 }, // &
{ 0x00, 0x00, 0x07, 0x00, 0x00, 0x00 }, // '
{ 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00 }, // (
{ 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00 }, // )
{ 0x0A, 0x04, 0x1F, 0x04, 0x0A, 0x00 }, // *
{ 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 }, // +
{ 0x00, 0x50, 0x30, 0x00, 0x00, 0x00 }, // ,
{ 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, // -
{ 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 }, // .
{ 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 }, // slash
{ 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 }, // 0
{ 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 }, // 1
{ 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 }, // 2
{ 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00 }, // 3
{ 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 }, // 4
{ 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 }, // 5
{ 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00 }, // 6
{ 0x03, 0x71, 0x09, 0x05, 0x03, 0x00 }, // 7
{ 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 }, // 8
{ 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00 }, // 9
{ 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 }, // :
{ 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 }, // ;
{ 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 }, // <
{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 }, // =
{ 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 }, // >
{ 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 }, // ?
{ 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00 }, // @
{ 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00 }, // A
{ 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 }, // B
{ 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 }, // C
{ 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // D
{ 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 }, // E
{ 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 }, // F
{ 0x3E, 0x41, 0x41, 0x49, 0x7A, 0x00 }, // G
{ 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 }, // H
{ 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 }, // I
{ 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 }, // J
{ 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 }, // K
{ 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 }, // L
{ 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00 }, // M
{ 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 }, // N
{ 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // O
{ 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 }, // P
{ 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 }, // Q
{ 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 }, // R
{ 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 }, // S
{ 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00 }, // T
{ 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 }, // U
{ 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 }, // V
{ 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 }, // W
{ 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, // X
{ 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 }, // Y
{ 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 }, // Z
{ 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00 }, // [
{ 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 }, // backslash
{ 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00 }, // ]
{ 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 }, // ^
{ 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 }, // _
{ 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 }, // `
{ 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 }, // a
{ 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00 }, // b
{ 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 }, // c
{ 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00 }, // d
{ 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 }, // e
{ 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00 }, // f
{ 0x08, 0x54, 0x54, 0x54, 0x3C, 0x00 }, // g
{ 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 }, // h
{ 0x00, 0x48, 0x7D, 0x40, 0x00, 0x00 }, // i
{ 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00 }, // j
{ 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 }, // k
{ 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 }, // l
{ 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00 }, // m
{ 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 }, // n
{ 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 }, // o
{ 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00 }, // p
{ 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00 }, // q
{ 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 }, // r
{ 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 }, // s
{ 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00 }, // t
{ 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 }, // u
{ 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 }, // v
{ 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 }, // w
{ 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 }, // x
{ 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00 }, // y
{ 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 }, // z
{ 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 }, // {
{ 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00 }, // |
{ 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 }, // }
{ 0x10, 0x08, 0x08, 0x10, 0x08, 0x00 } // ~
};

// Variables
byte displayBuffer[84][6];                  // x = 84 cols, y = 6 banks (8 Bit each)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  byte res;
  pinMode(pinDC, OUTPUT);
  pinMode(pinRST, OUTPUT);
  pinMode(pinLcdLed, OUTPUT);
  pinMode(pinLed, OUTPUT);

  digitalWrite(pinRST, HIGH);               // reset
  digitalWrite(pinRST, LOW);                // reset
  delay(500);                               // for 500 ms
  digitalWrite(pinRST, HIGH);               // end reset
  analogWrite(pinLcdLed, 128);              // switch LED on LCD

  SPI.begin(pinCS0);
  digitalWrite(pinDC, LOW);                 // enter command mode
  SPI.setClockDivider(pinCS0, 84);          // 84 MHz / 84 = 1 MHz
  SPI.transfer(pinCS0, 0x21);               // extended instruction set: H = 1, V = 0
  SPI.transfer(pinCS0, 0x40);               // set TC 00
  SPI.transfer(pinCS0, 0x14);               // set Bias mode = 4: 1:40 / 1:34
  SPI.transfer(pinCS0, 0xB0);               // set VOP / Contrast
  SPI.transfer(pinCS0, 0x20);               // basic instruction set: H = 0, V = 0
  SPI.transfer(pinCS0, 0x0C);               // set Display Mode Normal
  // test();
} // end setup

void test() {
  // testing the display
  for (int i = 0; i < 32; i++) {
    SPI.transfer(pinCS0, 0xFF);
  }
  delay(1000);
}

void loop() {
  clearDisplayBuffer();
  printChar(39, 20, '@');
  updateDisplay();
}

int printChar(int x, int y, char value) {
  // Druckt das Zeichen vlaue an der Stelle x, y (linke obere Ecke)
  int f =  value - 0x20;
  byte b;
  if ((x + 6 >= 84) | (y + 8 >= 48)) {
    return -1;
  }
  for (int fcol = 0; fcol < 5; fcol++) {
    b = font[f][fcol];
    for (int frow = 0; frow < 8; frow++) {
      setPixel(x + fcol, y + frow, b & byteBits[frow]);
    }
  }
  return 0;
}

void setColumnBitmap(int x, byte bitmap) {
  // activate column "x" in all 6 banks with the same bitmap
  x = constrain(x, 0, 83);                  // limit x-values
  for (int bank = 0; bank < 6; bank++) {
    setBankBitmap(x, bank, bitmap);
  }
}

void setBankBitmap(int x, int bank, byte bitmap) {
  // activate column "x" in bank "bank" with bitmap "bitmap"
  x = constrain(x, 0, 83);                  // limit x-values
  bank = constrain(bank, 0, 5);             // limit bank-values
  displayBuffer[x][bank] = bitmap;
}


void clearDisplayBuffer() {
  // clears the displayBuffer completely
  for (int x = 0; x < 84; x++) {
    for (int bank = 0; bank < 6; bank++) {
      displayBuffer[x][bank] = 0;
    }
  }
}

void setPixel(int x, int y, char value) {
  // set a Pixel in the displayBuffer. Params: x=[0:83], y=[0:47], value=[0:1]
  int bank = y / 8 ;
  int bitPos = y % 8;
  x = constrain(x, 0, 83);                  // limit x-values
  bank = constrain(bank, 0, 5);             // limit bank-values
  if (value == 0) {                         // clear bit at bitPos
    displayBuffer[x][bank] =  displayBuffer[x][bank] & (~byteBits[bitPos]);
  } else {                                  // set bit at bitPos
    displayBuffer[x][bank] =  displayBuffer[x][bank] | byteBits[bitPos];
  }
}

void updateDisplay() {
  // push displayBuffer to display see PCD8544 manual how to
  byte b;                                   // b is a bitmap of a bank-column
  digitalWrite(pinDC, LOW);                 // enter command mode
  SPI.transfer(pinCS0, 0x20);               // basic instrucion set: H = 0, V = 0
                                            // V = 0 means horizontal adressing
  SPI.transfer(pinCS0, 0x80);               // set X-address to 0
  SPI.transfer(pinCS0, 0x40);               // set Y-address to 0
  digitalWrite(pinDC, HIGH);                // enter data mode
  for (int bank = 0; bank < 6; bank++) {
    for (int x = 0; x < 84; x++) {
      b = displayBuffer[x][bank];
      SPI.transfer(pinCS0, b);
    }
  }
}



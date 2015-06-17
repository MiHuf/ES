// Aufgabe 5.1, Stand von 2015-06-17
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
  // return;
  clearDisplayBuffer();
  updateDisplay();
  for (int x = 0; x < 84; x++) {
    setColumnBitmap(x, 0xFF);
    updateDisplay();
    delay(50);
  }
  for (int x = 0; x < 84; x++) {
    setColumnBitmap(x, 0x00);
    updateDisplay();
    delay(50);
  }
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

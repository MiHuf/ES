// Aufgabe 5.1, Stand von 2015-06-10
// Lösung von Michael Hufschmidt , 6436122 , michael@hufschmidt-web.de,
//            Tim Welge          , 6541929 , tw@ens-fiti.de
//            Rania Wittenberg   , xxxxxxx , rania_wittenberg@hotmail.com

#include <SPI.h>

// Pin Constants
const int pinCS0 = 10;                  // SPI: Chip Select 0  <<< used here
// const int pinCS1 = 4;                // SPI: Chip Select 1
// const int pinCS2 = 52;               // SPI: Chip Select 2
const int pinRST = 5;                   // SPI: RST = Reset
const int pinDC = 6;                    // SPI: D/C = Data / Control
const int pinLed = 13;                  // internal LED

// Variables
byte pixelBuffer[48][84];               // 48 rows x 84 cols
byte displayBuffer[6][84];              // 6 banks (8 Bit each) x 84 cols

// Ergänzungen von Michael, die mir heute noch eingefallen sind:
void setup() {
  // put your setup code here, to run once:
  byte res;
  pinMode(pinLed, OUTPUT);
  pinMode(pinRST, OUTPUT);
  pinMode(pinDC, OUTPUT);
  Serial.begin(9600);
  digitalWrite(pinRST, HIGH);
  digitalWrite(pinRST, LOW);
  delay(500);
  digitalWrite(pinRST, HIGH);
  SPI.begin(pinCS0);
  digitalWrite(pinDC, LOW);                    // enter command mode
  SPI.setClockDivider(pinCS0, 84);             // 1 MHz
  res = SPI.transfer(10, 0x21, SPI_CONTINUE);  // extended instrucion set
  res = SPI.transfer(10, 0x14, SPI_CONTINUE);  // set Bias
  res = SPI.transfer(10, 0xE0, SPI_CONTINUE);  // set Contrast
  res = SPI.transfer(10, 0x20, SPI_CONTINUE);  // set Display Mode Normal
  res = SPI.transfer(10, 0x0C);
  digitalWrite(pinDC, HIGH);                   // enter data mode

} // end setup

void loop() {
  // put your main code here, to run repeatedly:

}  // end loop

void setPixel(int x, int y, char value) {
  // set a Pixel in the pixelBuffer value must be 0 or 1
  pixelBuffer[x][y] = value;
}

void updateDisplay() {
  // calculate displayBuffer from PixelBuffer see PCD8544 manual how to
  // push displayBuffer to display see PCD8544 manual how to
  byte b;                               // b is a bitmap of a bank-column
  int line, bank, col;
  digitalWrite(pinDC, HIGH);            // enter data mode
  // The following requires much mode code !!!
  for (bank = 0; bank < 6; bank++) {
    b = 0;
    for (line = 0; line < 48; line++) {
      for (col = 0; col < 84; col++) {
        // here you need to calculate the bits within c
        b = pixelBuffer[line][col] ;  // it is not that easy!!!!
        SPI.transfer(pinCS0, b);
      }
    }
  }
}

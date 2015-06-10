// Aufgabe 5.3, Stand von 2015-06-10
// Lösung von Michael Hufschmidt , 6436122 , michael@hufschmidt-web.de,
//            Tim Welge          , 6541929 , tw@ens-fiti.de
//            Rania Wittenberg   , xxxxxxx , rania_wittenberg@hotmail.com

#include <SPI.h>

// Pin Constants
const int pinSS = 4;                    // SPI: Slave Select
const int pinRST = 5;                   // SPI: RST = Reset
const int pinDC = 6;                    // SPI: D/C = Data / Control
const int pinLed = 13;                  // internal LED


// Variables
byte displayBuffer[48][84];             // Zeilen x Spalten

void setup() {
  byte res;
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin(pinSS);
  SPI.setClockDivider(pinSS, 84);       // 1 MHz
  res = SPI.transfer(10, 0x21, SPI_CONTINUE);  // extended instrucion set
  res = SPI.transfer(10, 0x14, SPI_CONTINUE);  // set Bias
  res = SPI.transfer(10, 0xE0, SPI_CONTINUE);  // set Contrast
  res = SPI.transfer(10, 0x20, SPI_CONTINUE);  // set Display Mode Normal
  res = SPI.transfer(10, 0x0C);
} // end setup

void loop() {
  // put your main code here, to run repeatedly:

}  // end loop

void setPixel(int x, int y, char value) {
  // set a Pixel in the displayBuffer
  displayBuffer[x][y] = value;
}

void updateDisplay() {
  // push displayBuffer
  for (int line = 0; line < 48; line++) {
    for (int col = 0; col < 84; col++) {
      SPI.transfer(pinSS, displayBuffer[line][col]);
    }
  }
}

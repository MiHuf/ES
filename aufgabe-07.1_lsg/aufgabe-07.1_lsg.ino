// Aufgabe 7.1, Stand von 2015-07-03
// Lösung von Michael Hufschmidt , 6436122 , michael@hufschmidt-web.de,
//            Tim Welge          , 6541929 , tw@ens-fiti.de
//            Rania Wittenberg   , 6059892 , rania_wittenberg@hotmail.com

#include <SPI.h>
#include <SD.h>

// Pin Constants
// const int pinCS0 = 10;                   // SPI: SCE Chip Select 0  <<< for LCD-Display
const int pinCS1 = 4;                       // SPI: SCE Chip Select 1  <<< for Wireless-Shield
// Serial 3 pins are pinTX3 = 14 and pinRX3 = 15
const int pinLed = 13;                      // internal LED

const char *fileName = "RaMiTi.dat";        // Output Filename

// Variables
const int lineBufferLen = 80;               // size of line buffer
char lineBuffer[lineBufferLen];             // input line buffer
int linePos = 0;

void setup() {
  // put your setup code here, to run once:
  bool sdOK;
  Serial.begin(9600);
  Serial3.begin(9600);

  sdOK = SD.begin(pinCS1);                  // SD-Card
  if (sdOK) {
    Serial.println("Serial Card OK");
  } else {
    Serial.println("Serial Card NOT OK");
  }
  //
} // end setup

void loop() {
  // put your main code here, to run repeatedly:
}

void serialEvent() {
  // somebody wants to talk
}

void serialEvent3() {
  // GPS wants to talk
  byte c;
  /*
  if (Serial3.available()) {
    c = Serial3.read();
    zeile[pos] = c;
    pos ++;                             // ready for next char
  }
  if (c == '\n') {
    zeile[pos] = 0;                     // terminate String
    pos = 0;
    parseCommand();
    Serial.println(zeile);              // for testing
  }
  */
}

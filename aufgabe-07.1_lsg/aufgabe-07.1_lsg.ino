// Aufgabe 7.1, Stand von 2015-07-07
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

const char *fileName = "RaMiTi.dat";        // Output Filename von Rania, Michael, Tim

// Variables
const int lineBufferLen = 512;              // size of line buffer
char lineBuffer[lineBufferLen];             // input line buffer
void setup() {
  // put your setup code here, to run once:
  bool sdOK;
  Serial.begin(9600);                       //
  Serial3.begin(9600);                      // GPS Modul

  sdOK = SD.begin(pinCS1);                  // SD-Card
  if (sdOK) {
    Serial.println("microSD Card OK");
  } else {
    Serial.println("Error: microsSD Card NOT OK");
  }
  //
} // end setup

void loop() {
  // put your main code here, to run repeatedly:
}

void parseGPS() {
  // Analyse $GPGGA
  String line = lineBuffer;                 // lineBuffer converted to String
  int commapos[14];                         // Position of the commas
  int starpos;                              // Position of the *
  double latitude, longitude;               // Geographic position
  uint8_t fix = 0;                          // fix = sattelite data valid
  uint8_t sats = 0;                         // number of satellites
  char c1, c2;                              // Checksum chars
  // byte checksum;
  if (line.substring(0, 6).equals("$GPGGA")) {
    commapos[0] = line.indexOf(",");
    for (int i = 1; i < 14; i++) {
      commapos[i] = line.indexOf(",", commapos[i - 1] + 1);
    }
    starpos = line.indexOf("*");
    Serial.println(lineBuffer);             // just for testing
    latitude = line.substring(commapos[1] + 1, commapos[2]).toFloat();
    longitude = line.substring(commapos[3] + 1, commapos[4]).toFloat();
    fix = line.substring(commapos[5] + 1, commapos[6]).toInt();
    sats = line.substring(commapos[6] + 1, commapos[7]).toInt();
    c1 = line.charAt(starpos + 1);
    c2 = line.charAt(starpos + 2);
    if (check(c1, c2) && (fix > 0)) {
      Serial.print("No fo Sats = ");
      Serial.print(sats);
      Serial.print(", Laenge = ");
      Serial.print(latitude, 6);
      Serial.print(", Breite =");
      Serial.println(longitude, 6);
    }
  }
}

bool check(char c1, char c2) {
  // validates the checksum info, input are HEX values
  return true;
}

void serialEvent() {
  // somebody wants to talk
}

void serialEvent3() {
  // GPS wants to talk
  int pos = 0;                              // position within line buffer
  byte c;
  if (Serial3.available()) {
    c = Serial3.read();
    lineBuffer[pos] = c;
    pos ++;                                 // ready for next char
  }
  if (c == '\n') {
    lineBuffer[pos] = 0;                    // terminate String
    pos = 0;
    parseGPS();                             // evaluate lineBuffer
  }
}

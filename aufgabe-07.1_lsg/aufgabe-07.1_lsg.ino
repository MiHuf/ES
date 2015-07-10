// Aufgabe 7.1, Stand von 2015-07-10
// Lösung von Michael Hufschmidt , 6436122 , michael@hufschmidt-web.de,
//            Tim Welge          , 6541929 , tw@ens-fiti.de
//            Rania Wittenberg   , 6059892 , rania_wittenberg@hotmail.com

#include <SPI.h>
#include <SD.h>

// Pin Constants
const int pinCS1 = 4;                       // SPI: SCE Chip Select 1  <<< for Wireless-Shield
// Serial 3 pins are pinTX3 = 14 and pinRX3 = 15
const int pinLed = 13;                      // internal LED
const char *fileName = "RaMiTi.dat";        // Output Filename von Rania, Michael, Tim
int pos = 0;                                // position within line buffer

// Variables
const int lineBufferLen = 160;              // size of line buffer (should be <= 80)
char lineBuffer[lineBufferLen]  = {0};      // input line buffer
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
  double latitude, longitude;               // Geographic position
  uint8_t fix = 0;                          // fix = sattelite data valid
  uint8_t sats = 0;                         // number of satellites
  if (line.substring(0, 6).equals("$GPGGA")) {
    commapos[0] = line.indexOf(",");
    for (int i = 1; i < 14; i++) {
      commapos[i] = line.indexOf(",", commapos[i - 1] + 1);
    }
    // Serial.println(lineBuffer);             // just for testing
    latitude = line.substring(commapos[1] + 1, commapos[2]).toFloat();
    longitude = line.substring(commapos[3] + 1, commapos[4]).toFloat();
    fix = line.substring(commapos[5] + 1, commapos[6]).toInt();
    sats = line.substring(commapos[6] + 1, commapos[7]).toInt();
    if (check() && (fix > 0)) {
      Serial.print("No fo Sats = ");
      Serial.print(sats);
      Serial.print(", Laenge = ");
      Serial.print(latitude, 6);
      Serial.print(", Breite =");
      Serial.println(longitude, 6);
    }
  }
}

bool check() {
  // validates the checksum info within the lineBuffer
  uint8_t coll = 0;                         // collecting all chars for checksum
  uint8_t checksum = 0;                     // as transmitted
  int pos = 1;                              // behind the "$"
  char c1, c2;                              // checksum chars
  while (lineBuffer[pos] != '*') {
    coll = coll xor lineBuffer[pos];
    pos++;
  }                                         // now pos points to "*"  
  c1 = lineBuffer[pos + 1];                 // high order hex value 
  c2 = lineBuffer[pos + 2];                 // low order hex value 
  checksum = (c1 <= '9' ? c1 - '0' : c1 + 10 - 'A') * 16 + (c2 <= '9' ? c2 - '0' : c2 + 10 - 'A');
  return checksum == coll;
}

void serialEvent() {
  // somebody wants to talk
}

void serialEvent3() {
  // GPS wants to talk
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

// Aufgabe 7.3, Stand von 2015-07-10
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
File ofi;
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
  ofi = SD.open(fileName, FILE_WRITE);
  if (ofi) {
    Serial.println("Output-File open");
  } else {
    Serial.println("Error opening Output-File");
  }
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
    // Serial.println(lineBuffer);          // just for testing
    latitude = dataToDegrees(line.substring(commapos[1] + 1, commapos[2]));
    longitude = dataToDegrees(line.substring(commapos[3] + 1, commapos[4]));
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

void readData() {
  byte b;
  File ifi = SD.open(fileName, FILE_READ);
  if (ifi) {
    Serial.println("Input-File open");
  } else {
    Serial.println("Error opening Input-File");
  }
  while (ifi.available()) {
    b = ifi.read();
    Serial.print(b);
  }
}

void writeData(double laenge, double breite) {
  char buff[80];
  String ls(laenge, 6);
  String bs(breite, 6);
  String out = ls + "," + bs + "\n";
  out.toCharArray(buff, out.length());
  ofi.write(buff);
  ofi.flush();
}

double dataToDegrees(String s) {
  int pointpos = 0;
  double dd, mm;
  pointpos = s.indexOf(".");
  mm = s.substring(pointpos - 2, s.length() - 1).toFloat();
  dd = s.substring(0, pointpos - 2).toFloat() + mm / 60.0;
  return dd;
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
  byte c;
  if (Serial.available()) {
    c = Serial.read();
  }
  if (c == 'x') {
    readData();
  }
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

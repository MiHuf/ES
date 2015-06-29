// Aufgabe 7.3, Stand von 2015-06-29
// Lösung von Michael Hufschmidt , 6436122 , michael@hufschmidt-web.de,
//            Tim Welge          , 6541929 , tw@ens-fiti.de
//            Rania Wittenberg   , 6059892 , rania_wittenberg@hotmail.com

#include <SPI.h>
#include <SD.h>

// Pin Constants
const int pinCS0 = 10;                      // SPI: SCE Chip Select 0  <<< for LCD-Display
const int pinCS1 = 4;                       // SPI: SCE Chip Select 1  <<< for Wireless-Shield
// const int pinCS2 = 52;                   // SPI: SCE Chip Select 2
const int pinDC = 5;                        // SPI: D/C = Data / Control
const int pinRST = 6;                       // RST = Reset LCD-Display
const int pinLcdLed = 3;                    // LED on LCD
const int pinLed = 13;                      // internal LED
const int pinMosi = 109;                    // SPI-Block: MOSI
const int pinMiso = 108;                    // SPI-Block: MISO
const int pinClock = 110;                   // SPI-Block: Clock

static const byte byteBits[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

unsigned char font[95][6];                  // Not needed for 7.x
const String s0 = "Hallo Welt";

// Variables
byte displayBuffer[84][6];                  // x = 84 cols, y = 6 banks (8 Bit each)
const int commandLen = 80;                  // size of line buffer
char lineBuffer[commandLen];                // input line buffer = command buffer
int linePos = 0;
bool commandReady = false;
const int dataSize = 1024;                  // size of data buffer
char dataBuffer[dataSize];                  // data buffer (.txt or .img)
unsigned long dataLen;                      // aktual file size
int dataPos = 0;

void setup() {
  // put your setup code here, to run once:
  bool sdOK = false;

  Serial.begin(9600);
  pinMode(pinDC, OUTPUT);
  pinMode(pinRST, OUTPUT);
  pinMode(pinLcdLed, OUTPUT);
  pinMode(pinLed, OUTPUT);

  digitalWrite(pinRST, HIGH);               // reset
  digitalWrite(pinRST, LOW);                // reset
  delay(500);                               // for 500 ms
  digitalWrite(pinRST, HIGH);               // end reset
  analogWrite(pinLcdLed, 128);               // switch LED on LCD

  SPI.begin(pinCS0);
  digitalWrite(pinDC, LOW);                 // enter command mode
  SPI.setClockDivider(pinCS0, 84);          // 84 MHz / 84 = 1 MHz
  SPI.transfer(pinCS0, 0x21);               // extended instruction set: H = 1, V = 0
  SPI.transfer(pinCS0, 0x40);               // set TC 00
  SPI.transfer(pinCS0, 0x14);               // set Bias mode = 4: 1:40 / 1:34
  SPI.transfer(pinCS0, 0xE0);               // set VOP / Contrast : 0x80 ... 0xFF
  SPI.transfer(pinCS0, 0x20);               // basic instruction set: H = 0, V = 0
  SPI.transfer(pinCS0, 0x0C);               // set Display Mode Normal

  sdOK = SD.begin(pinCS1);                  // SD-Card
  if (sdOK) {
    Serial.println("Serial Card OK");
  } else {
    Serial.println("Serial Card NOT OK");
  }

  clearDisplayBuffer();                     //
  s0.toCharArray(dataBuffer, s0.length() + 1);
  printCharsLine(3, 0, s0.length() - 1, dataBuffer);
  updateDisplay();                          //
  clearDisplayBuffer();

  File file = SD.open("/");

  printDirectory(file, 8);
  //
} // end setup

void loop() {
  // put your main code here, to run repeatedly:
  if (commandReady) {
    processCommand();
    updateDisplay();
    clearDisplayBuffer();                     //
  }
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("tt");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}


void processCommand() {
  // Filename in lineBuffer ready for processing
  Serial.print("Input Command = ");         // for testing
  Serial.println(lineBuffer);               // for testing
  printCharsLine (2, 0, 13, lineBuffer);    // for testing: Print buffer on LCD
  readFile(lineBuffer);
  commandReady = false;                     // no more commands pending
  linePos = 0;                              // ready for next command;
}

bool readFile(char* fileName) {
  bool ok;
  File iFile;
  int pos = 0;
  /*
    File iFile = SD.open("/");

    File entry =  iFile.openNextFile();

    if (entry) {
      while (iFile.available()) {
        dataBuffer[pos] = iFile.read();
        pos ++;
         Serial.println(pos);
      }
      dataLen = pos - 1;
    }
    Serial.println(dataBuffer);

  */
  ok = SD.exists(fileName);
  if (ok) {
    Serial.println("File exists");
    iFile = SD.open(fileName);
    pos = 0;
    if (iFile) {
      dataLen = iFile.size();
      while (iFile.available()) {
        dataBuffer[pos] = iFile.read();
        pos ++;
      }
      dataLen = pos - 1;
    } else {
      Serial.println("File cannot be opened");
    }
  }
  else {
    Serial.println("SD No exists File");
  }
  return ok;
}

void serialEvent() {
  // somebody wants to talk
  byte c;
  while (Serial.available()) {
    c = (char) Serial.read();
    lineBuffer[linePos] = c;
    linePos ++;                             // ready for next char
  }
  if (c == '\n') {
    commandReady = true;
    lineBuffer[linePos] = 0;                // terminate String
  }
}

void printCharsLineWrap(char * s) {
  // Druckt alle Zeichen von s ab Zeile 0 und beginnt ggf. eine neue Zeile
  int line = 0;                             // Zeilennummer
  int pos1 = 0;                             // erstes Zeichen der Zeile
  int pos2 = 0;                             // letztes Zeichen der Zeile
  // TODO: Hier fehlt noch was
}

void printCharsLine(int line, int pos1, int pos2, char * s) {
  // Druckt die Zeichen von s[pos1] bis s[pos2] in eine Zeile ab Spalte 0
  int col0 = 0;
  for (int pos = 0; (pos1 + pos <= pos2) && (s[pos1 + pos] != 0); pos++) {
    printCharColLine(col0 + pos, line, s[pos]);
  }
}

int printCharColLine(int col, int line, char value) {
  // Druckt das Zeichen Value an der Spalte col [0 ... 13] in Zeile line [0 ... 5]
  int f =  value - 0x20;
  if ((col >= 14) | (line >= 6) | f < 0) {
    return -1;
  }
  for (int fcol = 0; fcol < 5; fcol++) {
    displayBuffer[6 * col + fcol][line] = font[f][fcol];
  }
}

void printStringCentered(int y, String s) {
  // Druckt den String S zentriert in einer lineBuffer
  int x0;
  int len = s.length();
  if (len > 14) {
    return;
  }
  x0 = (84 - 6 * len) / 2;
  for (int pos = 0; pos < len; pos++) {
    printChar(x0 + 6 * pos, y, s.charAt(pos));
  }
}

int printChar(int x, int y, char value) {
  // Druckt das Zeichen value an der Stelle x, y (linke obere Ecke)
  int f =  value - 0x20;
  byte b;
  if ((x + 6 >= 84) | (y + 8 >= 48) | f < 0) {
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


// Aufgabe 6.1, Stand von 2015-07-01
// Lösung von Michael Hufschmidt , 6436122 , michael@hufschmidt-web.de,
//            Tim Welge          , 6541929 , tw@ens-fiti.de
//            Rania Wittenberg   , 6059892 , rania_wittenberg@hotmail.com

#include <SPI.h>
#include <SD.h>

#define LCD_RESX 84
#define LCD_RESY 48

// Pin Constants
const int pinCS0 = 10;                      // SPI: SCE Chip Select 0  <<< for LCD-Display
const int pinCS1 = 4;                       // SPI: SCE Chip Select 1  <<< for Wireless-Shield
// const int pinCS2 = 52;                   // SPI: SCE Chip Select 2
const int pinDC = 3;                        // SPI: D/C = Data / Control
const int pinRST = 2;                       // RST = Reset LCD-Display
const int pinLcdLed = 7;                    // LED on LCD
const int pinLed = 13;                      // internal LED
const int pinMosi = 109;                    // SPI-Block: MOSI
const int pinMiso = 108;                    // SPI-Block: MISO
const int pinClock = 110;                   // SPI-Block: Clock

static const byte byteBits[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

unsigned char font[95][6] = {               // charset 6x8_ascii_data.txt
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // space
  { 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00 },   // !
  { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 },   // "
  { 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00 },   // #
  { 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00 },   // $
  { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 },   // %
  { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 },   // &
  { 0x00, 0x00, 0x07, 0x00, 0x00, 0x00 },   // '
  { 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00 },   // (
  { 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00 },   // )
  { 0x0A, 0x04, 0x1F, 0x04, 0x0A, 0x00 },   // *
  { 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 },   // +
  { 0x00, 0x50, 0x30, 0x00, 0x00, 0x00 },   // ,
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 },   // -
  { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 },   // .
  { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 },   // slash
  { 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 },   // 0
  { 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 },   // 1
  { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 },   // 2
  { 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00 },   // 3
  { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 },   // 4
  { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 },   // 5
  { 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00 },   // 6
  { 0x03, 0x71, 0x09, 0x05, 0x03, 0x00 },   // 7
  { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 },   // 8
  { 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00 },   // 9
  { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 },   // :
  { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 },   // ;
  { 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 },   // <
  { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 },   // =
  { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 },   // >
  { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 },   // ?
  { 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00 },   // @
  { 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00 },   // A
  { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 },   // B
  { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 },   // C
  { 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 },   // D
  { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 },   // E
  { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 },   // F
  { 0x3E, 0x41, 0x41, 0x49, 0x7A, 0x00 },   // G
  { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 },   // H
  { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 },   // I
  { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 },   // J
  { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 },   // K
  { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 },   // L
  { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00 },   // M
  { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 },   // N
  { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 },   // O
  { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 },   // P
  { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 },   // Q
  { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 },   // R
  { 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 },   // S
  { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00 },   // T
  { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 },   // U
  { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 },   // V
  { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 },   // W
  { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 },   // X
  { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 },   // Y
  { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 },   // Z
  { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00 },   // [
  { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 },   // backslash
  { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00 },   // ]
  { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 },   // ^
  { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 },   // _
  { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 },   // `
  { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 },   // a
  { 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00 },   // b
  { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 },   // c
  { 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00 },   // d
  { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 },   // e
  { 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00 },   // f
  { 0x08, 0x54, 0x54, 0x54, 0x3C, 0x00 },   // g
  { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 },   // h
  { 0x00, 0x48, 0x7D, 0x40, 0x00, 0x00 },   // i
  { 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00 },   // j
  { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 },   // k
  { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 },   // l
  { 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00 },   // m
  { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 },   // n
  { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 },   // o
  { 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00 },   // p
  { 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00 },   // q
  { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 },   // r
  { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 },   // s
  { 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00 },   // t
  { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 },   // u
  { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 },   // v
  { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 },   // w
  { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 },   // x
  { 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00 },   // y
  { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 },   // z
  { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 },   // {
  { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00 },   // |
  { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 },   // }
  { 0x10, 0x08, 0x08, 0x10, 0x08, 0x00 }    // ~
};
const String s0 = "Hallo Welt";

// Variables
byte displayBuffer[84][6];                  // x = 84 cols, y = 6 banks (8 Bit each)
const int commandLen = 80;                  // size of line buffer
char commandBuffer[commandLen];             // input line buffer = command buffer
int linePos = 0;
bool commandReady = false;
const int dataSize = 1024;                  // size of data buffer
char dataBuffer[dataSize];                  // data buffer (.txt or .img)
unsigned long dataLen;                      // aktual file size
int dataPos = 0;

uint8_t width, height;

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

  clearDisplayBuffer();                     //
  s0.toCharArray(dataBuffer, s0.length() + 1);
  printCharsLine(3, 0, s0.length() - 1, dataBuffer);
  updateDisplay();                          //
  delay (5000);
  clearDisplayBuffer();

  sdOK = SD.begin(pinCS1);                  // SD-Card
  if (sdOK) {
    Serial.println("SD: Serial Card OK");
  } else {
    Serial.println("SD: Serial Card NOT OK");
  }
  // File file = SD.open("/");
  // printDirectory(file, 8);
  //
} // end setup

void loop() {
    clearDisplayBuffer();                     //
    updateDisplay();
  return;
  // put your main code here, to run repeatedly:
  if (commandReady) {
    processCommand();
    updateDisplay();
  }
}

void printDirectory(File dir, int numTabs) {
  // aus dem Internet kopiert
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
  // fileName in commandBuffer ready for processing
  Serial.print("Input Command = ");         // for testing
  Serial.println(commandBuffer);            // for testing
  // readFile (commandBuffer);
  clearDisplayBuffer();
  processFile(commandBuffer);
  printCharsLine (2, 0, 13, commandBuffer); // for testing: Print buffer on LCD
  updateDisplay();
  commandReady = false;                     // no more commands pending
  linePos = 0;                              // ready for next command;
}

void processFile(char* fileName) {
  int was;
  int pos = 0;
  File iFile;
  was = fileName[0] - '0';                  // 1 ... 6
  switch (was) {
    case 1:
      // finam = "text1.txt";

//      iFile = SD.open("text1.txt");
//      dataLen = iFile.size();
//      while (iFile.available()) {
//        dataBuffer[pos] = iFile.read();
//        //       Serial.print(b);                         // for testing
//        pos ++;
//      }

  clearDisplayBuffer();                     //
  s0.toCharArray(dataBuffer, s0.length() + 1);
  printCharsLine(3, 0, s0.length() - 1, dataBuffer);
  updateDisplay();                          //

      Serial.println(dataBuffer);
      printStringCentered(3, dataBuffer) ;
      break;
    case 2:
      //   finam = "text2.txt";
      break;
    case 3:
      //   finam = "tams.img";
      displayImage("tams.img", LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2);
      break;
    case 4:
      //    finam = "smile1.img";
      //       displayImage(finam, LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2);
      break;
    case 5:
      //     finam = "smile2.img";
      //       displayImage(finam, LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2);
      break;
    case 6:
      //    finam = "smile3.img";
      //       displayImage(finam, LCD_RESX / 2 - width / 2, LCD_RESY / 2 - height / 2);
      break;
  }
}

bool getImageDims(char *file, uint8_t &width, uint8_t &height)
// von Eugen Richter
{
  bool result = false;
  if (SD.exists(file))
  {

    File img = SD.open(file, FILE_READ);
    if (img)
    { Serial.println("File  found");

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
      if (sscanf(dims, "%d,%d", &width, &height) == 2) {
        result = true;
      }
      img.close();
    }
  }
  else {

    Serial.println("File not found");
  }
  return result;
}

bool displayImage(char *file, uint8_t pX, uint8_t pY) {
  // von Eugen Richter
  uint8_t width, height;
  clearDisplayBuffer();
  if (!getImageDims(file, width, height)) {
    return false;
  }
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
              setPixel((index % width) + pX, (index / width) + pY, 0);
              index++;
              break;
            }
          case '1':
            {
              setPixel((index % width) + pX, (index / width) + pY, 1);
              index++;
              break;
            }
          default: {
              break;
            }
        }
      }
      result = true;
      img.close();
    }
  }
  return result;
}

bool readFile(char* fileName) {
  int was;
  char* finam;                              // = fileName, geht aber sonst nicht
  bool ok;
  byte b;                                   // byte read
  File iFile;
  int pos = 0;
  was = fileName[0] - '0';                  // 1 ... 6
  switch (was) {
    case 1:
      finam = "text1.txt";

      break;
    case 2:
      finam = "text2.txt";
      break;
    case 3:
      finam = "tams.img";
      break;
    case 4:
      finam = "smile1.img";
      break;
    case 5:
      finam = "smile2.img";
      break;
    case 6:
      finam = "smile3.img";
      break;
  }
  ok = SD.exists(finam);
  if (ok) {
    Serial.println("SD: File exists");
    iFile = SD.open(finam);
    pos = 0;
    if (iFile) {
      dataLen = iFile.size();
      while (iFile.available()) {
        b = iFile.read();
        dataBuffer[pos] = b;
        //       Serial.print(b);                         // for testing
        pos ++;
      }
      dataLen = pos - 1;
    } else {
      Serial.println("SD: File cannot be opened");
    }
  } else {
    Serial.println("SD: File does not exist");
  }
  dataBuffer[pos] = '\n';
  dataBuffer[pos + 1] = 0;
  Serial.print(dataBuffer);
  return ok;
}

void serialEvent() {
  // somebody wants to talk
  byte c;
  while (Serial.available()) {
    c = (char) Serial.read();
    commandBuffer[linePos] = c;
    linePos ++;                             // ready for next char
  }
  if (c == '\n') {
    commandBuffer[linePos] = 0;             // terminate String
    commandReady = true;
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
  // Druckt den String S zentriert in einer commandBuffer
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


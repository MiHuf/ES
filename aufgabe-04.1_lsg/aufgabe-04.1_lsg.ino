// Aufgabe 4.1, Stand von 2015-05-19
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include "KeyHandler.h"

// Pin Constants
const int pinLed = 13;                  // internal LED
const int pinKey = 7;                   // external pushbutton
// Serial 3 pins are pinTX3 = 14 and pinRX3 = 15

// Timer Params
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABpinLed
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000

// Variables
const int len = 160;                    // length of line buffer
char zeile[len];                        // line buffer
int pos = 0;                            // position within line buffer
uint32_t timerValue = 0;                // Timer value in milliseconds
int blinkTimer = 0;                     // zu kompliziert zu erklaeren
int bright = 0;                         // LED brightness
bool increase = true;
KeyHandler pb1;                         // Pushbutton object

void setup() {
  // put your setup code here, to run once:
  // Timer setup and start
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6);        // Timer 2, channel 0
  TC_Configure(TC2, dwChannel, dwMode);
  TC2->TC_CHANNEL[dwChannel].TC_IER = 0b1 << 4;    // Datenblatt Seite 917
  TC2->TC_CHANNEL[dwChannel].TC_IDR = ~(0b1 << 4); // Datenblatt Seite 918
  NVIC_ClearPendingIRQ(TC6_IRQn);       // Timer 2, channel 0
  NVIC_EnableIRQ(TC6_IRQn);             // Timer 2, channel 0
  TC_SetRC(TC2, dwChannel, 41999);      // (84 MHz / 2 - 1 kHz) set to 1 kHz
  TC_Start(TC2, dwChannel);

  // Other setup
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);            // LED Off
  pb1.keyInit(pinKey);
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}  // end loop

void setBlink() {
  if (blinkTimer <= 0) {                // if in non-blink mode
    blinkTimer = 14;                    // set to blink-mode
  }
}

void checkBlink() {
  // Serial.println(blinkTimer);           // for testing
  if (blinkTimer > 0) {
    switch (blinkTimer) {
      case 14 : case 9 : case 4:
        digitalWrite(pinLed, HIGH);     // LED ON
        break;
      case 11: case 6: case 1:
        digitalWrite(pinLed, LOW);      // LED OFF
        break;
    }
    blinkTimer = blinkTimer - 1;
  }
}

void checkBright() {
  // send new brightness to slave
  if ((bright != 0)) {
    sendBright();
    if (increase) {
      bright ++;
    } else {
      bright --;
    }
    if (bright >= 255) {
      increase = false;
    }
  }
}

void sendBright() {
  // send blink command to master
  // analogWrite(pinLed, bright);          // for testing on Master
  Serial3.print("h ");
  Serial3.println(bright);
}

void doPb1() {
  // initiiert den Zyklus
  if (pb1.validLow()) {
    increase = true;
    bright = 1;
  }
}

void parseCommand() {
  int _bright;                           // local bright
  String sbright;
  switch (zeile[0]) {
    case 'b':                            // Blink-Command
      setBlink();                        // local blink mode
      break;
    case 'h':                            // Brightness-Command
      sbright = String(zeile).substring(2);
      _bright = sbright.toInt();
      analogWrite(pinLed, _bright);
      if ((_bright <= 1) || (_bright >= 255)) {
        Serial3.println("b");            // send Blink-Command
      }
      break;
  }
}

void serialEvent3() {
  // somebody wants to talk
  byte c;
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
}

void TC6_Handler() {
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if (pb1.checkKey()) doPb1();
  if ((timerValue % 20) == 0) {   // every 20 ms
    checkBright();
  }
  if ((timerValue % 100) == 0) {  // every 100 ms
    checkBlink();
  }
}

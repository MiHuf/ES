// Aufgabe 2.1, Stand von 2015-05-05
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com
// Thanks to Norman Hendrich for debugging
#include "KeyHandler.h"

// Timer Params and Variables
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABLED
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000
uint32_t timerValue = 0;                                 // Millisekunden

// Other Params and Variables
const int swliPin = 4;          // one key   
const int led = 13;             // interne LED
KeyHandler swli;                // only one key 
bool ledOn = true;

void setup() {
  // Timer setup and start
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6);                   // Timer 2, channel 0
  TC_Configure(TC2, dwChannel, dwMode);
  TC2->TC_CHANNEL[dwChannel].TC_IER = 0b1 << 4;    // siehe Datenblatt Seite 917
  TC2->TC_CHANNEL[dwChannel].TC_IDR = ~(0b1 << 4); // siehe Datenblatt Seite 918
  NVIC_ClearPendingIRQ(TC6_IRQn);                  // Timer 2, channel 0
  NVIC_EnableIRQ(TC6_IRQn);                        // Timer 2, channel 0
  TC_SetRC(TC2, dwChannel, 41999);                 // (84 MHz / 2 - 1 Hz) set to 1 kHz
  TC_Start(TC2, dwChannel);

  // Other setup
  pinMode(led, OUTPUT);
  swli.keyInit(swliPin);
  digitalWrite(led, ledOn);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((timerValue % 1000) == 0) {  // alle 1000 ms
    Serial.println(timerValue);
  }
}

void switchLed() {
  ledOn = not(ledOn);
  digitalWrite(led, ledOn);
}

void doSwli() {
  // tu was
  if (swli.validLow()) switchLed();
}

void TC6_Handler() {
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if ((timerValue % 200) == 0) {  // alle 200 ms
    // switchLed();
  }
  if (swli.checkKey()) doSwli();
}


// Aufgabe 2.1, Stand von 2015-04-23
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

// Timer Params and Variables
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABLED
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000
uint32_t timerValue = 0;                                 // Millisekunden

// Other Params and Variables
const int led = 13;             // interne LED
const uint32_t bounceTime = 32; // bounce time in milli-seconds
boolean ledOn = true;

typedef struct {
  int pin;
  boolean validStatus;
  boolean actualStatus;
  uint32_t bouncing;
} Key;
typedef Key * pKey;
Key swli, swre;

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

  swli.pin = 4;
  pinMode(swli.pin, INPUT);
  swli.validStatus = digitalRead(swli.pin);
  swli.actualStatus = swli.validStatus;
  swli.bouncing = bounceTime;

  digitalWrite(led, ledOn);
  Serial.begin(9600);
}

// Warum kompiliert das nicht?
//void keyInit(pKey k ; int p) {
//  k->pin = p;
//  k->validStatus = digitalRead(k->pin);
//  k->actualStatus = k->validStatus;
//  k->.bouncing = bounceTime;
//}

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

// Warum kompiliert das nicht?
//boolean checkKey (pKey k) {
//  // Code wie unten hier einfügen
//  return false;
//}

boolean checkSwli () {
  swli.actualStatus = digitalRead(swli.pin);
  if (swli.actualStatus == swli.validStatus) {      // nothing happened, do nothing
    return false;
  } else {                                          // key has been pressed
    if (swli.bouncing == 0) {                       // bounce time exceeded?
      swli.bouncing = bounceTime;                   // reset timer
      swli.validStatus = swli.actualStatus;         // accept as valid
      return true;
    } else {
      swli.bouncing --;                             // decrement and check again later
      return false;
    };
  };
}
void doSwli() {
  // tu was
  if (swli.validStatus == LOW) switchLed();
}

void TC6_Handler() {
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if ((timerValue % 200) == 0) {  // alle 200 ms
    // switchLed();
  }
  if (checkSwli()) doSwli();
}

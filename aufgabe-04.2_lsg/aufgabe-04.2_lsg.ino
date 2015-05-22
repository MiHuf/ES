// Aufgabe 4.2, Stand von 2015-05-22
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Wire.h>

// Pin Constants
const int pinLed = 13;                  // internal LED
// I2C pins are SDA = 20, SCL = 21

// Other Constants
const bool isMaster = true;            // to be adapted before upload
const int adrSlave = 17;                // my birthdate

// Timer Params
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABpinLed
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000

// Variables
const int len = 160;                    // length of line buffer
byte inBuffer[len];                     // data buffer
int pos = 0;                            // position within line buffer
uint32_t timerValue = 0;                // Timer value in milliseconds
byte slaveLed = 0;                      // LED on slave
byte localLed = 0;                      // LED on local

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
  if (isMaster) {
    Wire.begin();
  } else {
    Wire.begin(adrSlave);
    Wire.onReceive(slaveHandler);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}  // end loop

void switchLedOnSlave() {
  byte result, c;
  Wire.beginTransmission(adrSlave);     // to slave
  Wire.write(slaveLed);                 // send value byte
  result = Wire.endTransmission();      // stop transmitting
  Serial.print("Master: ");
  Serial.println(slaveLed);
  slaveLed = 1 - slaveLed;
  Wire.requestFrom(adrSlave, 1);        // 1 byte from slave
  while (Wire.available()) {
    c = Wire.read();
  }
  Serial.print("From slave: ");
  Serial.println(c);  
}

void switchLedLocal() {
  digitalWrite(pinLed, localLed);
  Serial.print("Loca1: ");
  Serial.println(localLed);
  localLed = 1 - localLed;
}

void slaveHandler(int howMany) {
  byte c;
  for (int i = 0; i < howMany; i++)     {
    c = Wire.read();
    inBuffer[i] = c;
  }  // end of for loop
  digitalWrite(pinLed, c);
  Serial.print("Slave: ");
  Serial.println(c);
  Wire.beginTransmission (0);
  Wire.write(c);
  Wire.endTransmission();
}
void TC6_Handler() {
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if ((timerValue % 1000) == 0) {  // every 1000 ms
    // switchLedLocal();
    if (isMaster) {
      switchLedOnSlave();
    }
  }
}


// Aufgabe 4.3, Stand von 2015-06-05
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Wire.h>
#include <Servo.h>

// Pin Constants
const int pinLed = 13;                  // internal LED
// I2C pins are SDA = 20, SCL = 21
// for Gyro on Slave Board
const int pinX1 = A2;                   // Analog Gyro XOUT
const int pinZ1 = A3;                   // Analog Gyro ZOUT
const int pinX2 = A4;                   // Analog Gyro X4.5OUT
const int pinZ2 = A5;                   // Analog Gyro Z4.5OUT
const int pinRef = A6;                  // Analog Gyro VREF
const int pinAZ = 7;                    // Digital I/O Gyro
// factor = (5000.0 Volt / 1024.0 steps) / senitivity
// Winkelgeschwindigkeit in Grad / Sekunde = factor * (rate - vref)
const double factor2 = (5000.0 / 1024.0) / 2.0;
const double factor45 = (5000.0 / 1024.0) / 9.1;
// for Servo on Master Board
const int servoPin = 3;                 // Servo-Pin
const int wmin = 24;                    // Servo-Grenzwert, ermittelt
const int wmax = 159;                   // Servo-Grenzwert, ermittelt
const double schwelle = 1.0;            // Schwellwert fuer Gyro-Ausgabe
const double gain = 0.1;                // Verstaerkung fuer Gyro
int welchesGyro = pinZ1;                // evtl. anpassen

// Other Constants
const bool isMaster = true;             // to be adapted before upload
const int adrSlave = 17;                // my birthdate

// Timer Params
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABpinLed
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000

// Variables
const int len = sizeof(int);            // length of line buffer
byte inBuffer[len];                     // data bufferconst int len = 160;
int pos = 0;                            // position within line buffer
uint32_t timerValue = 0;                // Timer value in milliseconds
Servo neuServo;                         // Servo-Objekt erstellen
int winkel = (wmax - wmin) / 2;         // Mittelstellung am Anfang

void setup() {
  // put your setup code here, to run once:

  // Setup for Gyro
  pinMode(pinX1, INPUT);
  pinMode(pinZ1, INPUT);
  pinMode(pinX2, INPUT);
  pinMode(pinZ2, INPUT);
  pinMode(pinRef, INPUT);
  pinMode(pinAZ, OUTPUT);
  // 1 Sekunde Reset Impuls an Gyro
  digitalWrite(pinAZ, LOW);
  delay(1000);
  digitalWrite(pinAZ, HIGH);
  // Setup for Servo
  neuServo.attach(servoPin);            // Servo an Pin 3
  neuServo.write(winkel);               // Mittelstellung

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

void updateServo() {
  // Gyro auslesen, neuen Winkel berechnen und Servo einstellen
  double omega;
  omega = readGyro();                   // Winkelgeschwindigkeit Grad pro Sekunde
  winkel = winkel + int (gain * omega + 0.5); // Geschwindigkeit angepassen
  //   winkel = (wmax - wmin) / 2 ;
  if (winkel < wmin) {
    // setBlink();
    winkel = wmin;
  }
  if (winkel > wmax) {
    // setBlink();
    winkel = wmax;
  }
  moveServo(winkel);
}
void moveServo(int w) {
  // Servo auf den Winkel w einstellen
  Serial.print("Winkel = ");
  Serial.println(w);                    // Ausgabe zur Kontrolle
  neuServo.write(w);                    // Servo einstellen
}

void setServo(int w) {
  // set Servo on Master
}

int readGyro() {
  int rate, vref;
  vref = analogRead(pinRef);         // Liest Analog input Ref
  rate = analogRead(welchesGyro);
  return rate - vref;
}

double gyroToWinkel(int wint) {
  // Gyro auslesen und verarbeiten, gibt die Winkelgeschwindigkeit
  // in Grad pro Sekunde zurück, Ermittlung des Faktors siehe Zeilen 19, 20
  double factor, omega;
  switch (welchesGyro) {
    case pinX1:
      factor = factor2;
      break;
    case pinZ1:
      factor = factor2;
      break;
    case pinX2:
      factor = factor45;
      break;
    case pinZ2:
      factor = factor45;
      break;
  }
  omega = factor * (double) wint;
  if (abs(omega) < schwelle) {
    omega = 0.0;
  }
  return omega;
}

int checkGyro() {
  byte result;
  // check Gyro on Slave
  int w = 0;
  byte c;
  Wire.beginTransmission(adrSlave);
  Wire.write(0);                        // send value byte
  result = Wire.endTransmission();      // stop transmitting
  Wire.requestFrom(adrSlave, sizeof(int));
  // while (Wire.available()) {
  for (int i = 0; i < sizeof(int); i++) {
    c = Wire.read();
    inBuffer[sizeof(int) - i] = c;
  }  // end of for loop
  // }
  w = (int) inBuffer;
  Serial.print("Master: ");
  Serial.println(w);
  return w;
}

void slaveHandler(int howMany) {
  byte c;
  int w;
  c = Wire.read();    //  1 byte
  w = readGyro();
  // Wire.beginTransmission (0);
  for (int i = 0; i < sizeof(int); i++) {
    c = w & 0xFF;     // lowest byte
    // Wire.write(c);
    Wire.write(0);
    inBuffer[i] = c;
    c = (w >> 8) & 0xFF;
  }
  // Wire.endTransmission();
  Serial.print("Slave: ");
  Serial.println(w);
}

void TC6_Handler() {
  int w;
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if ((timerValue % 100) == 0) {        // alle 100 ms
    if (isMaster) {
      w = checkGyro();
      setServo(w);
    }
  }
  if ((timerValue % 1000) == 0) {       // every 1000 ms
    //
  }
}

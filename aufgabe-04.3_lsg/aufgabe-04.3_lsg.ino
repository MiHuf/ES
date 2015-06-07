// Aufgabe 4.3, Stand von 2015-06-07
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Wire.h>
#include <Servo.h>

// Pin Constants
const int pinLed = 13;                  // internal LED
const int pinIsMaster = 8;              // Pin 8 = 3,3 V for Master, = GND for Slave
// I2C pins are SDA = 20, SCL = 21

// for Gyro on Slave Board
const int pinX1 = A2;                   // Analog Gyro XOUT
const int pinZ1 = A3;                   // Analog Gyro ZOUT
const int pinX2 = A4;                   // Analog Gyro X4.5OUT
const int pinZ2 = A5;                   // Analog Gyro Z4.5OUT
const int pinRef = A6;                  // Analog Gyro VREF
const int pinAZ = 7;                    // Digital I/O Gyro
// factor = (5000.0 mV / 1024.0 steps) / senitivity in [mv / Grad / sec]
// Winkelgeschwindigkeit in Grad / Sekunde = factor * (rate - vref)
const double factor2 = (5000.0 / 1024.0) / 2.0;
const double factor45 = (5000.0 / 1024.0) / 9.1;

// for Servo on Master Board
const int servoPin = 3;                 // Servo-Pin
const int wmin = 24;                    // Servo-Grenzwert, ermittelt
const int wmax = 159;                   // Servo-Grenzwert, ermittelt
const double schwelle = 1.0;            // Schwellwert fuer Gyro-Ausgabe
int welchesGyro = pinZ1;                // evtl. anpassen

// Other Constants
const int adrSlave = 17;                // my birthdate

// Timer Params
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABpinLed
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000

// Variables
bool isMaster = true;                   // Hardware configured
const int len = sizeof(int);            // length of line buffer
byte inBuffer[len];                     // data bufferconst int len = 160;
int pos = 0;                            // position within line buffer
uint32_t timerValue = 0;                // Timer value in milliseconds
Servo myServo;                          // Servo-Objekt erstellen
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
  myServo.attach(servoPin);            // Servo an Pin 3
  myServo.write(winkel);               // Mittelstellung

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
  pinMode(pinIsMaster, INPUT);
  isMaster = digitalRead(pinIsMaster);
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

double gyroToWinkel(int wint) {
  // Ermittelt aus wint = rate - vref die Winkelgeschwindigkeit
  // in Grad pro Sekunde, Ermittlung des Faktors siehe Zeilen 20, 21
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

void setServo(int w) {
  // set Servo on Master according to w = rate - vref
  double omega;
  omega = gyroToWinkel(w);              // Winkelgeschwindigkeit Grad pro Sekunde
  winkel = winkel + int (0.1 * omega + 0.5); // increment 10x per second
  if (winkel < wmin) {
    // setBlink();
    winkel = wmin;
  }
  if (winkel > wmax) {
    // setBlink();
    winkel = wmax;
  }
  Serial.print("Setting Winkel on Master = ");
  Serial.println(winkel);                // Ausgabe zur Kontrolle
  myServo.write(winkel);                 // Servo einstellen
}

int readGyro() {
  // read analog values from Gyro and return raw data rate - vref
  int rate, vref;
  vref = analogRead(pinRef);            // read Analog input Ref
  rate = analogRead(welchesGyro);       // read Analog Input ZOUT
  return rate - vref;
}

int checkGyro() {
  // check Gyro on Slave, returns rate - vref
  int w = 0;
  byte result;
  byte c;
  //  Wire.beginTransmission(adrSlave);
  //  Wire.write(7);                        // send any byte
  //  result = Wire.endTransmission();      // stop transmitting
  Wire.requestFrom(adrSlave, sizeof(int));
  for (int i = 0; i < sizeof(int); i++) {
    c = Wire.read();                    // read byte, lowest first
    inBuffer[sizeof(int) - i] = c;      // fill inBuffer from right to left
  }  // end for loop
  w = (int) inBuffer;
  Serial.print("Master: ");
  Serial.println(w);
  return w;
}

void slaveHandler(int howMany) {
  byte c;
  int w;
  //  c = Wire.read();                      // read one byte and ignore
  w = readGyro();
  Serial.print("Slave: ");
  Serial.println(w);
  for (int i = 0; i < sizeof(int); i++) {
    c = w & 0xFF;                         // get lowest byte
    Wire.write(c);                        // transmit
    w = (w >> 8);                         // right shift for next byte
  }
}

void TC6_Handler() {
  int w;                                // = rate - vref
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if ((timerValue % 100) == 0) {        // every 100 ms
    if (isMaster) {
      w = checkGyro();
      setServo(w);
    }
  }
  if ((timerValue % 1000) == 0) {       // every 1000 ms
    //
  }
}

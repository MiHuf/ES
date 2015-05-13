// Aufgabe 3.3, Stand von 2015-05-13
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Servo.h>

// for Gyro
const int pinX1 = A2;           // Analog Gyro XOUT
const int pinZ1 = A3;           // Analog Gyro ZOUT
const int pinX2 = A4;           // Analog Gyro X4.5OUT
const int pinZ2 = A5;           // Analog Gyro Z4.5OUT
const int pinRef = A6;          // Analog Gyro VREF
const int pinAZ = 7;            // Digital I/O Gyro
const double x1Sens = 2.0;      // Sensitivity x-Out
const double z1Sens = 2.0;      // Sensitivity z-Out
const double x2Sens = 9.1;      // Sensitivity x4.5-Out
const double z2Sens = 9.1;      // Sensitivity z4.5-Out
// for Servo
const int servoPin = 3;         // Servo-Pin
const int wmin = 24;            // Servo-Grenzwert, ermittelt
const int wmax = 159;           // Servo-Grenzwert, ermittelt
const double schwelle = 5.0;    // Schwellwert fuer Servo-Ausgabe
int welchesGyro = pinZ2;        // evtl. anpassen
// for LED
const int led = 13;             // Internal LED

// Timer Params and Variables
const uint32_t dwChannel = 0;
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000
uint32_t timerValue = 0;        // Millisekunden
int blinkTimer = 14;            // zu kompliziert zu erklaeren

// Variables
Servo neuServo;                 // Servo-Objekt erstellen
int zeit = 100;                 // Wartezeit in ms zwischen Abtastung des Gyros
int winkel = (wmax - wmin) / 2; // Mittelstellung am Anfang

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
  neuServo.attach(servoPin);     // Servo an Pin 3
  neuServo.write(winkel);        // Mittelstellung
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);        // LED OFF
  // Timer setup and start
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6);                   // Timer 2, channel 0
  TC_Configure(TC2, dwChannel, dwMode);
  TC2->TC_CHANNEL[dwChannel].TC_IER = 0b1 << 4;    // siehe Datenblatt Seite 917
  TC2->TC_CHANNEL[dwChannel].TC_IDR = ~(0b1 << 4); // siehe Datenblatt Seite 918
  NVIC_ClearPendingIRQ(TC6_IRQn);                  // Timer 2, channel 0
  NVIC_EnableIRQ(TC6_IRQn);                        // Timer 2, channel 0
  TC_SetRC(TC2, dwChannel, 41999);                 // = 84.000.000 / 2 - 1.000
  TC_Start(TC2, dwChannel);

  setBlink();                                      // am Anfang einmal testen

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void updateServo() {
  // Gyro auslesen, neuen Winkel berechnen und Servo einstellen
  double omega;
  omega = readGyro();  // Winkelgeschwindigkeit in Grad pro Sekunde
  winkel = winkel + int ((omega / 10.0) + 0.5);  // 10x pro Sekunde
//   winkel = (wmax - wmin) / 2 ;
  if (winkel < wmin) {
    setBlink();
    winkel = wmin;
  }
  if (winkel > wmax) {
    setBlink();
    winkel = wmax;
  }
  moveServo(winkel);
}

void moveServo(int w) {
  // Servo auf den Winkel w einstellen
  Serial.print("Winkel = ");
  Serial.println(w);                       // Ausgabe zur Kontrolle
  neuServo.write(w);                       // Servo einstellen
}

void setBlink() {
  if (blinkTimer <= 0) {
    blinkTimer = 14;
  }
}

void checkBlink() {
  // Serial.println(blinkTimer);              // Ausgabe zur Kontrolle
  if (blinkTimer > 0) {
    switch (blinkTimer) {
      case 14 : case 9 : case 4:
        digitalWrite(led, HIGH);        // LED ON
        break;
      case 11: case 6: case 1:
        digitalWrite(led, LOW);         // LED OFF
        break;
    }
    blinkTimer = blinkTimer -1;
  }
}

double readGyro() {
  // Gyro auslesen und verarbeiten, gibt die Winkelgeschwindigkeit
  // in Grad pro Sekunde zurück
  int Vref;
  int rate;
  double sens, omega;
  Vref = analogRead(pinRef);         // Liest Analog input Ref
  switch (welchesGyro) {
    case pinX1:
      rate = analogRead(pinX1);      // Liest Analog input x-Out
      sens = x1Sens;
      break;
    case pinZ1:
      rate = analogRead(pinZ1);      // Liest Analog input z-Out
      sens = z1Sens;
      break;
    case pinX2:
      rate = analogRead(pinX2);      // Liest Analog input x4.5-Out
      sens = x2Sens;
      break;
    case pinZ2:
      rate = analogRead(pinZ2);      // Liest Analog input z4.5-Out
      sens = z2Sens;
      break;
  }
  omega = ((double) rate - (double) Vref) / sens;
  if (abs(omega) < schwelle) {
    omega = 0.0;
  }
  return omega;
}

void TC6_Handler() {
  // tu was
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  if ((timerValue % zeit) == 0) {  // alle 100 ms
    updateServo();
    checkBlink();
  }
}


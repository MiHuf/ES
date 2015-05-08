// Aufgabe 3.3, Stand von 2015-05-08
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
int welchesGyro = pinX1;        // evtl. anpassen
// for LED
const int led = 13;             // Internal LED

// Variables
Servo neuServo;                 // Servo-Objekt erstellen
int zeit = 100;                 // Wartezeit zwischen Abtastung des Gyros
int winkel = (wmax - wmin) / 2; // Mittelstellung

void setup() {
  // put your setup code here, to run once:
  // Setup for Gyro
  pinMode(pinX1, INPUT);
  pinMode(pinZ1, INPUT);
  pinMode(pinX2, INPUT);
  pinMode(pinZ2, INPUT);
  pinMode(pinRef, INPUT);
  pinMode(pinAZ, OUTPUT);
  digitalWrite(pinAZ, LOW);
  delay(1000);
  digitalWrite(pinAZ, HIGH);
  // Setup for Servo
  neuServo.attach(servoPin);     // Servo an Pin 3
  neuServo.write(winkel);        // Mittelstellung
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);       // LED OFF

  Serial.begin(9600);
}

void loop() {
  double omega;
  // put your main code here, to run repeatedly:
  omega = readGyro();
  winkel = winkel + (int) (omega / 10.0);
  if (winkel < wmin) {
    blink();
    winkel = wmin;
  }
  if (winkel < wmax) {
    blink();
    winkel = wmax;
  }
  moveServo();
  delay(zeit);                             // Warten
}

void moveServo() {
  neuServo.write(winkel);                  // Sollwert schreiben
  Serial.print("Winkel = ");
  Serial.println(winkel);                  // Ausgabe zur Kontrolle
}

void blink() {
  // 3x LED blinken
  digitalWrite(led, HIGH);         // LED OFF
  for (int i = 0; i < 3; i++) {
    digitalWrite(led, LOW);        // LED ON
    delay(300);
    digitalWrite(led, HIGH);       // LED OFF
    delay(200);
  }
}

double readGyro() {
  // Gyro auslesen und verarbeiten
  int x1Rate, z1Rate;              // x-Out, z-Out
  int x2Rate, z2Rate;              // x4.5-Out , z4.5-Out
  int Vref;
  int rate;
  double sens, omega;
  x1Rate = analogRead(pinX1);      // Liest Analog input x-Out
  z1Rate = analogRead(pinZ1);      // Liest Analog input z-Out
  x2Rate = analogRead(pinX2);      // Liest Analog input x4.5-Out
  z2Rate = analogRead(pinZ2);      // Liest Analog input z4.5-Out
  Vref = analogRead(pinRef);       // Liest Analog input Ref
  switch (welchesGyro) {
    case pinX1:
      rate = x1Rate;
      sens = x1Sens;
      break;
    case pinZ1:
      rate = z1Rate;
      sens = z1Sens;
      break;
    case pinX2:
      rate = x2Rate;
      sens = x2Sens;
      break;
    case pinZ2:
      rate = z2Rate;
      sens = z2Sens;
      break;
  }
  omega = ((double) rate - (double) Vref) / sens;
  if (abs(omega) < schwelle) {
    omega = 0.0;
  }
  return omega;
}


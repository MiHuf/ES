// Aufgabe 3.2, Stand von 2015-05-08
// Lösung von Michael Hufschwmidt   michael@hufschwmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Servo.h>

const int AnalogPin = A0;        // AnalogPin 0
const int servoPin = 3;          // Servo-Pin
Servo neuServo;                  // Servo-Objekt erstellen
int val1, val2;                  // Sollwerte fuer Servo
int wmin = 24;                   // Servo-Grenzwert ermittelt
int wmax = 159;                  // Servo-Grenzwert ermittelt
int wmid = (wmax - wmin) / 2;    // Mittelwert
int zeit = 500;                  // Wartezeit zwischen den Servo-Befehlen

void setup() {
  // put your setup code here, to run once:
  neuServo.attach(servoPin);     // Servo an Pin 3
  neuServo.write(wmid);          // Mittelstellung
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int w = wmin; w < wmax; w = w + 5) {
    Serial.print("----------\nAmplitude  = ");
    Serial.println(w);                  // Ausgabe zur Kontrolle
    moveServo(wmid);
    moveServo(w);
    moveServo(wmid);
  }
}

void moveServo(int w) {
  Serial.print("Winkel = ");
  Serial.println(w);                       // Ausgabe zur Kontrolle
  neuServo.write(w);                       // Sollwert einstellen
  delay(zeit);                             // Warten
}


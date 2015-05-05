// Aufgabe 3.2, Stand von 2015-05-05
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

//   0 Grad Wert 0 = 0V     ; Servo Position Links
//  90 Grad Wert 512 = 2,5V ; Servo Position Mitte
// 180 Grad Wert 1023 = 5V  ; Servo Position Rehts

#include <Servo.h>

const int AnalogPin = 0;         // AnalogPin 0
const int servoPin = 3;          // Servo-Pin
Servo neuServo;                  // Servo-Objekt erstellen
bool Teil321 = true;             // Aufgabe 3.2.1, = false fuer Aufgabe 3.2.2
int val1, val2;                  // Sollwerte fuer Servo
int winkel = 90;                 // Diese Variable enthaelt die Position des Servo, 0 bis 180
int zeit = 20;                   // Wartezeit 20ms zwischen den Servo-Befehlen

void setup() {
  // put your setup code here, to run once:
  neuServo.attach(servoPin);     // Servo an Pin 3
  neuServo.write(winkel);        // 90 Grad = Mittelstellung
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Teil321) {                            // Teil 3.2.1
    for (winkel = 90; winkel < 180; winkel++) {
      neuServo.write(winkel);               // Servo anweisen, die Position in der Variablen 'winkel' einzunehmen
      delay(zeit);                          // Warten
    }
    for (winkel = 180; winkel >= 90; winkel--) { // Von 180 Grad bis 90 Grad
      neuServo.write(winkel);               // Servo in Gegenrichtung bewegen
      delay(zeit);                          // Warten
    }
  } else {                                  // Teil 3.2.2
    val1 = analogRead(AnalogPin);           // Sollwert einlesen
    winkel = map(val1, 512, 1023, 90, 180); // Sollwert skalieren: 512-1023 entsprechen 90-180
    neuServo.write(winkel);                 // Sollwert schreiben
    delay(zeit);                            // Warten

    val2 = analogRead(AnalogPin);           // Sollwert einlesen
    winkel = map(val2, 1023, 512, 180, 90); // Sollwert skalieren: 512-1023 entsprechen 90-180
    neuServo.write(winkel);                 // Sollwert schreiben
    delay(zeit);                            // Warten
  }
}


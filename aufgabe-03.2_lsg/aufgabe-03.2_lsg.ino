// Aufgabe 3.2, Stand von 2015-05-06
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

//   0 Grad Wert 0 = 0V     ; Servo Position Links
//  90 Grad Wert 512 = 2,5V ; Servo Position Mitte
// 180 Grad Wert 1023 = 5V  ; Servo Position Rehts

#include <Servo.h>

const int AnalogPin = A0;        // AnalogPin 0
const int servoPin = 3;          // Servo-Pin
Servo neuServo;                  // Servo-Objekt erstellen
bool Teil321 = true;             // Aufgabe 3.2.1, = false fuer Aufgabe 3.2.2
int val1, val2;                  // Sollwerte fuer Servo
int wmin = 28;                   // Servo-Grenzwert ermittelt
int wmax = 160;                  // Servo-Grenzwert ermittelt
int mid = (wmax - wmin) / 2;     // Mittelwert
int winkel = mid;                // Aktuelle Position des Servo, wmin bis wmax
int zeit = 500;                  // Wartezeit zwischen den Servo-Befehlen

void setup() {
  // put your setup code here, to run once:
  neuServo.attach(servoPin);     // Servo an Pin 3
  neuServo.write(winkel);        // Mittelstellung
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Teil321) {                            // Teil 3.2.1
    for (int w = 0; w < mid; w = w + 5) {
      Serial.print("----------\nAmplitude  = ");
      Serial.println(w);                  // Ausgabe zur Kontrolle
      sweep(w);
    }
  } else {                                  // Teil 3.2.2 ???
    val1 = analogRead(AnalogPin);           // Sollwert einlesen
    winkel = map(val1, 0, 1023, wmin, wmax);// Sollwert skalieren: 0-1023 entsprechen wmin-wmax
    moveServo();
  }
}

void sweep(int width) {
  // move between mid +- width
  winkel = mid;
  moveServo();
  winkel = mid + width;
  moveServo();
  winkel = mid - width;
  moveServo();
  winkel = mid;
  moveServo();
  delay(zeit);
}

void moveServo() {
  neuServo.write(winkel);                  // Sollwert schreiben
  Serial.print("Winkel = ");
  Serial.println(winkel);                  // Ausgabe zur Kontrolle
  delay(zeit);                             // Warten
}

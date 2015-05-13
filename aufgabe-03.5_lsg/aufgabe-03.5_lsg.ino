// Aufgabe 3.5, Stand von 2015-05-13
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Servo.h>

// for Servo
const int servoPin = 3;         // Servo-Pin
const int wmin = 24;            // Servo-Grenzwert, ermittelt
const int wmax = 159;           // Servo-Grenzwert, ermittelt
const double schwelle = 1.0;    // Schwellwert fuer Gyro-Ausgabe
const double gain = 0.1;        // Verstaerkung fuer Gyro
// for LED
const int led = 13;             // Internal LED

// Variables
String readString;
Servo neuServo;                 // Servo-Objekt erstellen
int zeit = 100;                 // Wartezeit in ms zwischen Abtastung des Gyros
int winkel = 0; // Mittelstellung am Anfang

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);        // LED OFF
  neuServo.attach(servoPin);     // Servo an Pin 3

  // moveServo(winkel);
  Serial.begin(9600);
}

void loop() {
  while (Serial.available()) {
    winkel = Serial.parseInt();
    // end while
    if (winkel != 0) {
      //   Serial.println(winkel);  //so you can see the captured string
      if (winkel < wmin) {
        Serial.println("Winkel zu klein");
      } else if (winkel > wmax) {
        Serial.println("Winkel zu gross");
      } else {
        moveServo(winkel);
      }
    }
    delay(2);
  } // end if winkel
} // end loop

void moveServo(int w) {
  // Servo auf den Winkel w einstellen
  Serial.print("Winkel = ");
  Serial.println(w);                       // Ausgabe zur Kontrolle
  neuServo.write(w);                       // Servo einstellen
}



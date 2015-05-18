// Aufgabe 4.3, Stand von 2015-05-18
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Wire.h>

// for LED
const int led = 13;             // Internal LED

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);        // LED OFF
}

void loop() {
}  // end loop


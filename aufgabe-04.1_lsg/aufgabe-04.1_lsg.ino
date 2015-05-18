// Aufgabe 4.1, Stand von 2015-05-18
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

// for LED
const int led = 13;             // Internal LED
const int pinTX3 = 14;          // Serial 3
const int pinRX3 = 15;          // Serial 3
const int pinTX2 = 16;          // Serial 2
const int pinRX2 = 17;          // Serial 2

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);        // LED OFF
}

void loop() {
}  // end loop


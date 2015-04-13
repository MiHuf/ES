// Aufgabe 1.4, Version 2015-04-13
// Lösung von Rania rania_wittenberg@hotmail.com, Tim tw@ens-fiti.de, Michael Michael@hufschmidt-web.de
int swli = 4;
int swre = 9;
int led = 7;
byte bright = 0;

void setup() {
  pinMode(led, OUTPUT);
  analogWrite(led, bright);
  Serial.begin(9600);
}

void loop() {
  if ((millis() % 50) == 0) {
    bright++ ;
  }
  analogWrite(led, bright);
  Serial.println(bright);
}


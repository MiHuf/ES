// Aufgabe 1.1
// Lösung von Rania, Tim, Michael, Version 2015-04-14

int swli = 4;
int swre = 9;
int led = 7;
void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);
  delay(500);
}

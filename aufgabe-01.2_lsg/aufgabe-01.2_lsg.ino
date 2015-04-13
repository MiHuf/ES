
// Aufgabe 1.2
// Lösung von Rania, Tim, Michael
int swli = 4;
int swre = 9;
int led = 7;

void setup() {
  pinMode(swli, INPUT);
  pinMode(swre, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  if (!digitalRead(swli)) {
    digitalWrite(led, HIGH);
  }
  if (!digitalRead(swre)) {
    digitalWrite(led, LOW);
  }
}

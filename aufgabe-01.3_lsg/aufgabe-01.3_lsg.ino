
// Aufgabe 1.3
// Lösung von Rania, Tim, Michael
int swli = 4;
int swre = 9;
int led = 7;

void setup() {
  pinMode(swli, INPUT);
  attachInterrupt(swli, doLi, FALLING);
  pinMode(swre, INPUT);
  attachInterrupt(swre, doRe, FALLING);
  pinMode(led, OUTPUT);
  switchLed(LOW);
}

void loop() {
}

void switchLed(boolean mode) {
  digitalWrite(led, mode);
}

void doLi() {
  switchLed(HIGH);
}
void doRe() {
  switchLed(LOW);
}

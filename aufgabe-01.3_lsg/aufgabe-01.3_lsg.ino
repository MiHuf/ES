
// Aufgabe 1.3
// Lösung von Rania, Tim, Michael
int swli = 4;
int swre = 9;
int led = 13;
boolean mode = false;

void setup() {
  pinMode(swli, INPUT);
  attachInterrupt(swli, doLi, FALLING);
  pinMode(swre, INPUT);
  attachInterrupt(swre, doRe, FALLING);
  pinMode(led, OUTPUT);
  switchLed();
}

void loop() {
 switchLed();
}

void switchLed() {
  digitalWrite(led, mode);
}

void doLi() {
 mode = !mode;
}
void doRe() {

}

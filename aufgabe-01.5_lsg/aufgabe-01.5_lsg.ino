// Aufgabe 1.5, Version 2015-04-13
// Lösung von Rania rania_wittenberg@hotmail.com, Tim tw@ens-fiti.de, Michael Michael@hufschmidt-web.de
int swli = 4;
int swre = 9;
int led = 7;
byte bright = 0;
boolean rise = true;   // increase brightness
boolean poll = true;   // Fuer Aufgabe 1.5 a

void setup() {
  pinMode(swli, INPUT);
  pinMode(swre, INPUT);
  if (!poll) {
    attachInterrupt(swli, doLi, FALLING);
    attachInterrupt(swre, doRe, FALLING);
  }
  pinMode(led, OUTPUT);
  analogWrite(led, bright);
  Serial.begin(9600);
}

void loop() {
  changeBrightness();
  analogWrite(led, bright);
  Serial.println(bright);
}

void changeBrightness() {
  if ((millis() % 50) != 0) {
    return;
  }
  if (poll) {
    // Controllingby polling
    if (!digitalRead(swli)) {
      bright++ ;
    }
    if (!digitalRead(swre)) {
      bright-- ;
    }
  } else {
    // Controlling via interrupt
    if (rise) {
      bright++ ;
    } else {
      bright--;
    }
  }
}

void doLi() {
  rise = true;
}
void doRe() {
  rise = false;
}

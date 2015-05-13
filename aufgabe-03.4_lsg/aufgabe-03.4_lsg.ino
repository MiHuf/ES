// Aufgabe 3.4, Stand von 2015-05-13
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

String readString;
int swli = 4;
int swre = 9;
int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();          //gets one byte from serial buffer
    if (c != '\n') {
      readString += c;
    } else {
      if (readString.length() > 0) {
        Serial.println(readString);  //so you can see the captured string
        if (readString == "LedOn") {
          digitalWrite(led, HIGH);
        }
        if (readString == "LedOff") {
          digitalWrite(led, LOW);
        }
        readString = "";
      }
    }
    delay(2);
  }
}

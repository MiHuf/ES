// Aufgabe 3.5, Stand von 2015-05-20
// Version mit serialEvent()
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

#include <Servo.h>

// for Servo
const int servoPin = 3;                 // Servo-Pin
const int wmin = 24;                    // Servo-Grenzwert, ermittelt
const int wmax = 159;                   // Servo-Grenzwert, ermittelt
// for LED
const int led = 13;                     // Internal LED
// for Strings
const int len = 160;                    // Länge Zeilenpuffer
const String ziffern = " 0123456789+-"; //

// Variables
char zeile[len];                        // Zeilenpuffer
int pos = 0;                            // Pointer auf ein Zeichen
String command;
Servo neuServo;                         // Servo-Objekt erstellen
int zeit = 100;                         // Wartezeit in ms zwischen Abtastung des Gyros
int winkel = (wmax - wmin) / 2;         // Mittelstellung am Anfang

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);               // LED OFF
  neuServo.attach(servoPin);            // Servo an Pin 3

  moveServo(winkel);
  Serial.begin(9600);
  pos = 0;
}

void loop() {
}  // end loop

void parseCommand() {
  int posZi, posBracket;                // Pointer im Command-String
  String sWinkel;                       // Winkel als String
  command = String(zeile);              // Konvertiere in einen String
  Serial.println(command);              // Zum Testen: Ausgabe auf dem Terminal
  posBracket = command.indexOf(")");
  if ((command.substring(0, 7).compareTo("moveTo(") == 0) && (posBracket > 0) ) {
    posZi = 7;
    while ((ziffern.indexOf(command.charAt(posZi)) >= 0) && (posZi < command.length() + 1)) {
      posZi ++;
    }
    sWinkel = command.substring(7, posZi);
//    Serial.print("posZi = ");           // zum Testen
//    Serial.println(posZi);
//    Serial.println(sWinkel);
    winkel = sWinkel.toInt();
    if ((posZi != posBracket) || (winkel == 0)) {
      Serial.println("ungueltiger Winkel");
    } else {
      if (winkel < wmin) {
        Serial.print(winkel);
        Serial.println(" ist zu klein");
      } else if (winkel > wmax) {
        Serial.print(winkel);
        Serial.println(" ist zu gross");
      } else {
        moveServo(winkel);
        delay(100);
      }
    }
  } else {
    Serial.println("unbekannter Befehl");
  }
}

void moveServo(int w) {
  // Servo auf den Winkel w einstellen
  Serial.print("Winkel = ");            // Ausgabe zur Kontrolle
  Serial.println(w);                    // Ausgabe zur Kontrolle
  neuServo.write(w);                    // Servo einstellen
}

void serialEvent() {
  // somebody wants to talk
  byte c;
  if (Serial.available()) {
    c = Serial.read();
    zeile[pos] = c;
    pos ++;                             // ready for next char
  }
  if (c == '\n') {
    zeile[pos] = 0;                     // terminate String
    pos = 0;
    parseCommand();
  }
}

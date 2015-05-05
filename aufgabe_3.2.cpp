// Aufgabe 3.2

 // 0 Grad Wert 0 = 0V; Servo Position Links
 // 90 Grad Wert 512 = 2,5V ; Servo Position Mitte
 // 180 Grad Wert 1023 = 5V ; Servo Position Rehts

#include <servo.h>


Servo neuServo;                 //Servo-Objekt erstellen
int AnalogPin=0;                //AnalogPin 0
int val, val1;                        //Sollwert für Servo

void setup(){

    neuServo.attach(3);             //Servo an Pin 3
    //Serial.begin(9600);
}
void loop(){
    val = analogRead(AnalogPin);    // Einlesen des Sollwertes für die Servoposition
    val = map(val, 512, 1023, 90, 180) // Skalieren des Sollwertes für den Servo 512-1023 entsprechen 90-180
    neuServo.write(val);            // Sollwert für Servoüposition schreiben
    delay(20);                      // Warten auf Servo , nach Pause wieder von vorne

    val1 = analogRead(AnalogPin);    // Einlesen des Sollwertes für die Servoposition
    val1 = map(val1, 1023, 512, 180, 90) // Skalieren des Sollwertes für den Servo 512-1023 entsprechen 90-180
    neuServo.write(val1);            // Sollwert für Servoüposition schreiben
    delay(20);                      // Warten auf Servo , nach Pause wieder von vorne
}

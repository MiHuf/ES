// Aufgabe 3.2

 // 0 Grad Wert 0 = 0V; Servo Position Links
 // 90 Grad Wert 512 = 2,5V ; Servo Position Mitte
 // 180 Grad Wert 1023 = 5V ; Servo Position Rehts

#include <servo.h>


Servo neuServo;                 //Servo-Objekt erstellen
int AnalogPin=0;                //AnalogPin 0
int val;                        //Sollwert für Servo
int winkel=0;                   //Diese Variable enthält die Position des Servo

void setup(){

    neuServo.attach(3);             //Servo an Port 3
   // Serial.begin(9600);
}

void loop(){

    for(winkel=90; winkel<180; winkel++){

        neuServo.write(winkel); // Servo anweisen, die Position in der Variablen 'winkel' einzunehmen
        delay(20);              // Warten 20ms zwischen den Servo-Befehlen
    }

    for(winkel=180; winkel>=90; winkel--){  // Von 180 Grad bis 90 Grad

        neuServo.write(winkel); //Servo in Gegenrichtung bewegen
        delay(20);              // Warten 20ms zwischen den Servo-Befehlen
    }
}

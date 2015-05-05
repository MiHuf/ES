// Aufgabe 3.1, Stand von 2015-05-05
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

const int pinX1 = 2;
const int pinZ1 = 3;
const int pinX2 = 4;
const int pinZ2 = 5;
const int pinRef = 6;

const double x1Sens = 2;    // Sensitivity x-Out
const double z1Sens = 2;    // Sensitivity z-Out
const double x2Sens = 9.1;  // Sensitivity x4.5Out
const double z2Sens = 9.1;  // Sensitivity z4.5Out
int x1Rate, z1Rate;         // x-Out, z-Out
int x2Rate, z2Rate;         // x4.5Out , z4.5Out
int zeit = 3000;            // Delay 1 s
int Vref;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  x1Rate = analogRead(pinX1);      // Liest Analog input pin 2
  z1Rate = analogRead(pinZ1);      // Liest Analog input pin 3
  x2Rate = analogRead(pinX2);      // Liest Analog input pin 4
  z2Rate = analogRead(pinZ2);      // Liest Analog input pin 5
  Vref = analogRead(pinRef);       // Liest Analog input pin 6

  Serial.print("Rotation x1 ,z1: ");
  Serial.print(x1Rate, DEC);    // Ausgabe Rotation  in Achse  X
  Serial.print(", ");           // leertaste
  Serial.print(z1Rate, DEC);    // Ausgabe Rotation  in Achse  Z
  Serial.print("\nWinkelgeschindigkeit x1-Out, z1-Out: ");
  Serial.print(winkelgeschwindigkeit(x1Rate, Vref, x1Sens), DEC);
  Serial.print(", ");
  Serial.print(winkelgeschwindigkeit(z1Rate, Vref, z1Sens), DEC);
  Serial.print("\n");           // neue Zeile
  Serial.print("\n");           // neue Zeile

  Serial.print("Rotation x2 ,z2: ");
  Serial.print(x2Rate, DEC);    // Ausgabe Rotation  in Achse  X
  Serial.print(", ");           // leertaste
  Serial.print(z2Rate, DEC);    // Ausgabe Rotation  in Achse  Z
  Serial.print("\nWinkelgeschindigkeit x2-Out, z2-Out: ");
  Serial.print(winkelgeschwindigkeit(x2Rate, Vref,  x2Sens), DEC);
  Serial.print(", ");
  Serial.print(winkelgeschwindigkeit(z2Rate, Vref,  z2Sens), DEC);
  Serial.print("\n");           // neue Zeile
  Serial.print("\n");           // neue Zeile

  delay(zeit);                   // Warten  100ms vor die nächste Lesung
}

double winkelgeschwindigkeit(int voltage, int zero, double sensitiv) {
  //Formel gemäß Seite 13 im Datenblatt
  return ((double) voltage - (double) zero) / sensitiv;
}

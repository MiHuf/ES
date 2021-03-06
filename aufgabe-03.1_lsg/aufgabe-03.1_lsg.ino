// Aufgabe 3.1, Stand von 2015-05-08
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

const int pinX1 = A2;       // Analog Gyro XOUT
const int pinZ1 = A3;       // Analog Gyro ZOUT
const int pinX2 = A4;       // Analog Gyro X4.5OUT
const int pinZ2 = A5;       // Analog Gyro Z4.5OUT
const int pinRef = A6;      // Analog Gyro VREF
const int pinAZ = 7;        // Digital I/O Gyro

const double x1Sens = 2.0;  // Sensitivity x-Out
const double z1Sens = 2.0;  // Sensitivity z-Out
const double x2Sens = 9.1;  // Sensitivity x4.5-Out
const double z2Sens = 9.1;  // Sensitivity z4.5-Out
int x1Rate, z1Rate;         // x-Out, z-Out
int x2Rate, z2Rate;         // x4.5-Out , z4.5-Out
int zeit = 100;             // Delay 100 ms
int Vref;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinX1, INPUT);
  pinMode(pinZ1, INPUT);
  pinMode(pinX2, INPUT);
  pinMode(pinZ2, INPUT);
  pinMode(pinRef, INPUT);
  pinMode(pinAZ, OUTPUT);
  digitalWrite(pinAZ, LOW);
  delay(1000);
  digitalWrite(pinAZ, HIGH);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  x1Rate = analogRead(pinX1);      // Liest Analog input x-Out
  z1Rate = analogRead(pinZ1);      // Liest Analog input z-Out
  x2Rate = analogRead(pinX2);      // Liest Analog input x4.5-Out
  z2Rate = analogRead(pinZ2);      // Liest Analog input z4.5-Out
  Vref = analogRead(pinRef);       // Liest Analog input Ref

  Serial.print("Rohdaten x1 ,z1, VRef: ");
  Serial.print(x1Rate, DEC);    // Ausgabe Rotation  in Achse  X
  Serial.print(", ");           // leertaste
  Serial.print(z1Rate, DEC);    // Ausgabe Rotation  in Achse  Z
  Serial.print(", ");           // leertaste
  Serial.print(Vref, DEC);      // Ausgabe Rotation  in Achse  Z
  Serial.print("\nWinkelgeschindigkeit x1, z1: ");
  Serial.print(winkelgeschwindigkeit(x1Rate, Vref, x1Sens), DEC);
  Serial.print(", ");
  Serial.print(winkelgeschwindigkeit(z1Rate, Vref, z1Sens), DEC);
  Serial.print("\n");           // neue Zeile

  Serial.print("Rohdaten x2 ,z2: ");
  Serial.print(x2Rate, DEC);    // Ausgabe Rotation  in Achse  X
  Serial.print(", ");           // leertaste
  Serial.print(z2Rate, DEC);    // Ausgabe Rotation  in Achse  Z
  Serial.print("\nWinkelgeschindigkeit x2, z2: ");
  Serial.print(winkelgeschwindigkeit(x2Rate, Vref,  x2Sens), DEC);
  Serial.print(", ");
  Serial.print(winkelgeschwindigkeit(z2Rate, Vref,  z2Sens), DEC);
  Serial.print("\n");           // neue Zeile
  Serial.print("\n");           // neue Zeile

  delay(zeit);                   // Warten  100ms vor die nächste Lesung
}

double winkelgeschwindigkeit(int voltage, int zero, double sensitivity) {
  //Formel gemäß Seite 13 im Datenblatt
  return ((double) voltage - (double) zero) / sensitivity;
}

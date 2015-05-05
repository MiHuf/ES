// Aufgabe 3.1, Stand von 2015-05-05
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

const int pinX1 = 2;
const int pinZ1 = 3;
const int pinX2 = 4;
const int pinZ2 = 5;
const int pinRef = 6;

const int xSens = 2;        //Sensitivity x-Out
const int zSens = 2;        //Sensitivity z-Out
const double x1Sens = 9.1;  //Sensitivity x4.5Out
const double z1Sens = 9.1;  //Sensitivity z4.5Out
int x1Rate, z1Rate;         // x-Out, z-Out
int x2Rate, z2Rate;         // x4.5Out , z4.5Out
int VREF;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  x1Rate = analogRead(pinX1);      // Liest Analog input pin 2
  z1Rate = analogRead(pinZ1);      // Liest Analog input pin 3
  x2Rate = analogRead(pinX2);     // Liest Analog input pin 4
  z2Rate = analogRead(pinZ2);     // Liest Analog input pin 5
  VREF = analogRead(pinRef);       // Liest Analog input pin 6

  Serial.println("Rotation x,z:");
  Serial.println(x1Rate, DEC);   // Ausgabe Rotation  in Achse  X
  Serial.println(" ");          // leertaste
  Serial.println(z1Rate, DEC);   // Ausgabe Rotation  in Achse  Z
  delay(100);                   // Warten  100ms vor die nächste Lesung

  Serial.println("Gyro Rotation x-Out: ");
  Serial.println(gyroRotationRate((double) x1Rate, (double) VREF, (double) xSens), DEC);
  Serial.println("\n ");
  Serial.println("Gyro Rotation z-Out: ");
  Serial.println(gyroRotationRate((double) z1Rate, (double) VREF, (double) zSens), DEC);

  Serial.println(x2Rate, DEC);  // Ausgabe Rotation  x4.5  in Achse  X
  Serial.println(" ");          // leertaste
  Serial.println(z2Rate, DEC);  // Ausgabe Rotation z4.5  in Achse  Z
  delay(100);                   // Warten  100ms vor die nächste Lesung
  Serial.println("\n ");

  Serial.println("Gyro Rotation x4.5Out: ");
  Serial.println(gyroRotationRate((double) x2Rate, (double) VREF, x1Sens), DEC);
  Serial.println("\n ");
  Serial.println("Gyro Rotation z4.5Out: ");
  Serial.println(gyroRotationRate((double) z2Rate, (double) VREF, z1Sens), DEC);
}

double gyroRotationRate(double voltage, double zero, double sensitiv) {
  // Gyro rotation rate=(Gyro Output Voltage - Gyro Zero-Rate Out) / Sensitivity
  return (voltage - zero) / sensitiv;
}

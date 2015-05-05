// Aufgabe 3.1

int xRate, zRate;           // x-Out, z-Out
int x1Rate, z2Rate;         // x4.5Out , z4.5Out
const int xSens = 2;        //Sensitivity x-Out
const int zSens = 2;        //Sensitivity z-Out
const double x1Sens = 9.1;  //Sensitivity x4.5Out
const double z1Sens = 9.1;  //Sensitivity z4.5Out
int VREF;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
    xRate = analogRead(2);      // Liest Analog input pin 2
    zRate = analogRead(3);      // Liest Analog input pin 3
    x1Rate = analogRead(4);     // Liest Analog input pin 4
    z1Rate = analogRead(5);     // Liest Analog input pin 5
    VREF = analogRead(6);       // Liest Analog input pin 6

    Serial.println("Rotation x,z:");
    Serial.println(xRate, DEC);   // Ausgabe Rotation  in Achse  X
    Serial.println(" ");          // leertaste
    Serial.println(zRate, DEC);   // Ausgabe Rotation  in Achse  Z
    delay(100);                   // Warten  100ms vor die nächste Lesung

    Serial.println("Gyro Rotation x-Out: ");
    Serial.println(gyroRotationRate((double) xRate, (double) VREF, (double) xSens),DEC);
    Serial.println("\n ");
    Serial.println("Gyro Rotation z-Out: ");
    Serial.println(gyroRotationRate((double) zRate, (double) VREF, (double) zSens),DEC);



    Serial.println(x1Rate, DEC);  // Ausgabe Rotation  x4.5  in Achse  X
    Serial.println(" ");          // leertaste
    Serial.println(z2Rate, DEC);  // Ausgabe Rotation z4.5  in Achse  Z
    delay(100);                   // Warten  100ms vor die nächste Lesung

    Serial.println("\n ");
    Serial.println("Gyro Rotation x4.5Out: ");
    Serial.println(gyroRotationRate((double) x1Rate, (double) VREF, x1Sens),DEC);
    Serial.println("\n ");
    Serial.println("Gyro Rotation z4.5Out: ");
    Serial.println(gyroRotationRate((double) z1Rate, (double) VREF, z1Sens),DEC);
}


double gyroRotationRate(double volltage, double zero, double sensitiv){

    // Gyro rotation rate=(Gyro Output Voltage – Gyro Zero-Rate Out) / Sensitivity
    return (volltage-zero)/sensitiv;

}

// Aufgabe 2.2, Stand von 2015-04-28
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

// Timer Params and Variables
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABLED
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000
uint32_t timerValue = 0;                                 // Millisekunden

// Other Params and Variables

const int motPWM = 2;
const int motIN1 = 4;    // Motor
const int motIN2 = 3;    // Motor
const int led = 13;             // interne LED
const uint32_t bounceTime = 32; // bounce time in milli-seconds
boolean ledOn = true;
int speed = 128;
bool plus;     // beschleunigt
bool cw;       // Clockwise

void setup() {
  // Timer setup and start
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6);                   // Timer 2, channel 0
  TC_Configure(TC2, dwChannel, dwMode);
  TC2->TC_CHANNEL[dwChannel].TC_IER = 0b1 << 4;    // siehe Datenblatt Seite 917
  TC2->TC_CHANNEL[dwChannel].TC_IDR = ~(0b1 << 4); // siehe Datenblatt Seite 918
  NVIC_ClearPendingIRQ(TC6_IRQn);                  // Timer 2, channel 0
  NVIC_EnableIRQ(TC6_IRQn);                        // Timer 2, channel 0
  TC_SetRC(TC2, dwChannel, 41999);                 // (84 MHz / 2 - 1 Hz) set to 1 kHz
  TC_Start(TC2, dwChannel);
  // Other setup
  pinMode(led, OUTPUT);
  pinMode(motPWM, OUTPUT);
  pinMode(motIN1, OUTPUT);
  pinMode(motIN2, OUTPUT);
  digitalWrite(led, ledOn);
  digitalWrite(motIN1, LOW);
  digitalWrite(motIN2, HIGH);
  analogWrite(motPWM, speed);
  Serial.begin(9600);
}

void toggleDirection() {
  if (cw) {
    digitalWrite(motIN1, LOW);
    digitalWrite(motIN2, HIGH);
  } else {
    digitalWrite(motIN1, HIGH);
    digitalWrite(motIN2, LOW);
  }
  // Serial.println( cw);  // geht nicht
  cw = !cw;
}

void controlMotor() {
  if (plus) {
    if (speed < 255) speed++ ;
  } else {
    if (speed > 0) speed--;
  }
  if (speed >= 255) {
    toggleDirection();
  }
  if (speed <= 0) {
    toggleDirection();
  }
  // Serial.println(speed);  // geht nicht
  analogWrite(motPWM, speed);
}
void loop() {
  // put your main code here, to run repeatedly:
}

void switchLed() {
  ledOn = not(ledOn);
  digitalWrite(led, ledOn);
}

void TC6_Handler() {
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if (plus) {
    if ((timerValue % 20) == 0) {  // 5 Sekunden von 0 bis 255
      controlMotor();
    }
    if ((timerValue % 2) == 0) {   // 0,5 Sekunden von 255 bis 0
      controlMotor();
    }
  }
}


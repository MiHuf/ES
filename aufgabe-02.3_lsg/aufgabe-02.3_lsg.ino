// Aufgabe 2.3, Stand von 2015-04-29
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

typedef struct {
  int pin;
  bool validStatus;
  bool actualStatus;
  uint32_t bouncing;
} Key;

typedef Key * pKey;
Key swli, swre;

// Timer Params and Variables
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABLED
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000
uint32_t timerValue = 0;                                 // Millisekunden

// Other Params and Variables
const int motPWM = 2;           // Motor PWM
const int motIN1 = 4;           // Motor 1
const int motIN2 = 3;           // Motor 2
const int swlipin = 6;          // swli
const int swrepin = 7;          // swre
const int led = 13;             // interne LED
const uint32_t bounceTime = 32; // bounce time in milli-seconds
boolean ledOn = true;
int mode = -1;                  // 0 = stop, 1 = cw, 2 = stop , 3 = ccw
bool cw = true;
int speed = 128;                // medium speed
int s;
bool accelerate = true;         // increase speed with swre


void keyInit(void * p, int pin) {
  pKey pk = (pKey) p;
  pk->pin = pin;
  pinMode(pk->pin, INPUT);
  pk->validStatus = digitalRead(pk->pin);
  pk->actualStatus = pk->validStatus;
  pk->bouncing = bounceTime;
}

bool checkKey (void * p) {
  pKey pk = (pKey) p;
  pk->actualStatus = digitalRead(pk->pin);
  if (pk->actualStatus == pk->validStatus) {      // nothing happened, do nothing
    return false;
  } else {                                        // key has been pressed
    if (pk->bouncing == 0) {                      // bounce time exceeded?
      pk->bouncing = bounceTime;                  // reset timer
      pk->validStatus = pk->actualStatus;         // accept as valid
      return true;
    } else {
      pk->bouncing --;                            // decrement and check again later
      return false;
    };
  };
  return false;
}

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
  keyInit(&swli, swlipin);
  keyInit(&swre, swrepin);
  pinMode(led, OUTPUT);
  pinMode(motPWM, OUTPUT);
  pinMode(motIN1, OUTPUT);
  pinMode(motIN2, OUTPUT);
  digitalWrite(led, ledOn);
  digitalWrite(motIN1, LOW);
  digitalWrite(motIN2, LOW);
  analogWrite(motPWM, speed);
  Serial.begin(9600);
}

void nextMode() {
  mode++;
  Serial.println(mode % 3);
  switch (mode % 3) {
    case 0:
      setCw();
      break;
    case 1:
      setCcw();
      break;
    case 2:
      stopMotor();
      break;
  }
}

void setCw() {
  // digitalWrite(led, true);
  digitalWrite(motIN1, LOW);  // set CW
  digitalWrite(motIN2, HIGH);
}
void setCcw() {
  // digitalWrite(led, true);
  digitalWrite(motIN1, HIGH);   // set CCW
  digitalWrite(motIN2, LOW);
}
void stopMotor() {
  digitalWrite(motIN1, LOW);
  digitalWrite(motIN2, LOW);
}
void controlSpeed() {
  if (accelerate) {
    speed = speed + 16;
  } else {
    speed = speed - 16;
  }
  if (speed >= 255) {
    speed = 256;
    accelerate = false;
  }
  if (speed <= 0) {
    speed = 0;
    accelerate = true;
  }
  s = constrain(speed, 0, 255);
  analogWrite(motPWM, s);
  // analogWrite(led, s);
  Serial.println(speed);
}
void switchLed() {
  ledOn = not(ledOn);
  digitalWrite(led, ledOn);
}

void doSwli() {
  if (swli.validStatus == LOW) {
    nextMode();
  }
}

void doSwre() {
  if (swre.validStatus == LOW) {
    controlSpeed();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

void TC6_Handler() {
  uint32_t stat;
  stat = TC_GetStatus(TC2, dwChannel);
  timerValue = timerValue + 1;
  // tu was
  if (checkKey(&swli)) doSwli();
  if (checkKey(&swre)) doSwre();
}


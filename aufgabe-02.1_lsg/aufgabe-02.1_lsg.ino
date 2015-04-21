// Aufgabe 2.1, Stand von 2015-04-21
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de,
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com


// Timer Params and Variables
const uint32_t dwChannel = 0;
// TCLK1 (bits 0, 1, 2 ) = 0 ,
// WAVSEL (bits 13, 14)  = 2 = UPRC,
// WAVE (bit 15) = 1 = ENABLED
const uint32_t dwMode = 0b000 | 0b10 << 13 | 0b1 << 15;  // = 49152 = C000


// Other Params and Variables
const int led = 13;  // interne LED
boolean ledOn = false;

void setup() {
  // Timer setup and start
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6);
  TC_Configure(TC2, dwChannel, dwMode);
  NVIC_ClearPendingIRQ(TC6_IRQn);  // Timer 2, channel 0
  NVIC_EnableIRQ(TC6_IRQn);        // Timer 2, channel 0
  TC_SetRC(TC2, dwChannel, 41999); // (84 MHz / 2) - 1 Hz
  TC_Start(TC2, dwChannel);
  // Other setup
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void switchLed() {
  ledOn = !ledOn;
  digitalWrite(led, ledOn);
}

void TC6_Handler() {
  uint32_t stat, value;
  stat = TC_GetStatus(TC2, 0);
  value = TC_ReadCV(TC2, 0);
  // tu was


}

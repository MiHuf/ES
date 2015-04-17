// Aufgabe 2.1, Stand von 2015-04-17
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de, 
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com


// Timer Params
uint32_t dwFreq = 1000;  // ???
uint32_t dwMCk = 0;  // ???
uint32_t dwBoardMCK = 84000000;  // ???
uint32_t *dwDiv;
uint32_t *dwTcClks;
uint32_t mckDivisor;
uint32_t dwMode = 0; // ???

void setup() {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6);
  // >> Hier erfolgt die Konfiguration des Timers
  /**
  * \brief Configures a Timer Counter Channel, 
  * Configures a Timer Counter to operate in the given mode. Timer is stopped
  * after configuration and must be restarted with TC_Start(). All the
  * interrupts of the timer are also disabled.
  *
  * \param pTc  Pointer to a Tc instance.
  * \param channel Channel number.
  * \param mode  Operating mode (TC_CMR value).
  * extern void TC_Configure( Tc *pTc, uint32_t dwChannel, uint32_t dwMode )
  */
  TC_Configure(TC2, 0, dwMode);
  NVIC_ClearPendingIRQ(TC6_IRQn);
  NVIC_EnableIRQ(TC6_IRQn);
  mckDivisor = TC_FindMckDivisor(dwFreq, dwMCk, dwDiv, dwTcClks, dwBoardMCK);
  TC_SetRC(TC2, 0, mckDivisor);

  // >> Hier wird der konfigurierte Timer gestartet
  /**
   * \brief Reset and Start the TC Channel
   *
   * Enables the timer clock and performs a software reset to start the counting.
   *
   * \param pTc  Pointer to a Tc instance.
   * \param dwChannel Channel number.
   * extern void TC_Start( Tc *pTc, uint32_t dwChannel )
   */
   TC_Start(TC2, 0);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}

void TC6_Handler() {
  uint32_t stat, value;
  stat = TC_GetStatus(TC2, 0);
  value = TC_ReadCV(TC2, 0);
  // tu was
  

}

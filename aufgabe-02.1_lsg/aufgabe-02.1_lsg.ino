// Aufgabe 2.1, Stand von 2015-04-14
// Lösung von

void setup() {
  pmc_set_writeprotect(false);
  // pmc_enable_periph_clk( >> PMC id des Timers << );
  // >> Hier erfolgt die Konfiguration des Timers
  /**
  * \brief Configures a Timer Counter Channel
  *
  * Configures a Timer Counter to operate in the given mode. Timer is stopped
  * after configuration and must be restarted with TC_Start(). All the
  * interrupts of the timer are also disabled.
  *
  * \param pTc  Pointer to a Tc instance.
  * \param channel Channel number.
  * \param mode  Operating mode (TC_CMR value).
  * extern void TC_Configure( Tc *pTc, uint32_t dwChannel, uint32_t dwMode )
  */
  TC_Configure(TC0, 0, 0);
  // NVIC_ClearPendingIRQ( >> NVIC irq des Timers << );
  // NVIC_EnableIRQ( >> NVIC irq des Timers << );

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

  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}

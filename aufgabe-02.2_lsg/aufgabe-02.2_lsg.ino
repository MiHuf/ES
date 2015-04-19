// Aufgabe 2.2, Stand von 2015-04-14
// Lösung von Michael Hufschmidt   michael@hufschmidt-web.de, 
//            Tim Welge            tw@ens-fiti.de
//            Rania Wittenberg     rania_wittenberg@hotmail.com

void setup() {
  pmc_set_writeprotect(false);
  // pmc_enable_periph_clk( >> PMC id des Timers << );
  // >> Hier erfolgt die Konfiguration des Timers
  TC_Configure(TC0, 0, 0);
  // NVIC_ClearPendingIRQ( >> NVIC irq des Timers << );
  // NVIC_EnableIRQ( >> NVIC irq des Timers << );
  // >> Hier wird der konfigurierte Timer gestartet
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
 * Library KeyHandler, Version of 2015-05-05
 * Author: Michael Hufschmidt
 * with help from Tim Welge and Rania Wittenberg
 * Thanks to Norman Hendrich for debugging
 *
 * Usage:
 * ------
 * 1. Declare objects for each pushbotton:
 *      KeyHandler pb1, pb2, ...
 * 2. Initialise each key within the setup() method and assign a pin:
 *     pb1.keyInit(4);
 *     pb2.keyInit(5);
 * 3. In an Interrupt-Routine (e.g. TC6_Handler() ) check for key action:
 *     if (pb1.checkKey()) doPb1();
 * 4. create a function void doPb1() for acting:
 *     if (validLow())  ...
 */

#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

#include <Arduino.h>
class KeyHandler {
  public:
    int pin;
    bool validStatus;
    bool actualStatus;
    uint32_t bouncing;
    uint32_t bounceTime;

    void keyInit(int thePin);
    // Initialize key and assign pin. Parameter: thePin = pin number (e.g. 4 )

    bool checkKey ();
    // Returns true if key has been operated and bounce time exceeded.
    // If true, you can call getVaildStatus(), validLow() or validHIGH().

    bool getValidStatus();
    // return validStatus;
    bool validLow();
    // returns true is validStatus which is LOW
    bool validHigh();
    // returns true is validStatus which is HIGH
};
#endif // KEY_HANDLER_H

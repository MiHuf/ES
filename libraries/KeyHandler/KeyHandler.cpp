/*
 * Library KeyHandler, Version of 2015-05-05
 * Author: Michael Hufschmidt
 * with help from Tim Welge and Rania Wittenberg
 * Thanks to Norman Hendrich for debugging
 *
 * Usage:
 * ------
 * 1. Declare objects for each pushbotton:
 *     KeyHandler pb1, pb2, ...
 * 2. Initialise each key within the setup() method and assign a pin:
 *     pb1.keyInit(4);
 *     pb2.keyInit(5);
 * 3. In an Interrupt-Routine (e.g. TC6_Handler() ) check for key action:
 *     if (pb1.checkKey()) doPb1();
 * 4. create a function void doPb1() for acting:
 *     if (pb1.validLow())  ...
 */

#ifndef KEY_HANDLER_CPP
#define KEY_HANDLER_CPP

#include "KeyHandler.h"

void KeyHandler::keyInit(int thePin) {
  // Initialize key and assign pin. Parameter: thePin = pin number (e.g. 4 )
  bounceTime = 32; // milliseconds
  pin = thePin;
  pinMode(pin, INPUT);
  validStatus = digitalRead(pin);
  actualStatus = validStatus;
  bouncing = bounceTime;
}

bool KeyHandler::checkKey () {
  // Check if key has been operated and bounce time exceeded. Parameters:
  // p = pointer variable of type Key (e.g. &pb1 )
  // if return-values is true, you can check whether pb1 is pushed or
  // released by evaluating pb.validStatus which is either LOW or HIGH.
  actualStatus = digitalRead(pin);
  if (actualStatus == validStatus) {      // nothing happened, do nothing
    return false;
  } else {                                // key has been pressed
    if (bouncing == 0) {                  // bounce time exceeded?
      bouncing = bounceTime;              // reset timer
      validStatus = actualStatus;         // accept as valid
      return true;
    } else {
      bouncing --;                        // decrement and check again later
      return false;
    };
  };
  return false;
}

bool KeyHandler::getValidStatus() {
  // return validStatus;
  return validStatus;
}
bool KeyHandler::validLow() {
  // returns true is validStatus which is LOW
  return getValidStatus() == LOW;
}
bool KeyHandler::validHigh() {
  // returns true is validStatus which is HIGH
  return getValidStatus() == HIGH;
}

#endif // KEY_HANDLER_CPP

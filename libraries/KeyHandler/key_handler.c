/*
 * Library KeyHandler, Version of 2015-04-25
 * Author: Michael Hufschmidt
 * with help from Tim Welge and Rania Wittenberg
 *
 * Usage:
 * ------
 * 1. Declare variables for each pushbotton:
 *     Key pb1. pb2, ...
 * 2. Initialise each key within the setup() method and assign a pin:
 *     keyInit(&pb1, 4);
 *     keyInit(&pb2, 5);
 * 3. In an Interrupt-Routine (e.g. TC6_Handler() ) check for key action:
 *     if (checkKey(&Pb1)) doPb1();
 * 4. create a function void doPb1() for acting:
 *     if  if (pb1.validStatus == LOW)  ...
 */

#ifndef KEY_HANDLER_C
#define KEY_HANDLER_C

#include "key_handler.h"

void keyInit(void * p, int pin) {
// Initialize key and assign pin.  Parameters:
// p = pointer variable of type Key (e.g. &pb1 )
// pin = pin number (e.g. 4 )

  pKey pk = (pKey) p;
  pk->pin = pin;
  pinMode(pk->pin, INPUT);
  pk->validStatus = digitalRead(pk->pin);
  pk->actualStatus = pk->validStatus;
  pk->bouncing = bounceTime;
}

bool checkKey (void * p) {
// Check if key has been operated and bounce time exceeded. Parameters:
// p = pointer variable of type Key (e.g. &pb1 )
// if return-values is true, you can check whether pb1 is pushed or
// released by evaluating pb.validStatus which is either LOW or HIGH.
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

#endif // KEY_HANDLER_C

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

#ifndef KEY_HANDLER_H_
#define KEY_HANDLER_H

#include <Arduino.h>

const uint32_t bounceTime = 32; // bounce time in milli-seconds

typedef struct {
  int pin;
  bool validStatus;
  bool actualStatus;
  uint32_t bouncing;
} Key;
typedef Key * pKey;


void keyInit(void * p, int pin);
// Initialize key and assign pin.  Parameters:
// p = pointer variable of type Key (e.g. &pb1 )
// pin = pin number (e.g. 4 )

bool checkKey (void * p);
// Check if key has been operated and bounce time exceeded. Parameters:
// p = pointer variable of type Key (e.g. &pb1 )
// if return-values is true, you can check whether pb1 is pushed or
// released by evaluating pb.validStatus which is either LOW or HIGH.

#endif // KEY_HANDLER_H

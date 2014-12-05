/*
 * servo.c
 * @author mlw2224
 *
 * The functionality for the Servo.
 */

#include "servo.h"

int positionToPWM[6] = {5, 9, 13, 17, 21, 25};

void mov(Servo *servo, int parameter) {
	if (parameter >= 0 && parameter <= 5) { /* Valid input */
		servo->position = parameter;
    * servo->pwmDuty = positionToPWM[parameter];
		/* Increment the PC */
	}
}

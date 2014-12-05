/*
 * servo.h
 * @author mlw2224
 *
 * Header file for the Servos.
 */
#ifndef SERVO_H
#define SERVO_H

typedef struct {
  int   id;
	int		position;	/* Current Servo Position */
	unsigned char *   pwmDuty;
	unsigned char *   pwmPer;
	unsigned char *   leds;
} Servo;

/* 
 * @param parameter The target position number
 *
 * An out of range parameter value produces an error.
 */
void mov(Servo *servo, int parameter);

#endif

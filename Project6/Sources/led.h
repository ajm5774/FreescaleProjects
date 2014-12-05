#ifndef SERVO_H
#define SERVO_H

// LED masks
#define LED_3_OFF (1<<7)
#define LED_2_OFF (1<<6)
#define LED_1_OFF (1<<5)
#define LED_0_OFF (1<<4)

#define LED_3_ON (0<<7 | LED_2_OFF | LED_1_OFF | LED_0_OFF)
#define LED_2_ON (0<<6 | LED_3_OFF | LED_1_OFF | LED_0_OFF)
#define LED_1_ON (0<<5 | LED_3_OFF | LED_2_OFF | LED_0_OFF)
#define LED_0_ON (0<<4 | LED_3_OFF | LED_2_OFF | LED_1_OFF)

#endif
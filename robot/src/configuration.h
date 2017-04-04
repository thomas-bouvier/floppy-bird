#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define PRESS_DELAY 100000			// delay in uS
#define REST_DELAY 20000 	//delay in uS
#define STYLUS_CLICK_POSITION 73
#define STYLUS_REST_POSITION 90
#define PWM_PIN 1


#define PWM_RANGE 2000
#define PWM_CLOCK 192
#define MIN_PWM_PULSE_DURATION 80
#define MAX_PWM_PULSE_DURATION 200

#define NB_PIXEL_THRESHOLD 10
#define BIRD_CIRCLE_DIAMETER 15

#define CAPTURE_WIDTH 960/2		// Must be a multiple of 32
#define CAPTURE_HEIGHT 720/2	// Must be a multiple of 32


#endif //CONFIGURATION_H

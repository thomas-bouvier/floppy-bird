/*!
* \file configuration.h
* \brief File containing constants
*/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define PRESS_DELAY 100000			/*delay in uS*/
#define REST_DELAY 20000 			/*delay in uS*/
#define STYLUS_CLICK_POSITION 73
#define STYLUS_REST_POSITION 90
#define PWM_PIN 1


#define PWM_RANGE 2000
#define PWM_CLOCK 192
#define MIN_PWM_PULSE_DURATION 80
#define MAX_PWM_PULSE_DURATION 200

#define NB_PIXEL_THRESHOLD 10
#define THRESHOLD_NB_PIXELS_PIPE 200
#define BIRD_CIRCLE_DIAMETER 15

#define CAPTURE_WIDTH 	960/2//(32*10)//960/2 //256		/* Must be a multiple of 32 */
#define CAPTURE_HEIGHT 	720/2//(32*8)//720/2 //192		/* Must be a multiple of 32 */

#define WORKSPACE_DEF_COLOR CV_RGB(255, 0, 0)
#define DEFAULT_TRACKER_COLOR CV_RGB(255, 0, 0)

#define WIDTH_BIRD_TRACKING_ZONE 50			/* relative width = 1/7.56 */
#define RELATIVE_WIDTH_BIRD_TRACKING_ZONE (1/7.56)
#define WIDTH_PIPE_TRACKING_ZONE 115		/* relative width  = 1/2.52*/
#define RELATIVE_WIDTH_PIPE_TRACKING_ZONE (1/6.0)
typedef enum {false = 0,true = 1,False = 0,True = 1}boolean;

#define NB_PIPE_TRACKER 3

/* Tolerances for color analysis (HSV format)*/
#define Htolerance 5 //2
#define Stolerance 200//5

/* Maths methods */
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define sign(x) ((x) > 0 ? 1 : -1)



#endif /*CONFIGURATION_H*/

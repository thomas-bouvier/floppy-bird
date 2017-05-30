/*!
* \file configuration.h
* \brief File containing constants
*/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define PRESS_DELAY 100000			/*!< press delay in uS */
#define REST_DELAY 	30000 			/*!< rest delay in uS */
#define STYLUS_CLICK_POSITION 71	/*!< The servomotor position to have the stylus hitting the screen */
#define STYLUS_REST_POSITION 95		/*!< The servomotor rest position */
#define PWM_PIN 1		/*!< The GPIO pin connected to the servomotor signal */


#define PWM_RANGE 2000	/*!< The pwm range for the servo */
#define PWM_CLOCK 192	/*!< the pwm clock for the servo */
#define MIN_PWM_PULSE_DURATION 80	/*!< the pulse min duration for the servo */
#define MAX_PWM_PULSE_DURATION 200	/*!< the pulse max duration for the servo */

#define NB_PIXEL_THRESHOLD 10	/*!< The threshold from which a tracker detects something */
#define THRESHOLD_NB_PIXELS_PIPE 200	/*!< The threshold from which a tracker detects a pipe */
#define BIRD_CIRCLE_DIAMETER 15		/*!< the diameter for the bird tracker */

#define CAPTURE_WIDTH 	(32*10)		/*!< Capture width in pixels (must be a multiple of 64) */
#define CAPTURE_HEIGHT 	(32*8)	/*!< Capture height in pixels (must be a multiple of 64) */

#define WORKSPACE_DEF_COLOR CV_RGB(255, 0, 0)		/*!< The color of the rectangle used to define the workspace area */
#define DEFAULT_TRACKER_COLOR CV_RGB(255, 0, 0)		/*!< The default tracker color */

#define RELATIVE_WIDTH_BIRD_TRACKING_ZONE (1/7.56)		/*!< Relative width of the bird tracking zone */
#define RELATIVE_WIDTH_PIPE_TRACKING_ZONE (1/6.0)		/*!< Relative width of the pipe tracking zone */
#define NB_PIPE_TRACKER 3	/*!< number of pipe tracker (3 should be enough) */


/* Tolerances for color analysis (HSV format)*/
#define Htolerance 10 		/*!< Hue tolerance for color analysis */
#define Stolerance 150		/*!< Saturation tolerence for the color analysis */

/* Maths methods */
#define max(a, b) ((a) > (b) ? (a) : (b))		/*!< max macro definition */
#define min(a, b) ((a) < (b) ? (a) : (b))  		/*!< min macro definition */

typedef enum {false = 0,true = 1,False = 0,True = 1}boolean;	/*!< Boolean definition */


#endif /*CONFIGURATION_H*/

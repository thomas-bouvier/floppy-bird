/*!
* \file stylus.h
* \brief File containing structures and prototypes linked to the stylus
*/
#ifndef STYLUS_H
#define STYLUS_H

#include <time.h>
#include <sys/time.h>

/*!
* \struct Stylus 
* \brief A Stylus is actuated by a servomotor
*/
typedef struct {
	int pin;				/*!< physical pin for the servomotor */
	int clickPosition;		/*!< the position (in uS*10) of the servomotor to press the stylus on the screen */
	int restPosition;		/*!< the position (in uS*10) of the servomotor to put the stylus out of the screen */
	int currentPosition;	/*!< the current position (in uS*10) of the servomotor */
	long int pressDelay;			/*!< the delay needed (in uS) by the device to take the click into account */
	long int restDelay;			/*!< the delay needed (in uS) by the device between 2 clicks */
	int nbClick;			/*!< the number of click to act */
	struct timeval moveTime;		/*!< the time of the last move*/
} Stylus;


void attach(Stylus* stylus, int pin, int clickPosition, int restPosition, long int pressDelay, long int restDelay);
void update(Stylus* stylus);
void disable(Stylus* stylus);
void enable(Stylus* stylus);
void click(Stylus* stylus);
void flushClickStack(Stylus* stylus);
void moveStylus(Stylus* stylus, int targetPos);


#endif //STYLUS_H

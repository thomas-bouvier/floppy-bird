#include <unistd.h>
#include <stdio.h>
#include <wiringPi.h>
#include <sys/time.h>
#include "stylus.h"
#include "configuration.h"

static void moveStylus(Stylus* stylus, int targetPos);

/*!
* \brief Attach a stylus with its physical parameters
* \param[in] stylus : address of the stylus 
* \param[in] pin : physical pin for the servomotor
* \param[in] clickPosition : the position (in uS*10) of the servomotor to press the stylus on the screen
* \param[in] restPosition : the position (in uS*10) of the servomotor to put the stylus out of the screen
* \param[in] pressDelay : the delay needed by the device to take the click into account
* \param[in] restDelay : the delay needed etween 2 clicks
*/
void attach(Stylus* stylus, int pin, int clickPosition, int restPosition, long int pressDelay, long int restDelay)
{
	pinMode(pin,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(PWM_RANGE);
	pwmSetClock(PWM_CLOCK);
	stylus->pin = pin;
	stylus->clickPosition = clickPosition;
	stylus->restPosition = restPosition;
	stylus->pressDelay = pressDelay;
	stylus->restDelay = restDelay;
	stylus->currentPosition = 0;
	stylus->nbClick = 0;
	gettimeofday(&(stylus->moveTime),NULL);	// init of the current time
	disable(stylus);
}

/*!
* \brief Time managment function : Update the stylus position depending on the delays
* \param[in] stylus : address of the stylus 
*/
void update(Stylus* stylus)
{
	if(stylus->nbClick>=1){
		struct timeval currentTime;
		gettimeofday(&currentTime,NULL);
		if(stylus->currentPosition == stylus->restPosition){
			if((((currentTime.tv_sec - stylus->moveTime.tv_sec)*1000000+currentTime.tv_usec) - stylus->moveTime.tv_usec)>(stylus->restDelay)) {		// if rest delay past
				moveStylus(stylus,stylus->clickPosition);
				stylus->currentPosition = stylus->clickPosition;
				gettimeofday(&(stylus->moveTime),NULL);	// reset of the current time
			}
		} else if (stylus->currentPosition == stylus->clickPosition){
			if((((currentTime.tv_sec - stylus->moveTime.tv_sec)*1000000+currentTime.tv_usec) - stylus->moveTime.tv_usec)>(stylus->pressDelay)) {	// if press delay past
				moveStylus(stylus,stylus->restPosition);
				stylus->currentPosition = stylus->restPosition;
				stylus->nbClick--;
				gettimeofday(&(stylus->moveTime),NULL);	// reset of the current time
			}
		} else {		// default case
			stylus->currentPosition = stylus->restPosition;
			gettimeofday(&(stylus->moveTime),NULL);	// reset of the current time
		}
	}
}

/*!
* \brief disable the servomotor
* \param[in] stylus : address of the stylus 
*/
void disable(Stylus* stylus)
{
	pwmWrite(stylus->pin,0);	// Put the signal to the ground
}

/*!
* \brief enable the servomotor
* \param[in] stylus : address of the stylus 
*/
void enable(Stylus* stylus)
{
	moveStylus(stylus,stylus->restPosition);	// Put the stylus to the rest pos
	stylus->currentPosition = stylus->restPosition;		// Save the last position
	gettimeofday(&(stylus->moveTime),NULL);	// reset of the current time
}

/*!
* \brief Manage the click action
* \param[in] stylus : address of the stylus 
*/
void click(Stylus* stylus)
{
	stylus->nbClick++;
}

/*!
* \brief Update the stylus target position
* \param[in] stylus : address of the stylus 
* \param[in] targetPos : target position msut be included between MIN_PWM_PULSE_DURATION and MAX_PWM_PULSE_DURATION
*/
static void moveStylus(Stylus* stylus, int targetPos)
{
	if(targetPos>MAX_PWM_PULSE_DURATION)
		targetPos = MAX_PWM_PULSE_DURATION;
	else if (targetPos<MIN_PWM_PULSE_DURATION)
		targetPos = MIN_PWM_PULSE_DURATION;
	pwmWrite(stylus->pin,targetPos);
}








#include "stylus.h"


/*!
* \brief Attach a stylus with its physical parameters
* \param[in] address of the stylus 
* \param[in] physical pin for the servomotor
* \param[in] the position (in uS*10) of the servomotor to press the stylus on the screen
* \param[in] the position (in uS*10) of the servomotor to put the stylus out of the screen
* \param[in] the delay needed by the device to take the click into account
*/
void attach(Stylus* stylus, int pin, int clickPosition, int restPosition, int pressDelay)
{
	pinMode(pin,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(PWM_RANGE);
	pwmSetClock(PWM_CLOCK);
	stylus->pin = pin;
	stylus->clickPosition = clickPosition;
	stylus->restPosition = restPosition;
	stylus->pressDelay = pressDelay;
}

/*!
* \brief Time managment function : Update the stylus position depending on the delays
* \param[in] address of the stylus 
*/
void update(Stylus* stylus)
{
	
}

/*!
* \brief disable the servomotor
* \param[in] address of the stylus 
*/
void disable(Stylus* stylus)
{
	pwmWrite(pin,0);	// Put the signal to the ground
}

/*!
* \brief enable the servomotor
* \param[in] address of the stylus 
*/
void enable(Stylus* stylus)
{
	moveStylus(stylus,restPosition);	// Put the stylus to the rest pos
}

/*!
* \brief Manage the click action
* \param[in] address of the stylus 
*/
void click(Stylus* stylus)
{
	
}

/*!
* \brief Update the stylus target position
* \param[in] address of the stylus 
* \param[in] target position msut be included between MIN_PWM_PULSE_DURATION and MAX_PWM_PULSE_DURATION
*/
static void moveStylus(Stylus* stylus, int targetPos)
{
	if(targetPos>MAX_PWM_PULSE_DURATION)
		targetPos = MAX_PWM_PULSE_DURATION;
	else if (targetPosition<MIN_PWM_PULSE_DURATION)
		targetPos = MIN_PWM_PULSE_DURATION;
	pwmWrite(stylus->pin,targetPos);
}


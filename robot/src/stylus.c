#include "stylus.h"


/*!
* \brief Attach a stylus with its physical parameters
* \param[in] address of the stylus 
* \param[in] physical pin for the servomotor
* \param[in] the position (in uS*10) of the servomotor to press the stylus on the screen
* \param[in] the position (in uS*10) of the servomotor to put the stylus out of the screen
* \param[in] the delay needed by the device to take the click into account
* \return Return 1 if everything is ok, 0 if error
*/
int attach(Stylus* stylus, int pin, int clickPosition, int restPosition, int pressDelay)
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
	pwmWrite(pin,restPosition);	// Put the stylus to the rest pos
}

/*!
* \brief Manage the click action
* \param[in] address of the stylus 
*/
void click(Stylus* stylus)
{
	
}


/*
char key;
	
	
	while(1) {
		if(scanf("%c",&key)){
			pwmWrite(1,140);// Stylus touching screen
			delay(PRESS_DELAY);
			pwmWrite(1,155);// Stylus out of the screen
		}
	}*/

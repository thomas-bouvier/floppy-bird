#include "stylus.h"


/*!
* \brief Attach a stylus with its physical parameters
* \param[in] physical pin for the servomotor
* \param[in] the position (in uS*10) of the servomotor to press the stylus on the screen
* \param[in] the position (in uS*10) of the servomotor to put the stylus out of the screen
* \param[in] the delay needed by the device to take the click into account
* \return Return 1 if everything is ok, 0 if error
*/
int attach(int pin, int clickPosition, int restPosition, int pressDelay)
{
	
}

/*!
* \brief Time managment function : Update the stylus position depending on the delays
*/
void update()
{
	
}

/*!
* \brief disable the servomotor
*/
void disable()
{
	
}

/*!
* \brief enable the servomotor
*/
void enable()
{
	
}

/*!
* \brief Manage the click action
*/
void click()
{
	
}


/*
char key;
	wiringPiSetup();
	pinMode(1,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(2000);
	pwmSetClock(192);
	
	while(1) {
		if(scanf("%c",&key)){
			pwmWrite(1,140);// Stylus touching screen
			delay(PRESS_DELAY);
			pwmWrite(1,155);// Stylus out of the screen
		}
	}*/

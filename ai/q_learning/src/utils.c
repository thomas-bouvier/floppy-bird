#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/*!
* \brief Return a random number between 0 and the specified parameter
* \param[in] limit the upper limit
* \return Return a random number in the interval [0; limit]
*/
int randomAtMost(int limit) {
  	int r;
  	int d;
 
  	if (limit == 0) return 0;

  	d = RAND_MAX / limit;
  	limit *= d;

  	do {
    	r = rand();
  	} while (r >= limit);

  	return r / d;
}

/*!
* \brief Give the value raised at the power
* \param[in] value value to raise at
* \param[in] power power
* \return Return the value raised at the power given
*/
float powerOf(float value, int power)
{
	float r=1;
	int i;
	for(i=0; i<power; ++i) r *= value;
	return r;
}

/*!
* \brief Shift an array to the right
* \param[in] array array to be shifted
* \param[in] size size of the array
*/
void shift_array(int * array, int size)
{
	int i;
	for(i=size-1; i>0; --i) array[i]=array[i-1];
}

/*!
* \brief Initialize an array at the given value
* \param[in] array array to be initialized
* \param[in] size size of the array
* \param[in] value initialization value
*/
void init_array(int * array, int size, int value)
{
	int i;
	for(i=size-1; i>=0; --i) array[i]=value;
}

/*!
* \brief Wait during the given value in ms
* \param[in] mseconds waiting duration
*/
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

/*!
* \brief Clear the terminal screen (for debug purpose)
*/
void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

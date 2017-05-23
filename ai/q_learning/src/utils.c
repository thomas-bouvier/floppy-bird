#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

/*!
* \brief Return a random number in a given range
* \param[in] limit_min the lower limit
* \param[in] limit_max the upper limit
* \return Return a random number in the interval [limit_min; limit_max], 0 and message if error occurred
*/
unsigned int randomInRange(unsigned int limit_min, unsigned int limit_max) {
  	
	if((limit_min<0) || (limit_max<limit_min) || (limit_max>UINT_MAX))
	{
		printf("Error due to the random function in the qlearning/utils.c file.\n");
		return 0;
	}

	unsigned int range= 1 + limit_max - limit_min;
  	unsigned int d=RAND_MAX / range;
    unsigned int limit= range*d;
    unsigned int r;

    if (limit_max == 0) return 0;

  	do {
    	r = rand();
  	} while (r >= limit);
    
  	return limit_min+(r/d);
}

/*!
* \brief Give the value raised at the given power
* \param[in] value value to raise at
* \param[in] power power
* \return Return the value raised at the given power
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
void rightShift_array(int * array, int size)
{
	int i;
	for(i=size-1; i>0; --i) array[i]=array[i-1];
	array[0]=0;
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

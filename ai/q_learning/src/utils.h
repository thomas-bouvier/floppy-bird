/*!
* \file utils.h
* \brief File containing utility functions
*/
#ifndef UTILS_H
#define UTILS_H

unsigned int randomInRange(unsigned int limit_min, unsigned int limit_max);
float powerOf(float value, int power);
void rightShift_array(int * array, int size);
void init_array(int * array, int size, int value);

#endif

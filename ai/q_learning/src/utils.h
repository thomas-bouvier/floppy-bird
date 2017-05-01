/*!
* \file utils.h
* \brief File containing utility functions
*/
#ifndef UTILS_H
#define UTILS_H

int randomAtMost(int limit);
int randomInRange(int limit_min, int limit_max);
float powerOf(float value, int power);
void shift_array(int * array, int size);
void init_array(int * array, int size, int value);
/*void delay(unsigned int mseconds);
void clearScreen();*/

#endif

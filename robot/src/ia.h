/*!
* \file ia.h
* \brief File containing structures and prototypes of functions to make the IA integration easier
*/
#ifndef IA_H
#define IA_H

#include "stylus.h"
#include "tracking.h"
#include "pipeTracking.h"

/*!
* \struct Robot 
* \brief struct gathering all the necessary to interface the IA with the robot
*/
struct Robot{
	TrackedObject* birdTracker;		/*!<  the bird tracker*/
	PipeDynamicTracker* pipeDynTracker;	/*!<  the pipe tracker*/
	Stylus* stylus;		/*!< The stylus actuated by the servo */
};

float getNextPipePosition(Robot* robot);
float getNextPipeHeight(Robot* robot);
float getBirdHeight(Robot* robot);
int getGameStatus(Robot* robot);
void jump(Robot* robot);

#endif /* IA_H */

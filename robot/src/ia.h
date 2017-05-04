/*!
* \file ia.h
* \brief File containing structures and prototypes of functions to make the IA integration easier
*/
#ifndef IA_H
#define IA_H

#include "stylus.h"
#include "tracking.h"
#include "pipeTracking.h"
#include <pthread.h>

/*!
* \struct Robot 
* \brief struct gathering all the necessary to interface the IA with the robot
*/
struct Robot{
	pthread_mutex_t mutex_robot;	/*!< the mutex to avoid threads writing conflicts */
	float birdHeight;		/*!<  the bird height*/
	float pipeHeight; 		/*!<  the pipe height*/
	float pipeBirdDist;		/*!<  the pipe - bird distance*/
	int gameStatus;			/*!< the status of the game : 1 if the game is running, 0 if the bird is dead */
	Stylus* stylus;		/*!< The stylus actuated by the servo */
};

/* init */
void initRobot(Robot* robot, Stylus* stylus);

/* getter / setter */
float getNextPipePosition(Robot* robot);
void setNextPipePosition(Robot* robot, float value);
float getNextPipeHeight(Robot* robot);
void setNextPipeHeight(Robot* robot, float value);
float getBirdHeight(Robot* robot);
void setBirdHeight(Robot* robot, float value);
int getGameStatus(Robot* robot);
void setGameStatus(Robot* robot, int status);

/* actions */
void jump(Robot* robot);

#endif /* IA_H */
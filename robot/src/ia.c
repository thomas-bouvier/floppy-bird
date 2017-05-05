#include "ia.h"

/*!
* \brief the main function for the IA
* \param[in] robot : the robot interface
*/
void* mainIa (void* robot)
{
	robot = (Robot*) robot;
	return NULL;
}

/*!
* \brief init the robot struct
* \param[in] robot : the robot interface
* \param[in] stylus : the address of the stylus
*/
void initRobot(Robot* robot, Stylus* stylus)
{
	pthread_mutex_init ( &robot->mutex_robot, NULL);
	setBirdHeight(robot,-1);
	setNextPipeHeight(robot,-1);
	setNextPipePosition(robot,-1);
	setGameStatus(robot,0);
	robot->stylus = stylus;
}

/*!
* \brief get the next pipe relative position with the bird
* \param[in] robot : the robot interface
* \return a float positive value corresponding to the next pipe relative position with the bird
*/
float getNextPipePosition(Robot* robot)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	float value = robot->pipeBirdDist;
	pthread_mutex_unlock (&(robot->mutex_robot));
	return value;
}

/*!
* \brief set the next pipe relative position with the bird
* \param[in] robot : the robot interface
* \param[in] value : the new next pipe position
*/
void setNextPipePosition(Robot* robot, float value)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	robot->pipeBirdDist = value;
	pthread_mutex_unlock (&(robot->mutex_robot));
}

/*!
* \brief get the next pipe relative height
* \param[in] robot : the robot interface
* \return a float positive value corresponding to the next pipe relative height (relatif to the screen height)
*/
float getNextPipeHeight(Robot* robot)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	float value = robot->pipeHeight;
	pthread_mutex_unlock (&(robot->mutex_robot));
	return value;
}

/*!
* \brief set the next pipe relative height
* \param[in] robot : the robot interface
* \param[in] value : the new next pipe height
*/
void setNextPipeHeight(Robot* robot, float value)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	robot->pipeHeight = value;
	pthread_mutex_unlock (&(robot->mutex_robot));
}

/*!
* \brief get the bird relative height
* \param[in] robot : the robot interface
* \return a float positive value corresponding to the bird relative height (relatif to the screen height)
*/
float getBirdHeight(Robot* robot)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	float value = robot->birdHeight;
	pthread_mutex_unlock (&(robot->mutex_robot));
	return value;
}

/*!
* \brief set the bird relative height
* \param[in] robot : the robot interface
* \param[in] value : the new bird height
*/
void setBirdHeight(Robot* robot, float value)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	robot->birdHeight = value;
	pthread_mutex_unlock (&(robot->mutex_robot));
}

/*!
* \brief get the game status
* \param[in] robot : the robot interface
* \return 1 if the game is running, 0 if the bird is dead
*/
int getGameStatus(Robot* robot)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	int status = robot->gameStatus;
	pthread_mutex_unlock (&(robot->mutex_robot));
	return status;
}

/*!
* \brief set the game status
* \param[in] robot : the robot interface
* \param[in] status : the new game status
*/
void setGameStatus(Robot* robot, int status)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	robot->gameStatus = status;
	pthread_mutex_unlock (&(robot->mutex_robot));
}

/*!
* \brief click with the stylus to make the bird jump
* \param[in] robot : the robot interface
*/
void jump(Robot* robot)
{
	click(robot->stylus);
}

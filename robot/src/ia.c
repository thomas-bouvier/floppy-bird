#include "ia.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define JUMP_OFFSET 0.3
#define JUMP_HEIGHT 0.5
#define SENSIBILITY 0.9

/*!
* \brief the main function for the IA
* \param[in] robot : the robot interface
*/
void* mainIa (void* bot)
{
	Robot* robot = (Robot*)malloc(sizeof(Robot));
	robot = (Robot*)bot;



    /* Press Play *//*1 clk 120us*/
    sleep(1);
    jump(robot);
    while(!getDataUpdated(robot));
    setDataUpdated(robot, 0);  
    jump(robot);
    jump(robot);
    sleep(1);
    

    while(1)
    {
        if(getDataUpdated(robot))
        {
			setDataUpdated(robot, 0); 
			float birdHeight = getBirdHeight(robot);
			float pipeHeight = getNextPipeHeight(robot) + JUMP_OFFSET;
			if(!jumpRunning(robot) && (birdHeight < pipeHeight)){
				float n = ((pipeHeight - birdHeight)/JUMP_HEIGHT) + SENSIBILITY ;
				int nbJump = (int)n;
				int i;
				printf("run %f (%d) jumps ; jump running : %s\n",n,nbJump,jumpRunning(robot)? "yes" : "no"); 
				for (i = 0; i<nbJump;i++){
					jump(robot);
				}
			}

        }     
       
        if(getGameStatus(robot)){
			cancelAllJump(robot);
			printf("Mort\n");
			sleep(2);
			jump(robot);
			sleep(1);
			jump(robot);
			sleep(1);
			jump(robot);
			jump(robot);
			sleep(1);
		}
    }

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
* \return 0 if the game is running, 1 if the bird is dead
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
* \brief tells if data has been updated or not
* \param[in] robot : the robot interface
* \return 1 if the data updated, 0 otherwise
*/
boolean getDataUpdated(Robot* robot)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	boolean value = robot->dataUpdated;
	pthread_mutex_unlock (&(robot->mutex_robot));
	return value;
}

/*!
* \brief set the var dataUpdated
* \param[in] robot : the robot interface
* \param[in] value : the new value for dataUpdated
*/
void setDataUpdated(Robot* robot, boolean value)
{
	pthread_mutex_lock (&(robot->mutex_robot));
	robot->dataUpdated = value;
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

/*!
* \brief cancel the next Jump (put the nbClick to 0)
* \param[in] robot : the robot interface
*/
void cancelAllJump(Robot* robot)
{
	flushClickStack(robot->stylus);
	moveStylus(robot->stylus,robot->stylus->restPosition);
}

/*!
* \brief test if a jump is running
* \param[in] robot : the robot interface
* \return 1 if a jump is running, 0 otherwise
*/
boolean jumpRunning(Robot* robot)
{
	return getNbClick(robot->stylus) > 0;
}

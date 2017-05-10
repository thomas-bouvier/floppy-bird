#include "ia.h"
#include <unistd.h>
#include "../../ai/q_learning/src/q_learning.h"
#include "../../ai/q_learning/src/game_state.h"
#include <time.h>
/*!
* \brief the main function for the IA
* \param[in] robot : the robot interface
*/
void* mainIa (void* robot)
{
	robot = (Robot*) robot;

    MatrixQ * matrixQ = NULL;
    int last_states[NB_SAVED_STATES];
    int last_action[NB_SAVED_ACTIONS];
    char *qmatrixPath="../res/qmatrix_data_robot.txt";
    int hit_saved=0;
    int action_break=1;
    srand(time(NULL));
    matrixQ = loadQMatrix(qmatrixPath);

    init_array(last_states, NB_SAVED_STATES, -1);
    init_array(last_action, NB_SAVED_ACTIONS, -1);
    
    int complement=0;
    long int time=0;

    /* Press Play *//*1 clk 120us*/
    sleep(1);
    jump(robot);
    jump(robot);
    sleep(1);

    while(1)
    {
		//printf("hit_saved: %d\n", hit_saved);
		
        if(action_break == 1 || hit_saved == 1)
        {
            action_break=0;
            q_learning_loop(matrixQ, last_states, last_action, 10*getNextPipePosition(robot), 10*getNextPipeHeight(robot)-10*getBirdHeight(robot), 10*getNextPipeHeight(robot), hit_saved);

            if(last_action[0] != -1)
            {	
				printf("Saut\n");
                if(last_action[0]) jump(robot);
            }
            if(hit_saved)
            {
				cancelAllJump(robot);
				saveQMatrix(matrixQ, qmatrixPath);
				printf("Mort\n");
				sleep(2);
                jump(robot);
                sleep(1);
                jump(robot);
                sleep(1);
                jump(robot);
                action_break = 1; /* Test random */
            }
        }   
        if(/*getDataUpdated(robot) && */time+500000<clock())
        {
			time=clock();
			printf("%li\n", time);
			/*if(complement) complement=0;
			else complement=1;
			
			if(complement)
			{*/
				printf("DataUpdated\n");
				action_break=1;
			/*}*/
            setDataUpdated(robot, 0);        
        }
        //printf("getUpdated: %d\n", getDataUpdated(robot));       
        hit_saved = getGameStatus(robot);
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

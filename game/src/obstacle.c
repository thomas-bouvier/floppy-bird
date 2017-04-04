/*!
* \file obstacle.c
* \brief File containing the functions to manage an obstacle
*/
#include "obstacle.h"

/*!
*\brief Create an obstacle
* \param[in] number the obstacle number
* \param[in] height_lower the ordinate of the lower pipe (must be between MIN_HEIGHT_LOWER and MAX_HEIGHT_LOWER)
* \param[in] obstacle_gap the gap between two pipes (must not be lower than OBSTACLE_GAP)
* \param[in] next_obstacle the next element of the current obstacle
* \return Return the created obstacle, NULL if error or incorrect value
*/
Obstacle * newObstacle(int number, int height_lower, int obstacle_gap, Obstacle * next_obstacle)
{
    if((height_lower < MIN_HEIGHT_LOWER) || (height_lower > MAX_HEIGHT_LOWER))
    {
    	fprintf(stderr, "Obstacle created has a wrong value of height_lower\n");
        return NULL;
    }
    if(obstacle_gap < OBSTACLE_GAP)
    {
    	fprintf(stderr, "Obstacle created has a wrong value of gap\n");
        return NULL;
    }
    Obstacle * new_obstacle = (Obstacle*) malloc(sizeof(Obstacle));
    if(new_obstacle == NULL)
    {
        fprintf(stderr, "Obstacle allocation problem");
        return NULL;
    }
    initPipe(&new_obstacle->lower, number, SCREEN_HEIGHT - height_lower, height_lower);             /* Lower pipe */
    initPipe(&new_obstacle->upper, number, 0, SCREEN_HEIGHT - (height_lower + obstacle_gap));       /* Upper pipe */
    new_obstacle->gap = obstacle_gap;
    new_obstacle->next = next_obstacle;
    return new_obstacle;
}

/*!
* \brief Indicate the next obstacle for the bird
* \param[out] l the list of obstacle
* \param[in] bird the bird that determines the next obstacle
* \return Return the next obstacle for the bird. If the bird is between two pipes of an obstacle, it returns this obstacle
*/
Obstacle * nextBirdObstacle(List * l, Bird * bird)
{
    int i = 0;
    setOnFirst(l);
    while (i < PIPES_ON_SCREEN)
    {
        if (l->current->lower.x + PIPE_WIDTH > bird->x - bird->w/2)
            return l->current;
        next(l);
        ++i;
    }
    return NULL;
}

/*!
* \brief Deallocate memory of the obstacle and his pipes
* \param[out] obstacle the obstacle to deallocate
*/
void freeObstacle(Obstacle * obstacle)
{
    free(obstacle);
}

/*!
* \brief Create an obstacle with the height indicated in the level file for its number
* \param[in] level the file that contains the height of the obstacles
* \param[in] number the obstacle number
* \param[out] l the list of obstacles
*/
void createObstacleFromFile(FILE * level, int number, List * l)
{
    int heightPipe = readLevel(level, number);
    Obstacle * newObs = newObstacle(number, heightPipe, OBSTACLE_GAP, NULL);
    insertLast(l, newObs);
}

/*!
* \brief Create an obstacle with an random height
* \param[in] number the obstacle number
* \param[out] l the list of obstacles
*/
void createObstacleRandomly(int number, List * l)
{
    int heightPipe = rand() % (MAX_HEIGHT_LOWER - MIN_HEIGHT_LOWER + 1) + MIN_HEIGHT_LOWER;
    Obstacle * newObs = newObstacle(number, heightPipe, OBSTACLE_GAP, NULL);
    insertLast(l, newObs);
}
/*!
* \brief Indicate if the bird passed the next obstacle
* \param[in] bird the bird that determines the next obstacle
* \param[in] savedObstacle the obstacle saved previously
* \return Return 1 when the bird passed the obstacle (ie if the left side of the bird exceed the right side of the obstacle), 0 else
*/
int obstaclePassed(Bird * bird, Obstacle * savedObstacle)
{
    if(bird->x - bird->w/3 > savedObstacle->lower.x + PIPE_WIDTH){
        return 1;
    }
    return 0;
}

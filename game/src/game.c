/*!
* \file game.c
* \brief File containing the functions to manage the game
*/
#include "game.h"

/*!
* \brief Allocate all the object of the game
* \param[out] bird the bird to allocate
* \param[out] camera the camera to allocate
* \param[out] l the list of obstacles
* \param[in] level the file that contains the height of the obstacles
*/
void startGame(Bird * bird, Camera * camera, List * l, FILE * level)
{
    initBird(bird);
    initCamera(camera, 0, CAMERA_SPEED);
    initList(l, level);
}

/*!
* \brief Allow to scroll the camera in the right direction
* \param[out] camera the camera that follows the bird
* \param[out] bird the bird that moves with the camera
*/
void cameraScrolling(Camera * camera, Bird * bird)
{
    camera->x += camera->speed;
    bird->x += camera->speed;
}

/*!
* \brief Create an obstacle when the distance between two obstacle is reached
* \param[in] camera the view of the game
* \param[out] l the list of obstacles
* \param[in] level the file that contains the height of the obstacles
* \param[in] number the obstacle number of the new obstacle
* \return Return 1 if the function created an obstacle, 0 otherwise
*/
int createObstacle(Camera * camera, List * l, FILE * level, int number)
{
    if ((camera->x + SCREEN_WIDTH >= l->first->lower.x) && (l->nbObstacles < OBSTACLE_NUMBER))
    {
        createObstacleFromFile(level, number, l);
        return 1;
    }
    return 0;
}

/*!
* \brief Delete an obstacle of the list when it leaves the camera
* \param[in] camera the view of the game
* \param[out] l the list of obstacles
* \return Return 1 if the function deleted an obstacle, 0 otherwise
*/
int deleteObstacle(Camera * camera, List * l){
    if ((l->first->lower.x + PIPE_WIDTH) < camera->x)
    {
        deleteFirst(l);
        return 1;
    }
    return 0;
}


/*!
* \brief Detect the collisions
* \param[in] bird the bird to test the collision with
* \param[in] obstacle the next obstacle, the only one that the bird can hit
* \return 1 if the bird hit something 0 in other cases
*
* Firstly test if the bottom limit of the bird (bird->y + bird->h/2) is lower than the limit of the screen (SCREEN_HEIGHT)
* then if the right limit of the bird is farther than the left limit of the next obstacle it text if the bird is'nt beyond the gap
*/
int detectHit(Bird * bird, Obstacle * obstacle, Sound * sound)
{
    int h = 0;
    if(bird->y + bird->h/2 >= SCREEN_HEIGHT)
    {
        h = 1;              /* Collision with the ground */
        (*sound) = DEATH;
    }
    if(bird->x + bird->w/2 >= obstacle->lower.x)
    {
        if(bird->y - bird->h/2 <= obstacle->upper.y + obstacle->upper.h)
        {
            h = 1;          /* Collision with the upper pipe */
        	(*sound) = DEATH;
        }
        if(bird->y + bird->h/2 >= obstacle->lower.y)
        {
            h = 1;          /* Collision with the lower pipe */
            (*sound) = DEATH;
        }
    }
    return h;
}

/*!
* \brief The function update the score if an obstacle is passed
* \param[in] score the current score
* \param[in] bird the bird that determines the next obstacle
* \param[out] savedObstacle the obstacle saved previously
* \return return the updated score
*/
int updateScore(int score, Bird * bird, Obstacle * savedObstacle, Sound * sound)
{
    if (obstaclePassed(bird, savedObstacle, sound))
    {
        score++;
        return score;
    }
    return score;
}

/*!
* \brief The function called every frame of the running game to update all objects
* \param[out] bird the bird linked to the running game
* \param[out] camera the camera linked to the running game
* \param[out] l the list of obstacles linked to the running game
* \param[in] level the file that contains the height of the obstacles
* \param[in] event the value that indicate if the bird must jump(1) or not(0 or 2)
* \param[in] number the obstacle number of the new obstacle
* \param[in] savedObstacle the obstacle saved to compare with the next obstacle
* \param[out] score the score of the player to be updated
* \return 1 in case of game over, 0 in the others cases
*/
int game(Bird * bird, Camera * camera, List * l, FILE * level, int event, int * number, Obstacle * savedObstacle, int * score, Sound * sound)
{
    updateBird(bird, event, sound);
    deleteObstacle(camera, l);
    if (createObstacle(camera, l, level, *number))
        (*number)++;
    *score = updateScore(*score, bird, savedObstacle, sound);
    cameraScrolling(camera, bird);
    return detectHit(bird, nextBirdObstacle(l, bird), sound);
}

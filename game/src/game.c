/*!
* \file game.c
* \brief File containing the functions to manage the game
*/
#include "game.h"
/*!
* \brief Allocate all the object of the game
* \param[out] bird the bird to allocate
* \param[out] camera the camera to allocate
* \param[out] obstacle[] the table that contains the obstacles
*/
void initGame(Bird * bird, Camera * camera, Obstacle * obstacle[])
{
    int i;
    bird = newBird();
    camera = newCamera(0,1);
    for(i=0; i<PIPES_ON_SCREEN; ++i)
    {
        initObstacle(obstacle[i], 0, 0, 0);
    }
}

/*!
* \brief Allow to scroll the camera in the right direction
* \param[out] pipe the pipe to scroll
* \param[out] bird the bird that moves with the camera
*/
void cameraScrolling(Camera * camera, Bird * bird)
{
    camera->x += camera->speed;
    bird->x += camera->speed;
}

/*!
* \brief Deallocate an obstacle when it leaves the screen by the left, and create a new obstacle on the right side of the screen. Update the table of obstacle
* \param[out] camera the view of the game
* \param[out] obstacle the table of obstacle that appears on the screen
* \param[in] number the obstacle number of the new obstacle
* \param[in] height_lower the ordinate of the lower pipe of the new obstacle
* \param[in] obstacle_gap the gap between two pipes of the new obstacle
*/
int obstacleCreation(Camera * camera, Obstacle * obstacle[], int number, int height_lower, int obstacle_gap)
{
    if ((obstacle[0]->lower.x + PIPE_WIDTH) < camera->x)
    {
        freeObstacle(obstacle);
        int i;
        for (i=0 ; i<PIPES_ON_SCREEN - 1 ; ++i)
        {
            obstacle[i] = obstacle[i + 1];
        }
        initObstacle(obstacle[PIPES_ON_SCREEN - 1], number, height_lower, obstacle_gap);
    }
    return 1;
}

/*!
* \brief Detect the collisions
* \param[in] bird the bird to test the collision with
* \param[in] obstacle the only obstacle that the bird can hit
* \return 1 if the bird hit something 0 in other cases
*/
int detectHit(Bird * bird, Obstacle * obstacle)
{
    int h = 0;
    if(bird->y + bird->h/2 >= SCREEN_HEIGHT)
        h = 1;              //detect the collision with the ground
    if(bird->x + bird->w/2 >= obstacle->lower.x)
    {
        if(bird->y - bird->h/2 <= obstacle->upper.y + obstacle->upper.h)
            h = 1;          //collision with the upper pipe
        if(bird->y + bird->h/2 >= obstacle->lower.y)
            h = 1;          //collision with the lower pipe
    }
    return h;
}
/*!
* \brief The function called every frame of the running game to update all objects
* \param[out] bird the bird linked to the running game
* \param[out] camera the camera linked to the running game
* \param[out] obstacle[] the table of obstacles linked to the running game
* \param[in] event the value that indicate if the bird must jump(1) or not(0 or 2)
* \param[in] heightPipe the ordinate of the lower pipe of the new obstacle created
* \param[in] number the obstacle number of the new obstacle
* \return 1 in case of game over, 0 in the others cases
*/
int game(Bird * bird, Camera * camera, Obstacle obstacle[],int event,int heightPipe, int number)
{
    updateBird(bird, event);
    obstacleCreation(camera,obstacle,number,heightPipe ,100);
    cameraScrolling(camera, bird);
    return detectHit(bird, nextObstacle(obstacle, bird));
}


void freeAll(Bird * bird, Obstacle obstacle[], Camera * camera)
{
    int i;
    freeBird(bird);
    freeCamera(camera);
    for (i=0 ; i<PIPES_ON_SCREEN ; ++i)
        freeObstacle(&obstacle[i]);
}


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
*/
void startGame(Bird * bird, Camera * camera, List * l)
{
    initBird(bird);
    initCamera(camera, 0, CAMERA_SPEED);
    initList(l);
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
* \param[in] number the obstacle number of the new obstacle
* \param[in] height_lower the ordinate of the lower pipe of the new obstacle
* \param[in] obstacle_gap the gap between two pipes of the new obstacle
*/
int createObstacle(Camera * camera, List * l, int number, int height_lower, int obstacle_gap)
{
    if (camera->x + SCREEN_WIDTH >= l->first->lower.x)
    {
        insertLast(l, newObstacle(number, height_lower, obstacle_gap, NULL));
        return 1;
    }
    return 0;
}

/*!
* \brief Delete an obstacle of the list when it leaves the camera
* \param[in] camera the view of the game
* \param[out] l the list of obstacles
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
int detectHit(Bird * bird, Obstacle * obstacle)
{
    int h = 0;
    if(bird->y + bird->h/2 >= SCREEN_HEIGHT)
        h = 1;              /* Collision with the ground */
    if(bird->x + bird->w/2 >= obstacle->lower.x)
    {
        if(bird->y - bird->h/2 <= obstacle->upper.y + obstacle->upper.h)
            h = 1;          /* Collision with the upper pipe */
        if(bird->y + bird->h/2 >= obstacle->lower.y)
            h = 1;          /* Collision with the lower pipe */
    }
    return h;
}
/*!
* \brief The function called every frame of the running game to update all objects
* \param[out] bird the bird linked to the running game
* \param[out] camera the camera linked to the running game
* \param[out] l the list of obstacles linked to the running game
* \param[in] event the value that indicate if the bird must jump(1) or not(0 or 2)
* \param[in] heightPipe the ordinate of the lower pipe of the new obstacle created
* \param[in] number the obstacle number of the new obstacle
* \return 1 in case of game over, 0 in the others cases
*/
int game(Bird * bird, Camera * camera, List * l, int event,int heightPipe, int number)
{
    updateBird(bird, event);
    deleteObstacle(camera, l);
    createObstacle(camera, l, number, heightPipe , 200);
    cameraScrolling(camera, bird);
    return detectHit(bird, nextBirdObstacle(l, bird));
}

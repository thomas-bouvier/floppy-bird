/*!
* \file game.c
* \brief File containing the functions to manage the game
*/
#include "game.h"

/*!
* \brief Allocate all the object of the game
* \param[out] bird the list of bird to allocate
* \param[out] camera the camera to allocate
* \param[out] l the list of obstacles
* \param[in] level the file that contains the height of the obstacles
* \param[in] levelFromFile 1 if the level is read from a file and 0 if the level is generate randomly
*/
void startGame(GenericList * bird, Camera * camera, List * l, FILE * level, int levelFromFile)
{
    Bird * new_bird = initBird();
    add(bird, new_bird);
    initCamera(camera, 0, LOW);
    initList(l, level, levelFromFile);
    setOnFirstElement(bird);
}

void startGameNeat(GenericList * birds, Camera * camera, List * l, FILE * level, int levelFromFile, MatingPool * pool)
{
    int i;
    Bird * bird = NULL;

    for (i = 0; i < pool->nb_species; ++i) {
        setOnFirstElement(pool->species[i].genomes);
        while (!outOfGenericList(pool->species[i].genomes)) {
            generateGenome(getCurrent(pool->species[i].genomes));

            if ((bird = newBird(r, getCurrent(pool->species[i].genomes))) == NULL)
                return;

            add(birds, bird);

            nextElement(pool->species[i].genomes);
        }
    }

    initCamera(camera, 0, LOW);
    initList(l, level, levelFromFile);
}

/*!
* \brief Allow to scroll the camera in the right direction
* \param[out] camera the camera that follows the bird
* \param[out] bird the list of bird that moves with the camera
*
* Add 1 to the abscissa of camera and bird in order to move them at the same speed
*/
void cameraScrolling(Camera * camera, GenericList * bird)
{
    camera->x += camera->speed;
    setOnFirstElement(bird);
    while(!outOfGenericList(bird))
    {
        if(((Bird*)bird->current->data)->dead == 0)
            (((Bird*)bird->current->data)->x) = (((Bird*)bird->current->data)->x) + camera->speed;
        nextElement(bird);
    }
}

/*!
* \brief Modifiy the speed of game if it is run in normal mode
* \param[in] score the score of the player
* \param[out] camera the camera whose speed is changed
*/
void modifySpeed(int score, Camera * camera)
{
    if(score > 60)
        camera->speed = EXTREME;
    else if(score > 40)
        camera->speed = HIGH;
    else if(score > 20)
        camera->speed = NORMAL;
    else
        camera->speed = LOW;
}

/*!
* \brief Create an obstacle when the distance between two obstacle is reached
* \param[in] camera the view of the game
* \param[out] l the list of obstacles
* \param[in] level the file that contains the height of the obstacles
* \param[in] number the obstacle number of the new obstacle
* \param[in] levelFromFile 1 if the level is read from a file and 0 if the level is generate randomly
* \return Return 1 if the function created an obstacle, 0 otherwise
*
* If LEVEL_FROM_FILE = 1, the obstacles will be generated from the predefined level file, if not, they will be generated randomly
*/
int createObstacle(Camera * camera, List * l, FILE * level, int number, int levelFromFile)
{
    if ((camera->x + SCREEN_WIDTH >= l->first->lower.x) && (l->nbObstacles < OBSTACLE_NUMBER))
    {
        if(levelFromFile)
        {
            createObstacleFromFile(level, number, l);
            return 1;
        }
        else
        {
            createObstacleRandomly(number, l);
            return 1;
        }
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
* \param[in] sound the sound played when the bird dies
* \return 1 if the bird hit something 0 in other cases
*
* Firstly test if the bottom limit of the bird (bird->y + bird->h/2) is lower than the limit of the screen (SCREEN_HEIGHT)
* then if the right limit of the bird is farther than the left limit of the next obstacle it text if the bird is'nt beyond the gap
*/
int detectHit(Bird * bird, Obstacle * obstacle, Sound * sound)
{
    int h = 0;
    if(bird->y + BIRD_SIZE/2 >= SCREEN_HEIGHT)
    {
        h = 1;              /* Collision with the ground */
        (*sound) = DEATH;
    }
    if(bird->x + BIRD_SIZE/2 >= obstacle->lower.x)
    {
        if(bird->y - BIRD_SIZE/2 <= obstacle->upper.y + obstacle->upper.h)
        {
            h = 1;          /* Collision with the upper pipe */
        	(*sound) = DEATH;
        }
        if(bird->y + BIRD_SIZE/2 >= obstacle->lower.y)
        {
            h = 1;          /* Collision with the lower pipe */
            (*sound) = DEATH;
        }
    }
    return h;
}
/*!
* \brief if we're in normal mode the function make the bird fall of of value
* \param[out] bird the bird that will fall
* \param[in] simplifiedMode the variable that indicate if we're in simplified (1) or normal (0) mode
* \return 0 if in simplified mode or if the bird can't fall anymore and 1 in others cases
*/
int birdFall(Bird * bird, int simplifiedMode)
{
    if(simplifiedMode)
        return 0;
    else
    {
        bird->y+=10;
        if(bird->y > SCREEN_HEIGHT)
            bird->y = SCREEN_HEIGHT - BIRD_SIZE;
        return (bird->y + BIRD_SIZE != SCREEN_HEIGHT);
    }
}
/*!
* \brief The function update the score if an obstacle is passed
* \param[in] score the current score
* \param[in] bird the bird that determines the next obstacle
* \param[in] savedObstacle the obstacle saved previously
* \param[in] sound the sound played when the bird passes an obstacle
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
* \brief A function use to send the height of the bird to the IA
* \param[in] bird the bird that we seek the informations
* \return return the ratio of the bird height over the screen height
*/
int ratioBirdHeight(Bird * bird)
{
    return bird->y;
}

/*!
* \brief A function use to send the height of the next pipe to the IA
* \param[in] bird the bird that determines the next obstacle
* \param[in] l the list of obstacle
* \return return the ratio of the height of the next obstacle over the screen height
*/
int ratioPipeHeight (Bird * bird, List * l)
{
    return nextBirdObstacle(l, bird)->lower.y;
}

/*!
* \brief A function use to send the distance between the left side of the camera and the next pipe to the IA
* \param[in] bird the bird current properties
* \param[in] camera the display current properties
* \param[in] l the list of obstacle
* \return return the distance between the left side of the camera and the ?pipe
*/
int ratioPipeWidth (Bird * bird, Camera * camera, List * l)
{
    return nextBirdObstacle(l, bird)->lower.x - camera->x;
}

/*!
* \brief Allow the bird to jump
* \param[out] event the action to be changed in status "jump"
*/
void jump(Action * event)
{
    *event = JUMP;
}
/*!
* \brief Add a bird in the list of bird
* \param[out] bird the list of bird
*
* The new bird added is placed at the beginning of the game
*/
void addBird(GenericList * bird)
{
    Bird * new_bird = initBird();
    add(bird, new_bird);
}

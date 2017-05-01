#include "ia.h"

/*!
* \brief get the next pipe relative position with the bird
* \param[in] robot : the robot interface
* \return a float positive value corresponding to the next pipe relative position with the bird
*/
float getNextPipePosition(Robot* robot);

/*!
* \brief get the next pipe relative height
* \param[in] robot : the robot interface
* \return a float positive value corresponding to the next pipe relative height (relatif to the screen height)
*/
float getNextPipeHeight(Robot* robot);

/*!
* \brief get the bird relative height
* \param[in] robot : the robot interface
* \return a float positive value corresponding to the bird relative height (relatif to the screen height)
*/
float getBirdHeight(Robot* robot);

/*!
* \brief get the game status
* \param[in] robot : the robot interface
* \return 1 if the game is running, 0 if the bird is dead
*/
int getGameStatus(Robot* robot);

/*!
* \brief click with the stylus to make the bird jump
* \param[in] robot : the robot interface
*/
void jump(Robot* robot);

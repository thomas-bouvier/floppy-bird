/*!
* \file conf.h
* \brief  File containing q_learning parameters.
*/
#ifndef CONF_H
#define CONF_H

/* PARAMETERS */

#define DISCOUNT 1 /*!< Changed the weight of future rewards in the computation of the best action to make */

#define LEARNING_RATE 1 /*!< Changed the percentage of the result of the last action in the computation of the new reward */
#define DEATH_REWARD -5 /*!< Reward when the bird died */

#define LIFE_REWARD +1 /*!< Reward when the bird is alive */

#define RANDOM_INIT_REWARD 10 /*!< Limit of the random reward initialization */

#define HIGHER_QREWARD_LIMIT 1000 /*!< Higher limit of the computation of the Q reward */

#define LOWER_QREWARD_LIMIT -1000  /*!< Higher limit of the computation of the Q reward */

#define DISTANCE_ACCURACY 1 /*!< Accuracy of the distance to process less states, an higher value reduce the accuracy and raise the collision risk */

#define NB_SAVED_STATES 2 /*!< Number of saved states to compute the new reward (must be >2) */


#define NB_SAVED_ACTIONS 5 /*!< Number of saved actions to compute the new reward and to compute the velocity (must be >(NB_SAVED_STATES-1) and >4 actions) */

#define NB_FPS_BEFORE_NEXT_ACTION_MIN 10 /*!< Number of FPS minimum between two actions */

#define NB_FPS_BEFORE_NEXT_ACTION_MAX 10 /*!< Number of FPS maximum between two actions */

/* FIXED PARAMETERS */

#define NB_ACTIONS 2 /*!< Number of possible actions */

#endif

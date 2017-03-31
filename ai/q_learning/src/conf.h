/*!
* \file conf.h
* \brief  File containing q_learning parameters.
*/
#ifndef CONF_H
#define CONF_H

/* PARAMETERS */

#define DISCOUNT 1 /*!< Changed the weight of future rewards in the computation of the best action to make */
#define LEARNING_RATE 1 /*!< Changed the percentage of the result of the last action in the computation of the new reward */
#define DEATH_REWARD -10 /*!< Reward when the bird died */

#define LIFE_REWARD +5 /*!< Reward when the bird is alive */

#define NB_SAVED_STATES 3 /*!< Number of saved states to compute the new reward */

#define RANDOM_INIT_REWARD 50 /*!< Limit of the random reward initialization */

#define HIGHER_QREWARD_LIMIT 15000 /*!< Higher limit of the computation of the Q reward */

#define LOWER_QREWARD_LIMIT -15000  /*!< Higher limit of the computation of the Q reward */

/* FIXED PARAMETERS */

#define NB_ACTIONS 2 /*!< Number of possible actions */

#endif

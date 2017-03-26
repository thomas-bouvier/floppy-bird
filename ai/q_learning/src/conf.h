/*!
* \file conf.h
* \brief  File containing q_learning parameters.
*/
#ifndef CONF_H
#define CONF_H

#define DISCOUNT 0.5 /*!< Changed the weight of future rewards in the computation of the best action to make */
#define LEARNING_RATE 0.5 /*!< Changed the percentage of the result of the last action in the computation of the new reward */
#define DEATH_REWARD -100 /*< Reward when the bird died */
#define LIFE_REWARD +10 /*< Reward when the bird is alive */

#endif

/*!
* \file qlearning.h
* \brief File containing structures and prototypes linked to the deep q-learning algorithm
*/
#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdlib.h>
#include <stdio.h>

#define DISCOUNT                0.5
#define LEARNING_RATE           0.5

typedef struct State State;
typedef struct Actions Actions;
typedef struct Q Q;
typedef enum Rewards Rewards;

/*!
* \struct State qlearning.h
* \brief A State contains every informations needed to know exactly the state of a bird and pipes.
*/
struct State {
  short int delta_x;    /*!< X is the horizontal distance from the bird to the next lower pipe */
  short int delta_y;    /*!< Y is the vertical distance from the bird to the next lower pipe */
  char bird_state;	/*!< bird_state is the state of the bird with those parameters, 0=death 1=live */
};

/*!
* \struct Actions qlearning.h
* \brief Actions contains every actions that the bird can perform.
*/
struct Actions {
  char jump;	/*!< jump is equal to zero if no jump otherwise one */
};

/*!
* \struct Q qlearning.h
* \brief Q linked a state with an action
*/
struct Q {
	short int index;
	struct State *state;
	struct Actions *actions;
};

/**
 * \enum Rewards qlearning.h
 * \brief the reward given to the associated state/action according to the future state
 */
enum Rewards {
  life = +15,  /*!< A positive reward if the bird still lives in the future state */
  death = -1000; /*!< A negative reward if the bird dies */
};

#endif // QLEARNING_H

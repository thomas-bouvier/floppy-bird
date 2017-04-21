/*!
* \file q_learning.h
* \brief File containing q_learning main loop and initialization
*/
#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include "qmatrix.h"
#include "file_manager.h"

int q_learning_loop(MatrixQ * matrixQ, int * last_states, int * last_action, int dx, int dy, int bird_state);

#endif

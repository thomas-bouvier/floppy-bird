#include "neat_utils.h"

/*!
* \brief Return a random number between 0 and the specified parameter
* \param[in] limit the upper limit
* \return int Return a random number in [0; limit[
*/
int randomLimit(int limit) {
  if ((limit - 1) == RAND_MAX) {
    return rand();
  } else {
    long end = RAND_MAX / limit;
    end *= limit;

    int r;
    while ((r = rand()) >= end);

    return r % limit;
  }
}

/*!
* \brief Return a random number between 0 and 1
* \return double Return a random number in [0; 1]
*/
double random01() {
    return (double) randomLimit(RAND_MAX) / (double) RAND_MAX;
}

/*!
* \brief Return a 0 or 1, at equal probability
* \return int 0 or 1
*/
int randomBool() {
  return randomLimit(2);
}

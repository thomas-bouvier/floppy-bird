#include "utils.h"

/*!
* \brief Return a random number between 0 and the specified parameter
* \param[in] limit the upper limit
* \return Return a random number in [0; limit[
*/
int randomAtMost(int limit) {
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
* \return Return a random number in [0; 1]
*/
double random01() {
    return (double) rand() / (double) RAND_MAX ;
}

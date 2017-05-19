#include "neat_utils.h"

/*!
* \brief Return a random number between 0 and the specified parameter (included)
* \param[in] limit the upper limit
* \return int Return a random number in [0; limit]
*/
int randomLimit(long limit) {
    if (limit < 0)
        return 0;

    unsigned long num_bins = (unsigned long) limit + 1;
    unsigned long num_rand = (unsigned long) RAND_MAX + 1;
    unsigned long bin_size = num_rand / num_bins;
    unsigned long defect = num_rand % num_bins;

    long x;
    do {
        x = random();
    } while (num_rand - defect <= (unsigned long) x);

    return x / bin_size;
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
  return randomLimit(1);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/constants.h"
#include "../src/bird.h"

/* initBird */

static int setup_initBird(void ** state) {
  Bird * bird = malloc(sizeof(Bird));

  if (bird == NULL) {
    return -1;
  }

  *state = bird;

  return 0;
}

static void test_initBird(void ** state) {
  Bird * bird = (Bird *) (* state);
  initBird(bird);

  assert_int_equal(bird->x, BIRD_X_OFFSET);
  assert_int_equal(bird->y, SCREEN_HEIGHT / 2);
  assert_int_equal(bird->w, BIRD_SIZE);
  assert_int_equal(bird->h, BIRD_SIZE);
  assert_int_equal(bird->dir_y, 0);
}

static int teardown_initBird(void ** state) {
  free(*state);
  return 0;
}

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown(test_initBird, setup_initBird, teardown_initBird),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

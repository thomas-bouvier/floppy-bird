#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/q_learning.h"
#include "../src/qmatrix.h"
#include "../src/game_state.h"
#include "../src/utils.h"
#include "../src/file_manager.h"

int main() {
  const struct CMUnitTest tests[] = {
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

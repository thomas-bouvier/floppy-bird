#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/network.h"
#include "../src/neuronList.h"

static int setup_initNeuronList(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  *state = neuron_list;

  return 0;
}

static void test_initNeuronList(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);
  initNeuronList(neuron_list);

  assert_null(neuron_list->first);
  assert_null(neuron_list->current);
  assert_null(neuron_list->last);
}

static int teardown_initNeuronList(void ** state) {
  freeNeuronList(*state);
  return 0;
}

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown(test_initNeuronList, setup_initNeuronList, teardown_initNeuronList)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

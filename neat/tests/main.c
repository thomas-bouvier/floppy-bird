#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/network.h"
#include "../src/neuronList.h"

/* initNeuronList */

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

/* emptyNeuronList */

static int setup_emptyNeuronList(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);

  *state = neuron_list;

  return 0;
}

static void test_emptyNeuronList(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  assert_int_equal(emptyNeuronList(neuron_list), 1);
}

static int setup_emptyNeuronListOneElementRemoved(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);
  addNeuron(neuron_list, newNeuron(UNKNOW));
  deleteNeuron(neuron_list, getCurrentNeuron(neuron_list));

  *state = neuron_list;

  return 0;
}

static void test_emptyNeuronListOneElementRemoved(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  assert_int_equal(emptyNeuronList(neuron_list), 1);
}

static int teardown_emptyNeuronList(void ** state) {
  freeNeuronList(*state);
  return 0;
}

/* outOfNeuronList */

static int setup_outOfNeuronList(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);

  *state = neuron_list;

  return 0;
}

static void test_outOfNeuronList(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  assert_int_equal(outOfNeuronList(neuron_list), 1);
}

static int setup_outOfNeuronListOneElement(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);
  addNeuron(neuron_list, newNeuron(UNKNOW));

  *state = neuron_list;

  return 0;
}

static void test_outOfNeuronListOneElement(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  assert_int_equal(outOfNeuronList(neuron_list), 0);
}

static int setup_outOfNeuronListOneElementNext(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);
  addNeuron(neuron_list, newNeuron(UNKNOW));
  nextNeuron(neuron_list);

  *state = neuron_list;

  return 0;
}

static void test_outOfNeuronListOneElementNext(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  assert_int_equal(outOfNeuronList(neuron_list), 1);
}

static int teardown_outOfNeuronList(void ** state) {
  freeNeuronList(*state);
  return 0;
}

/* setOnFirstNeuron */

static int setup_setOnFirstNeuronEmptyList(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);

  *state = neuron_list;

  return 0;
}

static void test_setOnFirstNeuronEmptyList(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  setOnFirstNeuron(neuron_list);

  assert_null(neuron_list->current);
}

static int setup_setOnFirstNeuronOneElement(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);
  addNeuron(neuron_list, newNeuron(UNKNOW));

  *state = neuron_list;

  return 0;
}

static void test_setOnFirstNeuronOneElement(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  setOnFirstNeuron(neuron_list);

  assert_ptr_equal(neuron_list->current, neuron_list->first);
}

static int setup_setOnFirstNeuron(void ** state) {
  NeuronList * neuron_list = newNeuronList();

  if (neuron_list == NULL)
    return -1;

  initNeuronList(neuron_list);
  addNeuron(neuron_list, newNeuron(UNKNOW));
  addNeuron(neuron_list, newNeuron(BIAS));
  addNeuron(neuron_list, newNeuron(INPUT));
  addNeuron(neuron_list, newNeuron(OUTPUT));

  *state = neuron_list;

  return 0;
}

static void test_setOnFirstNeuron(void ** state) {
  NeuronList * neuron_list = (NeuronList *) (* state);

  setOnFirstNeuron(neuron_list);

  assert_ptr_equal(neuron_list->current, neuron_list->first);
}

static int teardown_setOnFirstNeuron(void ** state) {
  freeNeuronList(*state);
  return 0;
}

/* initConnectionGeneList */

static int setup_initConnectionGeneList(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  *state = connection_gene_list;

  return 0;
}

static void test_initConnectionGeneList(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);
  initConnectionGeneList(connection_gene_list);

  assert_null(connection_gene_list->first);
  assert_null(connection_gene_list->current);
  assert_null(connection_gene_list->last);
}

static int teardown_initConnectionGeneList(void ** state) {
  freeConnectionGeneList(*state);
  return 0;
}

/* emptyConnectionGeneList */

static int setup_emptyConnectionGeneList(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);

  *state = connection_gene_list;

  return 0;
}

static void test_emptyConnectionGeneList(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  assert_int_equal(emptyConnectionGeneList(connection_gene_list), 1);
}

static int setup_emptyConnectionGeneListOneElementRemoved(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));
  deleteConnectionGene(connection_gene_list, getCurrentConnectionGene(connection_gene_list));

  *state = connection_gene_list;

  return 0;
}

static void test_emptyConnectionGeneListOneElementRemoved(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  assert_int_equal(emptyConnectionGeneList(connection_gene_list), 1);
}

static int teardown_emptyConnectionGeneList(void ** state) {
  freeConnectionGeneList(*state);
  return 0;
}

/* outOfConnectionGeneList */

static int setup_outOfConnectionGeneList(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);

  *state = connection_gene_list;

  return 0;
}

static void test_outOfConnectionGeneList(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  assert_int_equal(outOfConnectionGeneList(connection_gene_list), 1);
}

static int teardown_outOfConnectionGeneList(void ** state) {
  freeConnectionGeneList(*state);
  return 0;
}

static int setup_outOfConnectionGeneListOneElement(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));

  *state = connection_gene_list;

  return 0;
}

static void test_outOfConnectionGeneListOneElement(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  assert_int_equal(outOfConnectionGeneList(connection_gene_list), 0);
}

static int setup_outOfConnectionGeneListOneElementNext(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));
  nextConnectionGene(connection_gene_list);

  *state = connection_gene_list;

  return 0;
}

static void test_outOfConnectionGeneListOneElementNext(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  assert_int_equal(outOfConnectionGeneList(connection_gene_list), 1);
}

/* setOnFirstNeuron */

static int setup_setOnFirstConnectionGeneEmptyList(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);

  *state = connection_gene_list;

  return 0;
}

static void test_setOnFirstConnectionGeneEmptyList(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  setOnFirstConnectionGene(connection_gene_list);

  assert_null(connection_gene_list->current);
}

static int setup_setOnFirstConnectionGeneOneElement(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));

  *state = connection_gene_list;

  return 0;
}

static void test_setOnFirstConnectionGeneOneElement(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  setOnFirstConnectionGene(connection_gene_list);

  assert_ptr_equal(connection_gene_list->current, connection_gene_list->first);
}

static int setup_setOnFirstConnectionGene(void ** state) {
  ConnectionGeneList * connection_gene_list = newConnectionGeneList();

  if (connection_gene_list == NULL)
    return -1;

  initConnectionGeneList(connection_gene_list);
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));
  addConnectionGene(connection_gene_list, newConnectionGene(0.0, 0, 1));

  *state = connection_gene_list;

  return 0;
}

static void test_setOnFirstConnectionGene(void ** state) {
  ConnectionGeneList * connection_gene_list = (ConnectionGeneList *) (* state);

  setOnFirstConnectionGene(connection_gene_list);

  assert_ptr_equal(connection_gene_list->current, connection_gene_list->first);
}

static int teardown_setOnFirstConnectionGene(void ** state) {
  freeConnectionGeneList(*state);
  return 0;
}

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown(test_initNeuronList, setup_initNeuronList, teardown_initNeuronList),
    cmocka_unit_test_setup_teardown(test_emptyNeuronList, setup_emptyNeuronList, teardown_emptyNeuronList),
    cmocka_unit_test_setup_teardown(test_emptyNeuronListOneElementRemoved, setup_emptyNeuronListOneElementRemoved, teardown_emptyNeuronList),
    cmocka_unit_test_setup_teardown(test_outOfNeuronList, setup_outOfNeuronList, teardown_outOfNeuronList),
    cmocka_unit_test_setup_teardown(test_outOfNeuronListOneElement, setup_outOfNeuronListOneElement, teardown_outOfNeuronList),
    cmocka_unit_test_setup_teardown(test_outOfNeuronListOneElementNext, setup_outOfNeuronListOneElementNext, teardown_outOfNeuronList),
    cmocka_unit_test_setup_teardown(test_setOnFirstNeuronEmptyList, setup_setOnFirstNeuronEmptyList, teardown_setOnFirstNeuron),
    cmocka_unit_test_setup_teardown(test_setOnFirstNeuronOneElement, setup_setOnFirstNeuronOneElement, teardown_setOnFirstNeuron),
    cmocka_unit_test_setup_teardown(test_setOnFirstNeuron, setup_setOnFirstNeuron, teardown_setOnFirstNeuron),

    cmocka_unit_test_setup_teardown(test_initConnectionGeneList, setup_initConnectionGeneList, teardown_initConnectionGeneList),
    cmocka_unit_test_setup_teardown(test_emptyConnectionGeneList, setup_emptyConnectionGeneList, teardown_emptyConnectionGeneList),
    cmocka_unit_test_setup_teardown(test_emptyConnectionGeneListOneElementRemoved, setup_emptyConnectionGeneListOneElementRemoved, teardown_emptyConnectionGeneList),
    cmocka_unit_test_setup_teardown(test_outOfConnectionGeneList, setup_outOfConnectionGeneList, teardown_outOfConnectionGeneList),
    cmocka_unit_test_setup_teardown(test_outOfConnectionGeneListOneElement, setup_outOfConnectionGeneListOneElement, teardown_outOfConnectionGeneList),
    cmocka_unit_test_setup_teardown(test_outOfConnectionGeneListOneElementNext, setup_outOfConnectionGeneListOneElementNext, teardown_outOfConnectionGeneList),
    cmocka_unit_test_setup_teardown(test_setOnFirstConnectionGeneEmptyList, setup_setOnFirstConnectionGeneEmptyList, teardown_setOnFirstConnectionGene),
    cmocka_unit_test_setup_teardown(test_setOnFirstConnectionGeneOneElement, setup_setOnFirstConnectionGeneOneElement, teardown_setOnFirstConnectionGene),
    cmocka_unit_test_setup_teardown(test_setOnFirstConnectionGene, setup_setOnFirstConnectionGene, teardown_setOnFirstConnectionGene),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

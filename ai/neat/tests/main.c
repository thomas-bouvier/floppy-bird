#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/genome.h"
#include "../src/network.h"
#include "../src/generic_list.h"
#include "../src/neat_utils.h"

/*========================================================================
    newNode
========================================================================*/

static void test_newNode(void ** state) {
    Node * node = newNode();

    *state = node;

    assert_null(node->data);
    assert_null(node->next);
}

static int teardown_newNode(void ** state) {
    free(*state);
    return 0;
}

/*========================================================================
    newGenericList
========================================================================*/

static void test_newGenericListMissingFreeFunction(void ** state) {
    GenericList * list = newGenericList(NULL, NULL);

    assert_null(list);

    *state = list;
}

static void test_newGenericListMissingCloneFunction(void ** state) {
    GenericList * list = newGenericList(NULL, freeNeuron);

    assert_non_null(list);
    assert_null(list->clone_function);
    assert_ptr_equal(list->free_function, freeNeuron);

    initGenericList(list);

    *state = list;
}

static void test_newGenericList(void ** state) {
    GenericList * list = newGenericList(cloneNeuron, freeNeuron);

    assert_non_null(list);

    assert_ptr_equal(list->clone_function, cloneNeuron);
    assert_ptr_equal(list->free_function, freeNeuron);

    initGenericList(list);

    *state = list;
}

static int teardown_newGenericList(void ** state) {
    if (*state != NULL)
        freeGenericList(*state);

    return 0;
}

/*========================================================================
    initGenericList
========================================================================*/

static int setup_initGenericList(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  *state = list;

  return 0;
}

static void test_initGenericList(void ** state) {
  GenericList * list = (GenericList *) (* state);
  initGenericList(list);

  assert_null(list->first);
  assert_null(list->current);
  assert_null(list->last);
}

static int teardown_initGenericList(void ** state) {
  freeGenericList(*state);
  return 0;
}

/*========================================================================
    cloneGenericList
========================================================================*/

typedef struct {
    void * base_list;
    void * clone_list;
} CloneGenericListStruct;

static int setup_cloneGenericList(void ** state) {
    CloneGenericListStruct * helper = NULL;
    GenericList * list = newGenericList(cloneNeuron, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    if ((helper = (CloneGenericListStruct *) malloc(sizeof(CloneGenericListStruct))) == (CloneGenericListStruct *) NULL)
        return -1;

    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    helper->base_list = list;

    *state = helper;

    return 0;
}

static void test_cloneGenericList(void ** state) {
    CloneGenericListStruct * helper = (CloneGenericListStruct *) (* state);
    GenericList * base_list = (GenericList *) helper->base_list;
    GenericList * clone_list = NULL;

    clone_list = cloneGenericList(base_list);
    helper->clone_list = clone_list;

    assert_ptr_equal(base_list->clone_function, clone_list->clone_function);
    assert_ptr_equal(base_list->free_function, clone_list->free_function);

    setOnFirstElement(base_list);
    setOnFirstElement(clone_list);

    assert_int_equal(((Neuron *) base_list->current->data)->type, INPUT);
    assert_int_equal(((Neuron *) clone_list->current->data)->type, INPUT);
    assert_ptr_not_equal(base_list->current, clone_list->current);

    nextElement(base_list);
    nextElement(clone_list);

    assert_int_equal(((Neuron *) base_list->current->data)->type, OUTPUT);
    assert_int_equal(((Neuron *) clone_list->current->data)->type, OUTPUT);
    assert_ptr_not_equal(base_list->current, clone_list->current);
}

static int teardown_cloneGenericList(void ** state) {
    freeGenericList(((CloneGenericListStruct *) *state)->base_list);
    freeGenericList(((CloneGenericListStruct *) *state)->clone_list);
    free(*state);
    return 0;
}

/*========================================================================
    emptyGenericList
========================================================================*/

static int setup_emptyGenericList(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);

  *state = list;

  return 0;
}

static void test_emptyGenericList(void ** state) {
  GenericList * list = (GenericList *) (* state);

  assert_int_equal(emptyGenericList(list), 1);
}

static int setup_emptyGenericListOneElementRemoved(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);
  add(list, newNeuron(UNKNOW));
  delete(list, getCurrent(list));

  *state = list;

  return 0;
}

static void test_emptyGenericListOneElementRemoved(void ** state) {
  GenericList * list = (GenericList *) (* state);

  assert_int_equal(emptyGenericList(list), 1);
}

static int teardown_emptyGenericList(void ** state) {
  freeGenericList(*state);
  return 0;
}

/*========================================================================
    outOfGenericList
========================================================================*/

static int setup_outOfGenericList(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);

  *state = list;

  return 0;
}

static void test_outOfGenericList(void ** state) {
  GenericList * list = (GenericList *) (* state);

  assert_int_equal(outOfGenericList(list), 1);
}

static int setup_outOfGenericListOneElement(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);
  add(list, newNeuron(UNKNOW));

  *state = list;

  return 0;
}

static void test_outOfGenericListOneElement(void ** state) {
  GenericList * list = (GenericList *) (* state);

  assert_int_equal(outOfGenericList(list), 0);
}

static int setup_outOfGenericListOneElementNext(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);
  add(list, newNeuron(UNKNOW));
  nextElement(list);

  *state = list;

  return 0;
}

static void test_outOfGenericListOneElementNext(void ** state) {
  GenericList * list = (GenericList *) (* state);

  assert_int_equal(outOfGenericList(list), 1);
}

static int teardown_outOfGenericList(void ** state) {
  freeGenericList(*state);
  return 0;
}

/*========================================================================
    setOnFirstElement
========================================================================*/

static int setup_setOnFirstElementEmptyList(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);

  *state = list;

  return 0;
}

static void test_setOnFirstElementEmptyList(void ** state) {
  GenericList * list = (GenericList *) (* state);

  setOnFirstElement(list);

  assert_ptr_equal(list->current, list->first);
  assert_null(list->current);
}

static int setup_setOnFirstElementOneElement(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);
  add(list, newNeuron(OUTPUT));

  *state = list;

  return 0;
}

static void test_setOnFirstElementOneElement(void ** state) {
  GenericList * list = (GenericList *) (* state);

  setOnFirstElement(list);

  assert_ptr_equal(list->current, list->first);
  assert_int_equal(((Neuron *) list->current->data)->type, OUTPUT);
}

static int setup_setOnFirstElement(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);
  add(list, newNeuron(OUTPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(UNKNOW));

  *state = list;

  return 0;
}

static void test_setOnFirstElement(void ** state) {
  GenericList * list = (GenericList *) (* state);

  setOnFirstElement(list);

  assert_ptr_equal(list->current, list->first);
  assert_int_equal(((Neuron *) list->current->data)->type, OUTPUT);
}

static int teardown_setOnFirstElement(void ** state) {
  freeGenericList(*state);
  return 0;
}

/*========================================================================
    setOn
========================================================================*/

static int setup_setOnEmptyList(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);

  *state = list;

  return 0;
}

static void test_setOnEmptyList(void ** state) {
  GenericList * list = (GenericList *) (* state);

  setOn(list, 45);

  assert_null(list->current);
}

static int setup_setOn(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(OUTPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));

  *state = list;

  return 0;
}

static void test_setOn(void ** state) {
  GenericList * list = (GenericList *) (* state);

  setOn(list, 7);

  assert_int_equal(((Neuron *) list->current->data)->type, OUTPUT);
}

static int teardown_setOn(void ** state) {
  freeGenericList(*state);
  return 0;
}

/*========================================================================
    nextElement
========================================================================*/

static int setup_nextElement(void ** state) {
  GenericList * list = newGenericList(NULL, freeNeuron);

  if (list == NULL)
    return -1;

  initGenericList(list);
  add(list, newNeuron(UNKNOW));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(OUTPUT));
  add(list, newNeuron(UNKNOW));
  add(list, newNeuron(UNKNOW));
  add(list, newNeuron(INPUT));
  add(list, newNeuron(OUTPUT));

  setOnFirstElement(list);

  *state = list;

  return 0;
}

static void test_nextElement(void ** state) {
  int i;
  GenericList * list = (GenericList *) (* state);

  for (i = 0; i < 6; ++i)
    nextElement(list);

  assert_int_equal(((Neuron *) list->current->data)->type, INPUT);

  for (i = 0; i < 60; ++i)
    nextElement(list);

  assert_null(list->current);
}

static int teardown_nextElement(void ** state) {
  freeGenericList(*state);
  return 0;
}


/*========================================================================
    getCurrent
========================================================================*/

static int setup_getCurrent(void ** state) {
    GenericList * list = newGenericList(NULL, freeNeuron);

    if (list == NULL)
      return -1;

    initGenericList(list);
    add(list, newNeuron(UNKNOW));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(UNKNOW));
    add(list, newNeuron(UNKNOW));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));

    setOnFirstElement(list);

    *state = list;

    return 0;
}

static void test_getCurrent(void ** state) {
    GenericList * list = (GenericList *) (* state);

    setOnFirstElement(list);

    assert_int_equal(((Neuron *) list->current->data)->type, UNKNOW);

    nextElement(list);
    nextElement(list);
    nextElement(list);

    assert_int_equal(((Neuron *) list->current->data)->type, OUTPUT);

    setOn(list, 60);

    assert_null(getCurrent(list));
}

static int teardown_getCurrent(void ** state) {
    freeGenericList(*state);
    return 0;
}

/*========================================================================
    add
========================================================================*/

static int setup_add(void ** state) {
    GenericList * list = newGenericList(NULL, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    *state = list;

    return 0;
}

static void test_add(void ** state) {
    GenericList * list = (GenericList *) (* state);

    assert_int_equal(emptyGenericList(list), 1);

    assert_int_equal(add(list, newNeuron(INPUT)), 1);

    assert_int_equal(emptyGenericList(list), 0);

    assert_int_equal(((Neuron *) list->current->data)->type, INPUT);

    assert_int_equal(add(list, newNeuron(UNKNOW)), 1);
    assert_int_equal(add(list, newNeuron(OUTPUT)), 1);

    assert_int_equal(add(list, NULL), 0);
}

static int teardown_add(void ** state) {
    freeGenericList(*state);
    return 0;
}

/*========================================================================
    delete
========================================================================*/

static int setup_delete(void ** state) {
    GenericList * list = newGenericList(NULL, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(INPUT));

    *state = list;

    return 0;
}

static int setup_deleteOneElement(void ** state) {
    GenericList * list = newGenericList(NULL, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    add(list, newNeuron(INPUT));

    *state = list;

    return 0;
}

static void test_delete(void ** state) {
    GenericList * list = (GenericList *) (* state);

    setOn(list, 2);
    assert_int_equal(delete(list, getCurrent(list)), 1);

    setOnFirstElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, INPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, OUTPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, OUTPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, INPUT);

    nextElement(list);
    assert_int_equal(delete(list, getCurrent(list)), 0);
}

static void test_deleteFirst(void ** state) {
    GenericList * list = (GenericList *) (* state);

    setOnFirstElement(list);
    assert_int_equal(delete(list, getCurrent(list)), 1);

    setOnFirstElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, OUTPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, INPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, OUTPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, INPUT);
}

static void test_deleteLast(void ** state) {
    GenericList * list = (GenericList *) (* state);

    setOn(list, 4);
    assert_int_equal(delete(list, getCurrent(list)), 1);

    setOnFirstElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, INPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, OUTPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, INPUT);

    nextElement(list);
    assert_int_equal(((Neuron *) getCurrent(list))->type, OUTPUT);
}

static void test_deleteOneElement(void ** state) {
    GenericList * list = (GenericList *) (* state);

    setOnFirstElement(list);
    assert_int_equal(delete(list, getCurrent(list)), 1);

    assert_null(list->first);
    assert_null(list->last);
}

static int teardown_delete(void ** state) {
    freeGenericList(*state);
    return 0;
}

/*========================================================================
    clear
========================================================================*/

static int setup_clearGenericList(void ** state) {
    GenericList * list = newGenericList(cloneNeuron, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(BASIC));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(BASIC));
    add(list, newNeuron(INPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(BASIC));
    add(list, newNeuron(INPUT));

    *state = list;

    return 0;
}

static void test_clearGenericList(void ** state) {
    GenericList * list = (GenericList *) (* state);

    clearGenericList(list);

    assert_null(list->first);
    assert_null(list->last);
    assert_true(emptyGenericList(list));
    assert_int_equal(count(list), 0);
}

static int teardown_clearGenericList(void ** state) {
    freeGenericList(*state);
    return 0;
}

/*========================================================================
    sort
========================================================================*/

static int compareFitnessCulling(const void * genome_1, const void * genome_2) {
    int diff = ((Genome *) genome_1)->fitness - ((Genome *) genome_2)->fitness;

    if (diff == 0.0)
        return 0;
    else if (diff < 0.0)
        return -1;

    return 1;
}

typedef struct {
    GenericList * list;
    int innovation;
} SortStruct;

static int setup_sort(void ** state) {
    SortStruct * helper = NULL;
    Genome * genome_1 = NULL;
    Genome * genome_2 = NULL;
    Genome * genome_3 = NULL;
    Genome * genome_4 = NULL;
    Genome * genome_5 = NULL;
    Genome * genome_6 = NULL;
    GenericList * list = newGenericList(NULL, freeGenome);

    if (list == NULL)
        return -1;

    initGenericList(list);

    if ((helper = (SortStruct *) malloc(sizeof(SortStruct))) == (SortStruct *) NULL)
        return -1;

    helper->innovation = 40;

    genome_1 = newGenome(&helper->innovation);
    if (genome_1 == NULL)
        return -1;

    genome_1->fitness = 1.05;
    add(list, genome_1);

    genome_2 = newGenome(&helper->innovation);
    if (genome_2 == NULL)
        return -1;

    genome_2->fitness = 5.056;
    add(list, genome_2);

    genome_3 = newGenome(&helper->innovation);
    if (genome_3 == NULL)
        return -1;

    genome_3->fitness = 1.05;
    add(list, genome_3);

    genome_4 = newGenome(&helper->innovation);
    if (genome_4 == NULL)
        return -1;

    genome_4->fitness = -0.0419;
    add(list, genome_4);

    genome_5 = newGenome(&helper->innovation);
    if (genome_5 == NULL)
        return -1;

    genome_5->fitness = 45.0;
    add(list, genome_5);

    genome_6 = newGenome(&helper->innovation);
    if (genome_6 == NULL)
        return -1;

    genome_6->fitness = 10.2;
    add(list, genome_6);

    helper->list = list;

    *state = helper;

    return 0;
}

static void test_sort(void ** state) {
    SortStruct * helper = (SortStruct *) (* state);
    GenericList * list = helper->list;

    sort(list, compareFitnessCulling);

    setOnFirstElement(list);
    assert_true(((Genome *) getCurrent(list))->fitness - 45.0 == 0);

    nextElement(list);
    assert_true(((Genome *) getCurrent(list))->fitness - 10.2 == 0);

    nextElement(list);
    assert_true(((Genome *) getCurrent(list))->fitness - 5.056 == 0);

    nextElement(list);
    assert_true(((Genome *) getCurrent(list))->fitness - 1.05 == 0);

    nextElement(list);
    assert_true(((Genome *) getCurrent(list))->fitness - 1.05 == 0);

    nextElement(list);
    assert_true(((Genome *) getCurrent(list))->fitness + 0.0419 == 0);

    nextElement(list);
    assert_null(getCurrent(list));
}

static int teardown_sort(void ** state) {
    freeGenericList(((SortStruct *) *state)->list);
    free(*state);
    return 0;
}

/*========================================================================
    find
========================================================================*/

typedef struct {
    GenericList * list;
    Neuron * element;
    Neuron * to_be_freed;
} FindStruct;

static int setup_find(void ** state) {
    FindStruct * helper = NULL;
    Neuron * element = NULL;
    GenericList * list = newGenericList(NULL, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    if ((helper = (FindStruct *) malloc(sizeof(FindStruct))) == (FindStruct *) NULL)
        return -1;

    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(OUTPUT));

    element = newNeuron(INPUT);
    helper->element = element;
    add(list, element);

    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(OUTPUT));

    helper->list = list;

    *state = helper;

    return 0;
}

static void test_find(void ** state) {
    FindStruct * helper = (FindStruct *) (* state);
    GenericList * list = helper->list;
    Neuron * element = helper->element;
    Neuron * to_be_freed = NULL;

    assert_int_equal(find(list, NULL), 0);
    assert_int_equal(find(list, element), 1);

    to_be_freed = newNeuron(OUTPUT);
    helper->to_be_freed = to_be_freed;
    assert_int_equal(find(list, to_be_freed), 0);
}

static int teardown_find(void ** state) {
    freeGenericList(((FindStruct *) *state)->list);
    freeNeuron(((FindStruct *) *state)->to_be_freed);
    free(*state);
    return 0;
}

/*========================================================================
    count
========================================================================*/

static int setup_countEmptyList(void ** state) {
    GenericList * list = newGenericList(NULL, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    *state = list;

    return 0;
}

static int setup_count(void ** state) {
    GenericList * list = newGenericList(NULL, freeNeuron);

    if (list == NULL)
        return -1;

    initGenericList(list);

    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(OUTPUT));
    add(list, newNeuron(OUTPUT));

    *state = list;

    return 0;
}

static void test_countEmptyList(void ** state) {
    GenericList * list = (GenericList *) (* state);

    assert_int_equal(count(list), 0);
}

static void test_count(void ** state) {
    GenericList * list = (GenericList *) (* state);

    assert_int_equal(count(list), 5);
}

static int teardown_count(void ** state) {
    freeGenericList(*state);
    return 0;
}

/*========================================================================
    randomLimit
========================================================================*/

static void test_randomLimit(void ** state) {
    int i;

    for (i = 0; i < 10000; ++i)
        assert_in_range(randomLimit(100), 0, 100);
}

/*========================================================================
    random01
========================================================================*/

static void test_random01(void ** state) {
    int i;

    for (i = 0; i < 10000; ++i)
        assert_in_range(random01(), 0, 1);
}

/*========================================================================
    randomBool
========================================================================*/

static void test_randomBool(void ** state) {
    int i;

    for (i = 0; i < 10000; ++i)
        assert_in_range(randomBool(), 0, 1);
}

/*========================================================================
    newGenome
========================================================================*/

static void test_newGenome(void ** state) {
    int innovation = 40;
    Genome * genome = newGenome(&innovation);

    assert_non_null(genome);
    assert_non_null(genome->neurons);
    assert_non_null(genome->connection_genes);
    assert_true(emptyGenericList(genome->neurons));
    assert_true(emptyGenericList(genome->connection_genes));
    assert_true(genome->fitness == 0.0);
    assert_int_equal(genome->mutation_rates[0], POINT_MUTATION_RATE),
    assert_int_equal(genome->mutation_rates[1], LINK_MUTATION_RATE),
    assert_int_equal(genome->mutation_rates[2], NODE_MUTATION_RATE),
    assert_int_equal(genome->mutation_rates[3], ENABLE_DISABLE_MUTATION_RATE),
    assert_int_equal(genome->global_rank, 0);
    assert_ptr_equal(genome->innovation, &innovation);
    assert_int_equal(genome->nb_mutations, 0);

    *state = genome;
}

static int teardown_newGenome(void ** state) {
    freeGenome(*state);
    return 0;
}

/*========================================================================
    tests
========================================================================*/

int main() {
    const struct CMUnitTest tests_generic_list[] = {
        cmocka_unit_test_teardown(test_newNode, teardown_newNode),
        cmocka_unit_test_teardown(test_newGenericListMissingFreeFunction, teardown_newGenericList),
        cmocka_unit_test_teardown(test_newGenericListMissingCloneFunction, teardown_newGenericList),
        cmocka_unit_test_teardown(test_newGenericList, teardown_newGenericList),
        cmocka_unit_test_setup_teardown(test_initGenericList, setup_initGenericList, teardown_initGenericList),
        cmocka_unit_test_setup_teardown(test_cloneGenericList, setup_cloneGenericList, teardown_cloneGenericList),
        cmocka_unit_test_setup_teardown(test_emptyGenericList, setup_emptyGenericList, teardown_emptyGenericList),
        cmocka_unit_test_setup_teardown(test_emptyGenericListOneElementRemoved, setup_emptyGenericListOneElementRemoved, teardown_emptyGenericList),
        cmocka_unit_test_setup_teardown(test_outOfGenericList, setup_outOfGenericList, teardown_outOfGenericList),
        cmocka_unit_test_setup_teardown(test_outOfGenericListOneElement, setup_outOfGenericListOneElement, teardown_outOfGenericList),
        cmocka_unit_test_setup_teardown(test_outOfGenericListOneElementNext, setup_outOfGenericListOneElementNext, teardown_outOfGenericList),
        cmocka_unit_test_setup_teardown(test_setOnFirstElementEmptyList, setup_setOnFirstElementEmptyList, teardown_setOnFirstElement),
        cmocka_unit_test_setup_teardown(test_setOnFirstElementOneElement, setup_setOnFirstElementOneElement, teardown_setOnFirstElement),
        cmocka_unit_test_setup_teardown(test_setOnFirstElement, setup_setOnFirstElement, teardown_setOnFirstElement),
        cmocka_unit_test_setup_teardown(test_setOnEmptyList, setup_setOnEmptyList, teardown_setOn),
        cmocka_unit_test_setup_teardown(test_setOn, setup_setOn, teardown_setOn),
        cmocka_unit_test_setup_teardown(test_nextElement, setup_nextElement, teardown_nextElement),
        cmocka_unit_test_setup_teardown(test_getCurrent, setup_getCurrent, teardown_getCurrent),
        cmocka_unit_test_setup_teardown(test_add, setup_add, teardown_add),
        cmocka_unit_test_setup_teardown(test_delete, setup_delete, teardown_delete),
        cmocka_unit_test_setup_teardown(test_deleteFirst, setup_delete, teardown_delete),
        cmocka_unit_test_setup_teardown(test_deleteLast, setup_delete, teardown_delete),
        cmocka_unit_test_setup_teardown(test_deleteOneElement, setup_deleteOneElement, teardown_delete),
        cmocka_unit_test_setup_teardown(test_clearGenericList, setup_clearGenericList, teardown_clearGenericList),
        cmocka_unit_test_setup_teardown(test_sort, setup_sort, teardown_sort),
        cmocka_unit_test_setup_teardown(test_find, setup_find, teardown_find),
        cmocka_unit_test_setup_teardown(test_countEmptyList, setup_countEmptyList, teardown_count),
        cmocka_unit_test_setup_teardown(test_count, setup_count, teardown_count),
    };

    cmocka_run_group_tests(tests_generic_list, NULL, NULL);

    const struct CMUnitTest tests_neat_utils[] = {
        cmocka_unit_test(test_randomLimit),
        cmocka_unit_test(test_random01),
        cmocka_unit_test(test_randomBool),
    };

    cmocka_run_group_tests(tests_neat_utils, NULL, NULL);

    const struct CMUnitTest tests_genome[] = {
        cmocka_unit_test_teardown(test_newGenome, teardown_newGenome),
    };

    cmocka_run_group_tests(tests_genome, NULL, NULL);

    return EXIT_SUCCESS;
}

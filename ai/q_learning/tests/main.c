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
#include "../src/conf.h"

/** utils.c **/
/* randomAtMost */
static void test_randomAtMostpositiveLimit(void ** state)
{
	assert_in_range(randomAtMost(20), 0, 20);
}

static void test_randomAtMostzeroLimit(void ** state)
{
	assert_int_equal(0, randomAtMost(0));
}

/* powerOf */
static void test_powerOfpositiveValue(void ** state)
{
	assert_int_equal(9, powerOf(3, 2));
}

static void test_powerOfnegativeValue(void ** state)
{
	assert_int_equal(9, powerOf(-3, 2));
}

static void test_powerOfzero(void ** state)
{
	assert_int_equal(1, powerOf(3, 0));
}

/* shift_array */
typedef struct {
	int * data;
	int length;
}MyShiftArray;

static int setup_shift_array(void ** state)
{
	int i;
	MyShiftArray * array = (MyShiftArray*) malloc(sizeof(MyShiftArray));
	if(array == NULL)
		return -1;
	array->length = 3;
	array->data = (int*) malloc(array->length * sizeof(int));
	if(array->data == NULL)
		return -1;
	for(i=0;i<array->length;++i)
		array->data[i] = i+1;

	*state = array;
	return 0;
}

static void test_shift_array(void ** state)
{
	MyShiftArray * array = (MyShiftArray*) (*state);
	shift_array(array->data, array->length);
	
	assert_int_equal(0, array->data[0]);
	assert_int_equal(1, array->data[1]);
	assert_int_equal(2, array->data[2]);
}

static int teardown_shift_array(void ** state)
{
	free(((MyShiftArray*)*state)->data);
	free(*state);
	return 0;
}

/* init_array */
typedef struct {
	int * data;
	int length;
	int init_value;
}MyInitArray;

static int setup_init_array(void ** state)
{
	int i;
	MyInitArray * array = (MyInitArray*) malloc(sizeof(MyInitArray));
	if(array == NULL)
		return -1;
	array->length = 3;
	array->data = (int*) malloc(array->length * sizeof(int));
	if(array->data == NULL)
		return -1;
	for(i=0;i<array->length;++i)
		array->data[i] = i+1;

	array->init_value = 4;

	*state = array;
	return 0;
}

static void test_init_array(void ** state)
{
	MyInitArray * array = (MyInitArray*) (*state);
	init_array(array->data, array->length, array->init_value);
	
	assert_int_equal(4, array->data[0]);
	assert_int_equal(4, array->data[1]);
	assert_int_equal(4, array->data[2]);
}

static int teardown_init_array(void ** state)
{
	free(((MyInitArray*)*state)->data);
	free(*state);
	return 0;
}

/** game_state.c **/
/* getCurrentState */
typedef struct {
	int dx;
	int dy;
	int bird_state;
}MyCurrentState;

static int setup_getCurrentState(void ** state)
{
	MyCurrentState * state_data = (MyCurrentState*) malloc(sizeof(MyCurrentState));
	if(state_data == NULL)
		return -1;
	state_data->dx = 0;
	state_data->dy = 0;
	state_data->bird_state = 0;

	*state = state_data;
	return 0;
}

static void test_getCurrentStateBirdDied(void ** state)
{
	MyCurrentState * state_data = (MyCurrentState*) (*state);
	State * new_state_created = malloc(sizeof(State));

	state_data->dx = 150;
	state_data->dy = 151;
	state_data->bird_state = 0;
	new_state_created = getCurrentState(state_data->dx, state_data->dy, state_data->bird_state);
	assert_int_equal(-1, new_state_created->delta_x);
	assert_int_equal(-1, new_state_created->delta_y);
}

static void test_getCurrentStateBirdAlive(void ** state)
{
	MyCurrentState * state_data = (MyCurrentState*) (*state);
	State * new_state_created = malloc(sizeof(State));

	state_data->dx = 150;
	state_data->dy = 151;
	state_data->bird_state = 1;
	new_state_created = getCurrentState(state_data->dx, state_data->dy, state_data->bird_state);
	assert_int_equal(150, new_state_created->delta_x);
	assert_int_equal(151, new_state_created->delta_y);
}

static void test_getCurrentStateBirdUnknownState(void ** state)
{
	MyCurrentState * state_data = (MyCurrentState*) (*state);
	State * new_state_created = malloc(sizeof(State));

	state_data->dx = 150;
	state_data->dy = 151;
	state_data->bird_state = 2;
	new_state_created = getCurrentState(state_data->dx, state_data->dy, state_data->bird_state);
	assert_null(new_state_created);
}

static int teardown_getCurrentState(void ** state)
{
	free(*state);
	return 0;
}

/* getCurrentReward */
static void test_getCurrentRewardDeathReward(void ** state)
{
	assert_int_equal(DEATH_REWARD, getCurrentReward(-1));
}

static void test_getCurrentRewardLifeReward(void ** state)
{
	assert_int_equal(LIFE_REWARD, getCurrentReward(10));
}

/* processing_dxdy */
static void test_processing_dxdy(void ** state)
{
	assert_int_equal(10/DISTANCE_ACCURACY, processing_dxdy(10));
}

/* processing_birdstate */
static void test_processing_dxdyUn(void ** state)
{
	assert_int_equal(0, processing_birdstate(1));
}

static void test_processing_dxdyZero(void ** state)
{
	assert_int_equal(1, processing_birdstate(0));
}

static void test_processing_dxdyError(void ** state)
{
	assert_int_equal(-1, processing_birdstate(3));
}

int main() {
  const struct CMUnitTest tests_q_learning[] = {
	cmocka_unit_test(test_randomAtMostpositiveLimit),
	cmocka_unit_test(test_randomAtMostzeroLimit),
	cmocka_unit_test(test_powerOfpositiveValue),
	cmocka_unit_test(test_powerOfnegativeValue),
	cmocka_unit_test(test_powerOfzero),
	cmocka_unit_test_setup_teardown(test_shift_array, setup_shift_array, teardown_shift_array),
	cmocka_unit_test_setup_teardown(test_init_array, setup_init_array, teardown_init_array),
	cmocka_unit_test_setup_teardown(test_getCurrentStateBirdDied, setup_getCurrentState, teardown_getCurrentState),
	cmocka_unit_test_setup_teardown(test_getCurrentStateBirdAlive, setup_getCurrentState, teardown_getCurrentState),
	cmocka_unit_test_setup_teardown(test_getCurrentStateBirdUnknownState, setup_getCurrentState, teardown_getCurrentState),
	cmocka_unit_test(test_getCurrentRewardDeathReward),
	cmocka_unit_test(test_getCurrentRewardLifeReward),
	cmocka_unit_test(test_processing_dxdy),
	cmocka_unit_test(test_processing_dxdyUn),
	cmocka_unit_test(test_processing_dxdyZero),
	cmocka_unit_test(test_processing_dxdyError),
  };

  return cmocka_run_group_tests_name("Q-Learning Tests", tests_q_learning, NULL, NULL);
}

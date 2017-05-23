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
/*
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
}*/

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

/** qmatrix.c **/
/* findStateIndex */
typedef struct {
	State * new_state;
	MatrixQ * matrixq;
}State_Matrix;

static int setup_findStateIndex(void ** state)
{
	State_Matrix * new_statematrix = (State_Matrix*) malloc(sizeof(State_Matrix));
	if(new_statematrix == NULL)
		return -1;
	new_statematrix->new_state = (State*) malloc(sizeof(State));
	if(new_statematrix->new_state == NULL)
		return -1;
	new_statematrix->matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	if(new_statematrix->matrixq == NULL)
		return -1;
	
	new_statematrix->new_state->delta_x = 150;
	new_statematrix->new_state->delta_y = 151;
	
	new_statematrix->matrixq->nb_states = 1;
	new_statematrix->matrixq->state = (State*) malloc(new_statematrix->matrixq->nb_states * sizeof(State));
	new_statematrix->matrixq->reward = (float*) malloc(new_statematrix->matrixq->nb_states * NB_ACTIONS * sizeof(float));

	new_statematrix->matrixq->state[0].delta_x = 150;
	new_statematrix->matrixq->state[0].delta_y = 151;
	new_statematrix->matrixq->reward[0] = 1;
	new_statematrix->matrixq->reward[1] = 2;

	*state = new_statematrix;
	return 0;
}

static void test_findStateIndexAlreadyExist(void ** state)
{
	State_Matrix * new_statematrix = (State_Matrix*) (*state);
	assert_int_equal(0, findStateIndex(new_statematrix->new_state, new_statematrix->matrixq));
}

static void test_findStateIndexStateUnknown(void ** state)
{
	State_Matrix * new_statematrix = (State_Matrix*) (*state);
	new_statematrix->new_state->delta_x = 153;

	assert_int_equal(1, findStateIndex(new_statematrix->new_state, new_statematrix->matrixq));
}

static int teardown_findStateIndex(void ** state)
{
	free(((State_Matrix*)*state)->matrixq->state);
	free(((State_Matrix*)*state)->matrixq->reward);
	free(*state);
	return 0;
}

/* addState */
static int setup_addState(void ** state)
{
	State_Matrix * new_statematrix = (State_Matrix*) malloc(sizeof(State_Matrix));
	if(new_statematrix == NULL)
		return -1;
	new_statematrix->new_state = (State*) malloc(sizeof(State));
	if(new_statematrix->new_state == NULL)
		return -1;
	new_statematrix->matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	if(new_statematrix->matrixq == NULL)
		return -1;
	
	new_statematrix->new_state->delta_x = 153;
	new_statematrix->new_state->delta_y = 151;
	
	new_statematrix->matrixq->nb_states = 1;
	new_statematrix->matrixq->state = (State*) malloc(new_statematrix->matrixq->nb_states * sizeof(State));
	new_statematrix->matrixq->reward = (float*) malloc(new_statematrix->matrixq->nb_states * NB_ACTIONS * sizeof(float));

	new_statematrix->matrixq->state[0].delta_x = 150;
	new_statematrix->matrixq->state[0].delta_y = 151;
	new_statematrix->matrixq->reward[0] = 1;
	new_statematrix->matrixq->reward[1] = 2;

	*state = new_statematrix;
	return 0;
}

static void test_addState(void ** state)
{
	State_Matrix * new_statematrix = (State_Matrix*) (*state);

	assert_int_equal(1, addState(new_statematrix->new_state, new_statematrix->matrixq));
}

static int teardown_addState(void ** state)
{
	free(((State_Matrix*)*state)->matrixq->state);
	free(((State_Matrix*)*state)->matrixq->reward);
	free(*state);
	return 0;
}

/* findBestAction */
static int setup_findBestAction(void ** state)
{
	MatrixQ * matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	if(matrixq == NULL)
		return -1;

	matrixq->nb_states = 1;
	matrixq->state = (State*) malloc(matrixq->nb_states * sizeof(State));
	matrixq->reward = (float*) malloc(matrixq->nb_states * NB_ACTIONS * sizeof(float));

	matrixq->state[0].delta_x = 150;
	matrixq->state[0].delta_y = 151;
	matrixq->reward[0] = 0;
	matrixq->reward[1] = 1;

	*state = matrixq;
	return 0;
}

static void test_findBestActionDoNothing(void ** state)
{
	MatrixQ * matrixq = (MatrixQ*) (*state);

	matrixq->reward[0] = 0;
	matrixq->reward[1] = 1;

	assert_int_equal(1, findBestAction(0, matrixq));
}

static void test_findBestActionJump(void ** state)
{
	MatrixQ * matrixq = (MatrixQ*) (*state);

	matrixq->reward[0] = 1;
	matrixq->reward[1] = 0;

	assert_int_equal(0, findBestAction(0, matrixq));
}

static void test_findBestActionEqual(void ** state)
{
	MatrixQ * matrixq = (MatrixQ*) (*state);

	matrixq->reward[0] = 1;
	matrixq->reward[1] = 1;

	assert_in_range(findBestAction(0, matrixq),0,1);
}

static int teardown_findBestAction(void ** state)
{
	free(((MatrixQ*)*state)->state);
	free(((MatrixQ*)*state)->reward);
	free(*state);
	return 0;
}

/* updateQReward */
typedef struct {
	MatrixQ * matrixq;
	int * last_index;
	int * last_action;
}UpdateQValues;

static int setup_updateQReward(void ** state)
{
	int i;

	UpdateQValues * upd_qvalues = (UpdateQValues*) malloc(sizeof(UpdateQValues));
	if(upd_qvalues == NULL)
		return -1;
	upd_qvalues->matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	if(upd_qvalues->matrixq == NULL)
		return -1;
	
	upd_qvalues->matrixq->nb_states = 1;
	upd_qvalues->matrixq->state = (State*) malloc(upd_qvalues->matrixq->nb_states * sizeof(State));
	upd_qvalues->matrixq->reward = (float*) malloc(upd_qvalues->matrixq->nb_states * NB_ACTIONS * sizeof(float));

	upd_qvalues->matrixq->state[0].delta_x = 10;
	upd_qvalues->matrixq->state[0].delta_y = 10;
	upd_qvalues->matrixq->reward[0*NB_ACTIONS] = 100;
	upd_qvalues->matrixq->reward[0*NB_ACTIONS+1] = 100;

	upd_qvalues->last_index = (int*) malloc(NB_SAVED_STATES * sizeof(int));
	upd_qvalues->last_action = (int*) malloc(NB_SAVED_ACTIONS * sizeof(int));
	for(i=0; i<NB_SAVED_STATES; ++i) upd_qvalues->last_index[i] = 0;
	for(i=0; i<NB_SAVED_ACTIONS; ++i) upd_qvalues->last_action[i] = 1;

	*state = upd_qvalues;
	return 0;
}

static void test_updateQRewardPositive(void ** state)
{
	UpdateQValues * upd_qvalues = (UpdateQValues*) (*state);

	updateQReward(upd_qvalues->matrixq, upd_qvalues->last_index, upd_qvalues->last_action);

	assert_in_range((upd_qvalues->matrixq->reward[1]<0)? -(upd_qvalues->matrixq->reward[0]):upd_qvalues->matrixq->reward[0] , 0, HIGHER_QREWARD_LIMIT);
}

static void test_updateQRewardNegative(void ** state)
{
	UpdateQValues * upd_qvalues = (UpdateQValues*) (*state);

	upd_qvalues->last_index[0] = -1;

	updateQReward(upd_qvalues->matrixq, upd_qvalues->last_index, upd_qvalues->last_action);

	assert_in_range((upd_qvalues->matrixq->reward[1]<0)? -(upd_qvalues->matrixq->reward[0]):upd_qvalues->matrixq->reward[0] , 0, LOWER_QREWARD_LIMIT);
}

static int teardown_updateQReward(void ** state)
{
	free(((UpdateQValues*)*state)->matrixq->state);
	free(((UpdateQValues*)*state)->matrixq->reward);
	free(*state);
	return 0;
}

/** q_learning.c **/
/* q_learning_loop */
/*static int setup_q_learning_loop(void ** state)
{
	int i;

	UpdateQValues * upd_qvalues = (UpdateQValues*) malloc(sizeof(UpdateQValues));
	if(upd_qvalues == NULL)
		return -1;
	upd_qvalues->matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	if(upd_qvalues->matrixq == NULL)
		return -1;
	
	upd_qvalues->matrixq->nb_states = 1;
	upd_qvalues->matrixq->state = (State*) malloc(upd_qvalues->matrixq->nb_states * sizeof(State));
	upd_qvalues->matrixq->reward = (float*) malloc(upd_qvalues->matrixq->nb_states * NB_ACTIONS * sizeof(float));

	upd_qvalues->matrixq->state[0].delta_x = 10;
	upd_qvalues->matrixq->state[0].delta_y = 10;
	upd_qvalues->matrixq->reward[0*NB_ACTIONS] = 100;
	upd_qvalues->matrixq->reward[0*NB_ACTIONS+1] = 100;

	upd_qvalues->last_index = (int*) malloc(NB_SAVED_STATES * sizeof(int));
	upd_qvalues->last_action = (int*) malloc(NB_SAVED_ACTIONS * sizeof(int));
	for(i=0; i<NB_SAVED_STATES; ++i) upd_qvalues->last_index[i] = 0;
	for(i=0; i<NB_SAVED_ACTIONS; ++i) upd_qvalues->last_action[i] = 1;

	*state = upd_qvalues;
	return 0;
}

static void test_q_learning_loop(void ** state)
{
	UpdateQValues * upd_qvalues = (UpdateQValues*) (*state);

	upd_qvalues->last_index[0] = 0;

	assert_int_equal(1, q_learning_loop(upd_qvalues->matrixq, upd_qvalues->last_index, upd_qvalues->last_action, 101, 102, 1));
}

static void test_q_learning_loopReset(void ** state)
{
	UpdateQValues * upd_qvalues = (UpdateQValues*) (*state);

	upd_qvalues->last_index[0] = -1;

	assert_int_equal(1, q_learning_loop(upd_qvalues->matrixq, upd_qvalues->last_index, upd_qvalues->last_action, 101, 102, 0));
}

static int teardown_q_learning_loop(void ** state)
{
	free(((UpdateQValues*)*state)->matrixq->state);
	free(((UpdateQValues*)*state)->matrixq->reward);
	free(*state);
	return 0;
}*/

/** file_manager.c **/
/* loadQMatrix */
static int setup_loadQMatrixEmpty(void ** state) {

	char * filename = "test_qmatrixdata.txt";
	*state = filename;
	return 0;
}

static int setup_loadQMatrix(void ** state) {
	
	MatrixQ * matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	char * filename = "test_qmatrixdata.txt";
	if(matrixq == NULL)
		return -1;

	matrixq->nb_states = 1;
	matrixq->state = (State*) malloc(matrixq->nb_states * sizeof(State));
	matrixq->reward = (float*) malloc(matrixq->nb_states * NB_ACTIONS * sizeof(float));

	matrixq->state[0].delta_x = 10;
	matrixq->state[0].delta_y = 10;
	matrixq->reward[0*NB_ACTIONS] = 100;
	matrixq->reward[0*NB_ACTIONS+1] = 100;

	saveQMatrix(matrixq, filename);	
	
	*state = filename;
	return 0;
}

static void test_loadQMatrixEmpty(void ** state) {

	MatrixQ * matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	matrixq = loadQMatrix((*state));
	assert_int_equal(0, matrixq->nb_states);
}

static void test_loadQMatrix(void ** state) {

	MatrixQ * matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	matrixq = loadQMatrix((*state));
	assert_int_equal(1, matrixq->nb_states);
}

static int teardown_loadQMatrix(void ** state) {

  return remove("test_qmatrixdata.txt");
}

/* saveQMatrix */
static int setup_saveQMatrix(void ** state) {

	MatrixQ * matrixq = (MatrixQ*) malloc(sizeof(MatrixQ));
	if(matrixq == NULL)
		return -1;

	matrixq->nb_states = 1;
	matrixq->state = (State*) malloc(matrixq->nb_states * sizeof(State));
	matrixq->reward = (float*) malloc(matrixq->nb_states * NB_ACTIONS * sizeof(float));

	matrixq->state[0].delta_x = 10;
	matrixq->state[0].delta_y = 10;
	matrixq->reward[0*NB_ACTIONS] = 100;
	matrixq->reward[0*NB_ACTIONS+1] = 100;

	*state = matrixq;
	return 0;
}

static void test_saveQMatrix(void ** state) {

	MatrixQ * matrixq = (MatrixQ*) (*state);
	assert_int_equal(1, saveQMatrix(matrixq, "test_qmatrixdata.txt"));
}

static int teardown_saveQMatrix(void ** state) {

  return remove("test_qmatrixdata.txt");
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
	/*cmocka_unit_test_setup_teardown(test_getCurrentStateBirdDied, setup_getCurrentState, teardown_getCurrentState),
	cmocka_unit_test_setup_teardown(test_getCurrentStateBirdAlive, setup_getCurrentState, teardown_getCurrentState),
	cmocka_unit_test_setup_teardown(test_getCurrentStateBirdUnknownState, setup_getCurrentState, teardown_getCurrentState),*/
	cmocka_unit_test(test_getCurrentRewardDeathReward),
	cmocka_unit_test(test_getCurrentRewardLifeReward),
	cmocka_unit_test(test_processing_dxdy),
	cmocka_unit_test(test_processing_dxdyUn),
	cmocka_unit_test(test_processing_dxdyZero),
	cmocka_unit_test(test_processing_dxdyError),
	cmocka_unit_test_setup_teardown(test_findStateIndexAlreadyExist, setup_findStateIndex, teardown_findStateIndex),
	cmocka_unit_test_setup_teardown(test_findStateIndexStateUnknown, setup_findStateIndex, teardown_findStateIndex),
	cmocka_unit_test_setup_teardown(test_findStateIndexStateUnknown, setup_findStateIndex, teardown_findStateIndex),
	cmocka_unit_test_setup_teardown(test_addState, setup_addState, teardown_addState),
	cmocka_unit_test_setup_teardown(test_findBestActionDoNothing, setup_findBestAction, teardown_findBestAction),
	cmocka_unit_test_setup_teardown(test_findBestActionJump, setup_findBestAction, teardown_findBestAction),
	cmocka_unit_test_setup_teardown(test_findBestActionEqual, setup_findBestAction, teardown_findBestAction),
	cmocka_unit_test_setup_teardown(test_updateQRewardPositive, setup_updateQReward, teardown_updateQReward),
	cmocka_unit_test_setup_teardown(test_updateQRewardNegative, setup_updateQReward, teardown_updateQReward),
	/*cmocka_unit_test_setup_teardown(test_q_learning_loop, setup_q_learning_loop, teardown_q_learning_loop),
	cmocka_unit_test_setup_teardown(test_q_learning_loopReset, setup_q_learning_loop, teardown_q_learning_loop),*/
	cmocka_unit_test_setup_teardown(test_loadQMatrix, setup_loadQMatrix, teardown_loadQMatrix),
	cmocka_unit_test_setup_teardown(test_loadQMatrixEmpty, setup_loadQMatrixEmpty, teardown_loadQMatrix),
	cmocka_unit_test_setup_teardown(test_saveQMatrix, setup_saveQMatrix, teardown_saveQMatrix),
  };

  return cmocka_run_group_tests_name("Q-Learning Tests", tests_q_learning, NULL, NULL);
}

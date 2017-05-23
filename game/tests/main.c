#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/constants.h"
#include "../src/bird.h"
#include "../src/camera.h"
#include "../src/file.h"
//#include "../src/game.h"
#include "../src/obstacle.h"
#include "../src/pipe.h"
#include "../../ai/neat/src/generic_list.h"

/* initBird */

typedef struct{
    Genome * genome;
    Species * species;
}InitBirdStruct;

static int setup_initBird(void ** state) {
  InitBirdStruct * structure = malloc(sizeof(InitBirdStruct));
  structure->genome = malloc(sizeof(Genome));
  structure->species = malloc(sizeof(Species));

  if (structure->genome == (Genome *) NULL || structure->species == (Species *) NULL)
    return -1;

  *state = structure;

  return 0;
}

static void test_initBird(void ** state) {
  Genome * genome = (Genome *) ((InitBirdStruct *)* state)->genome;
  Species * species = (Species *) ((InitBirdStruct *)*state)->species;
  Bird * bird = initBird(genome, species);

  assert_int_equal(bird->x, BIRD_X_OFFSET);
  assert_int_equal(bird->y, SCREEN_HEIGHT / 2);
  assert_int_equal(bird->w, BIRD_SIZE);
  assert_int_equal(bird->h, BIRD_SIZE);
  assert_int_equal(bird->dir_y, 0);
  assert_int_equal(bird->dead, 0);
  assert_int_equal(bird->flaps, 0);
  assert_int_equal(bird->score, 0);
  assert_int_equal(bird->genome, genome);
  assert_int_equal(bird->species, species);
}

static int teardown_initBird(void ** state) {
  free(*state);
  return 0;
}

/* updateBird */

typedef struct{
    Bird * bird;
    Sound * sound;
}UpdateBirdStruct;

static int setup_updateBird(void ** state) {
    UpdateBirdStruct * structure = malloc(sizeof(UpdateBirdStruct));
    structure->bird = initBird(NULL, NULL);
    structure->sound = malloc(sizeof(Sound));

  if (structure->bird == (Bird *) NULL || structure->sound == NULL)
    return -1;

  *state = structure;

  return 0;
}

static void test_updateBirdNothing(void ** state) {
    UpdateBirdStruct * structure = (UpdateBirdStruct *)(* state);

  int y = structure->bird->y;
  int dir_y = structure->bird->dir_y;

  updateBird((Bird *)((UpdateBirdStruct *)*state)->bird, NOTHING, (Sound *)((UpdateBirdStruct *)*state)->sound);

  assert_int_equal(structure->bird->dir_y, dir_y + GRAVITY);
  assert_int_equal(structure->bird->y, y + dir_y + GRAVITY);
}

static void test_updateBirdNothingMaxFallSpeed(void ** state) {
  int i;
  Bird * bird = (Bird *) ((UpdateBirdStruct *)* state)->bird;


  for (i = 0; i < 100; ++i)
    updateBird(bird, NOTHING, (Sound *)((UpdateBirdStruct *)*state)->sound);

  assert_int_equal(bird->dir_y, BIRD_MAX_FALL_SPEED);
  assert_int_equal(bird->y, SCREEN_HEIGHT - BIRD_SIZE / 2);
}

static void test_updateBirdJump(void ** state) {
  Bird * bird = (Bird *) ((UpdateBirdStruct *)* state)->bird;
  Sound * sound = (Sound *)((UpdateBirdStruct *)* state)->sound;
  int flaps = bird->flaps;

  int y = bird->y;

  updateBird(bird, JUMP, (Sound *)((UpdateBirdStruct *)*state)->sound);

  assert_int_equal(bird->dir_y, BIRD_JUMP);
  assert_int_equal(bird->y, y + BIRD_JUMP);
  assert_int_equal(bird->flaps, flaps + 1);
  assert_int_equal(*sound, JUMPSOUND);
}

/*static void test_updateBirdJumpMaxHeight(void ** state) {
  int i;
  Bird * bird = (Bird *) ((UpdateBirdStruct *)* state)->bird;
  Sound * sound = (Sound *)((UpdateBirdStruct *)* state)->sound;
  int flaps = bird->flaps;

  for (i = 0; i < 100; ++i)
    updateBird(bird, JUMP, (Sound *)((UpdateBirdStruct *)*state)->sound);

  assert_int_equal(bird->dir_y, BIRD_JUMP);
  assert_int_equal(bird->y, BIRD_SIZE / 2);
  assert_int_equal(bird->flaps, flaps + 100);
  assert_int_equal(*sound, JUMPSOUND);
}*/

static int teardown_updateBird(void ** state) {
  free(((UpdateBirdStruct *)*state)->bird);
  free(((UpdateBirdStruct *)*state)->sound);
  free(*state);
  return 0;
}


/* FILE */


/* readLevel */

static int setup_readLevelEmpty(void ** state) {
  FILE * file = fopen("test_readLevel.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  *state = file;

  return 0;
}

static int setup_readLevel(void ** state) {
  FILE * file = fopen("test_readLevel.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  fputs("200\n", file);
  fputs("201\n", file);
  fputs("202\n", file);
  fputs("203\n", file);
  fputs("204\n", file);
  fputs("205\n", file);
  fputs("206\n", file);
  fputs("207\n", file);
  fputs("208\n", file);
  fputs("209\n", file);

  *state = file;

  return 0;
}

static void test_readLevel(void ** state) {
  FILE * file = (FILE *) (* state);

  assert_int_equal(readLevel(file, 0), 200);
  assert_int_equal(readLevel(file, 1), 201);
  assert_int_equal(readLevel(file, 2), 202);
  assert_int_equal(readLevel(file, 3), 203);
  assert_int_equal(readLevel(file, 4), 204);
  assert_int_equal(readLevel(file, 5), 205);
  assert_int_equal(readLevel(file, 6), 206);
  assert_int_equal(readLevel(file, 7), 207);
  assert_int_equal(readLevel(file, 8), 208);
  assert_int_equal(readLevel(file, 9), 209);
}

static void test_readLevelOut(void ** state) {
  FILE * file = (FILE *) (* state);

  assert_int_equal(readLevel(file, 150), 209);
}

static void test_readLevelEmpty(void ** state) {
  FILE * file = (FILE *) (* state);

  assert_int_equal(readLevel(file, 0), 150);
}

static int teardown_readLevel(void ** state) {
  fclose(*state);
  return remove("test_readLevel.txt");
}

/* readConfig */

static int setup_readConfigEmpty(void ** state) {
  FILE * file = fopen("test_readConfig.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  *state = file;

  return 0;
}

static int setup_readConfig(void ** state) {
  FILE * file = fopen("test_readConfig.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  fputs("level :\n", file);
  fputs("../../res/files/level.txt\n", file);

  *state = file;

  return 0;
}

static void test_readConfigEmpty(void ** state) {
  FILE * file = (FILE *) (* state);

  char * path = malloc(sizeof(char) * 100);

  assert_int_equal(readConfig(file, path, "level :\n"), 0);
  
  free(path);
}

static void test_readConfig(void ** state) {
  FILE * file = (FILE *) (* state);

  char * path = malloc(sizeof(char) * 100);

  assert_int_equal(readConfig(file, path, "level :\n"), 1);
  assert_string_equal(path, "../../res/files/level.txt");
  
  free(path);
}

static int teardown_readConfig(void ** state) {
  fclose(*state);
  return remove("test_readConfig.txt");
}

/* saveScore */

static int setup_saveScore(void ** state) {
  FILE * file = fopen("test_saveScore.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  *state = file;

  return 0;
}

static int setup_saveScoreLowScore(void ** state) {
  FILE * file = fopen("test_saveScore.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  fputs("1", file);

  *state = file;

  return 0;
}

static int setup_saveScoreHighScore(void ** state) {
  FILE * file = fopen("test_saveScore.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  fputs("1000000", file);

  *state = file;

  return 0;
}

static void test_saveScore(void ** state) {
  FILE * file = (FILE *) (* state);

  assert_int_equal(saveScore(file, 15000), 1);
}

static void test_saveScoreHighScore(void ** state) {
  FILE * file = (FILE *) (* state);

  assert_int_equal(saveScore(file, 15000), 0);
}

static int teardown_saveScore(void ** state) {
  fclose(*state);
  return remove("test_saveScore.txt");
}

/* readBestScore */

static int setup_readBestScoreEmpty(void ** state) {
  FILE * file = NULL;

  *state = file;

  return 0;
}

static int setup_readBestScore(void ** state) {
  FILE * file = fopen("test_readBestScore.txt", "w+");

  if (file == (FILE *) NULL)
    return -1;

  fputs("69", file);

  *state = file;

  return 0;
}

static void test_readBestScoreEmpty(void ** state) {
  FILE * file = (FILE *) (* state);

  assert_int_equal(readBestScore(file), -1);
}

static void test_readBestScore(void ** state) {
  FILE * file = (FILE *) (* state);
  remove("test_readBestScore.txt");

  assert_int_equal(readBestScore(file), 69);
}

static int teardown_readBestScore(void ** state) {
  fclose(*state);
  return remove("test_readBestScore.txt");
}

/* openGameFiles */

typedef struct {
	FILE * config;
	FILE * score;
	FILE * level;
} Files;

static int setup_openGameFiles(void ** state) {
  FILE * config = fopen("test_openGameFiles_config.txt", "w+");
	FILE * level = fopen("pathToLevelFile.txt", "w+");
	FILE * score = fopen("pathToScoreFile.txt", "w+");

  if (config == (FILE *) NULL){
    fprintf(stderr, "error1\n");
    return -1;
  }
  if (score == (FILE *) NULL){
    fprintf(stderr, "error2\n");
    return -1;
  }
  if (level == (FILE *) NULL){
    fprintf(stderr, "error3\n");
    return -1;
  }

  fputs("level :\n", config);
	fputs("pathToLevelFile.txt", config);
	fputs("\n", config);
	fputs("score :\n", config);
	fputs("pathToScoreFile.txt", config);
	fputs("\n", config);

	Files * files = NULL;
  if ((files = (Files *) malloc(sizeof(Files))) == (Files *) NULL){
  	fprintf(stderr, "error4\n");
    return -1;
  }
	files->config = config;
	files->score = score;
	files->level = level;

  *state = files;

  return 0;
}

static void test_openGameFiles(void ** state) {
  Files * files = (Files *) (* state);

  FILE * test_score = NULL;
  FILE * test_level = NULL;

  assert_int_equal(openGameFiles(files->config, &test_level, &test_score), 1);
}

static int teardown_openGameFiles(void ** state) {
  Files * files = (Files *) (* state);
  fclose(files->config);
  fclose(files->level);
  fclose(files->score);
  remove("pathToLevelFile.txt");
  remove("pathToScoreFile.txt");
  remove("test_openGameFiles_config.txt");
  free(*state);

  return 0;
}


/* CAMERA */


/* initCamera */

static int setup_initCamera(void ** state) {
  Camera * camera = malloc(sizeof(Camera));

  if (camera == (Camera *) NULL)
    return -1;

  *state = camera;

  return 0;
}

static void test_initCamera(void ** state) {
  Camera * camera = (Camera *) (* state);
  initCamera(camera, 0, 15000);

  assert_int_equal(camera->x, 0);
  assert_int_equal(camera->y, 0);
  assert_int_equal(camera->w, SCREEN_WIDTH);
  assert_int_equal(camera->h, SCREEN_HEIGHT);
  assert_int_equal(camera->speed, 15000);
}

static int teardown_initCamera(void ** state) {
  free(*state);
  return 0;
}

/* modifySpeed */

static int setup_modifySpeed(void ** state) {
  Camera * camera = malloc(sizeof(Camera));

  if (camera == (Camera *) NULL)
    return -1;

  *state = camera;

  return 0;
}

static void test_modifySpeed(void ** state) {
  Camera * camera = (Camera *) (* state);

	modifySpeed(10, camera);
  assert_int_equal(camera->speed, LOW);

 	modifySpeed(30, camera);
	assert_int_equal(camera->speed, NORMAL);

	modifySpeed(50, camera);
	assert_int_equal(camera->speed, HIGH);

	modifySpeed(70, camera);
	assert_int_equal(camera->speed, EXTREME);
}

static int teardown_modifySpeed(void ** state) {
  free(*state);
  return 0;
}


/* GAME */


/* cameraScrolling */

/*static int setup_cameraScrolling(void ** state_1, void ** state_2) {
  Camera * camera = NULL;
  Bird * bird = NULL;

  if ((camera = malloc(sizeof(Camera))) == (Camera *) NULL)
    return -1;

  initCamera(camera, 0, 100);

  *state_1 = camera;

  if ((bird = malloc(sizeof(Bird))) == (Bird *) NULL)
    return -1;

  initBird(bird);

  *state_2 = bird;

  return 0;
}

static void test_cameraScrolling(void ** state_1, void ** state_2) {
  Camera * camera = (Camera *) (* state_1);
  Bird * bird = (Bird *) (* state_2);

  int camera_x = camera->x;
  int bird_x = bird->x;

  cameraScrolling(camera, bird);

  assert_int_equal(camera->x, camera_x + 100);
  assert_int_equal(bird->x, bird_x + 100);
}

static int teardown_cameraScrolling(void ** state_1, void ** state_2) {
  free(*state_1);
  free(*state_2);
  return 0;
}*/


/* OBSTACLE */


/* newObstacle */

static int setup_newObstacle(void ** state) {
  Obstacle * obstacle = NULL;

  *state = obstacle;

  return 0;
}

static void test_newObstacle(void ** state) {
  Obstacle * obstacle = (Obstacle *) (* state);

  obstacle = newObstacle(14, 200, 250, NULL);

  assert_non_null(&obstacle->lower);
  assert_non_null(&obstacle->upper);
  assert_int_equal(obstacle->lower.y, SCREEN_HEIGHT-200);
  assert_int_equal(obstacle->gap, 250);
  assert_null(obstacle->next);
}

static void test_newObstacleNegativeGap(void ** state) {
  Obstacle * obstacle = (Obstacle *) (* state);

  obstacle = newObstacle(14, 120, -15, NULL);

	assert_int_equal(obstacle, NULL);
}

static void test_newObstacleMaxHeight(void ** state) {
  Obstacle * obstacle = (Obstacle *) (* state);

  obstacle = newObstacle(14, SCREEN_HEIGHT, 250, NULL);

	assert_int_equal(obstacle, NULL);
}

static int teardown_newObstacle(void ** state) {
  free(*state);
  return 0;
}

/* nextBirdObstacle */

typedef struct{
	GenericList * list;
	Bird * bird1;
	Bird * bird2;
}NextBirdObstacleStruct;

static int setup_nextBirdObstacle(void ** state){
	Obstacle * obstacle4 = newObstacle(4, 400, MEDIUM, NULL);
	Obstacle * obstacle3 = newObstacle(3, 100, MEDIUM, obstacle4);
	Obstacle * obstacle2 = newObstacle(2, 300, MEDIUM, obstacle3);
	Obstacle * obstacle1 = newObstacle(1, 200, MEDIUM, obstacle2);
	
	GenericList * list = newGenericList(NULL, freeObstacle);
  if (list == (GenericList *) NULL)
    return -1;
	
	initGenericList(list);
	
	add(list, (Obstacle *)obstacle1);
	add(list, (Obstacle *)obstacle2);
	add(list, (Obstacle *)obstacle3);
	add(list, (Obstacle *)obstacle4);	
	
	Bird * bird1 = initBird(NULL, NULL);
	bird1->x = 2000;													/* Bird between two obstacles */
	Bird * bird2 = initBird(NULL, NULL);
	bird2->x = 5000;													/* Bird after all the obstacles of the list */
		
	NextBirdObstacleStruct * structure = malloc(sizeof(NextBirdObstacleStruct));
	structure->list = list;
	structure->bird1 = bird1;
	structure->bird2 = bird2;
	
	*state = structure;
	
	return 0;
}

static void test_nextBirdObstacle(void ** state){
	NextBirdObstacleStruct * structure = (NextBirdObstacleStruct *) (* state);
	
	Obstacle * next_bird_obstacle = nextBirdObstacle(structure->list, structure->bird1);
	setOn(structure->list, 2);
	assert_ptr_equal(next_bird_obstacle, getCurrent(structure->list));
	
	Obstacle * null_obstacle = nextBirdObstacle(structure->list, structure->bird2);
	assert_ptr_equal(null_obstacle, NULL);
}

static int teardown_nextBirdObstacle(void ** state) {
	freeGenericList(((NextBirdObstacleStruct *)(*state))->list);
	freeBird(((NextBirdObstacleStruct *)(*state))->bird1);
	freeBird(((NextBirdObstacleStruct *)(*state))->bird2);
  free(*state);
  return 0;
}

/* createObstacleFromFile */

typedef struct{
	GenericList * list;
	FILE * file;
} createObstacleFromFileStruct;

static int setup_createObstacleFromFile(void ** state){
	FILE * level_file = fopen("level.txt", "w+");
  if (level_file == (FILE *) NULL)
    return -1;

	fputs("200\n", level_file);
	fputs("300\n", level_file);
	fputs("400\n", level_file);
	fputs("500\n", level_file);
	
	GenericList * list = newGenericList(NULL, freeObstacle);
  if (list == (GenericList *) NULL)
    return -1;
	
	initGenericList(list);
	
	Obstacle * obstacle2 = newObstacle(1, 300, MEDIUM, NULL);
	Obstacle * obstacle1 = newObstacle(0, 200, MEDIUM, obstacle2);
	
	add(list, (Obstacle *)obstacle1);
	add(list, (Obstacle *)obstacle2);
		
	createObstacleFromFileStruct * structure = malloc(sizeof(createObstacleFromFileStruct));
	if (structure == (createObstacleFromFileStruct *) NULL)
    return -1;
   
  structure->file = level_file;
  structure->list = list;
   
  *state = structure;

  return 0;
}

static void test_createObstacleFromFile(void ** state){
	createObstacleFromFileStruct * structure = (createObstacleFromFileStruct *) (* state);
	createObstacleFromFile(structure->file, 2, structure->list);
	setOn(structure->list, 2);
	assert_int_equal(((Obstacle *)getCurrent(structure->list))->lower.h, 400);
}

static int teardown_createObstacleFromFile(void ** state){
	createObstacleFromFileStruct * structure = (createObstacleFromFileStruct *) (* state);
	freeGenericList(structure->list);
	fclose(structure->file);
  free(*state);
  remove("level.txt");
  return 0;
}


/* PIPE */


/* initPipe */

static int setup_initPipe(void ** state) {
  Pipe * pipe = malloc(sizeof(Pipe));

  if (pipe == (Pipe *) NULL)
    return -1;

  *state = pipe;

  return 0;
}

static void test_initPipe(void ** state) {
  Pipe * pipe = (Pipe *) (* state);

  initPipe(pipe, 10, 100, 200);

  assert_int_equal(pipe->x, 10 * PIPE_X_OFFSET + SCREEN_WIDTH);
  assert_int_equal(pipe->y, 100);
  assert_int_equal(pipe->w, PIPE_WIDTH);
  assert_int_equal(pipe->h, 200);
}

static int teardown_initPipe(void ** state) {
  free(*state);
  return 0;
}

int main() {
  const struct CMUnitTest tests[] = {
		/* Bird */
    cmocka_unit_test_setup_teardown(test_initBird, setup_initBird, teardown_initBird),

    cmocka_unit_test_setup_teardown(test_updateBirdNothing, setup_updateBird, teardown_updateBird),
    cmocka_unit_test_setup_teardown(test_updateBirdNothingMaxFallSpeed, setup_updateBird, teardown_updateBird),
    cmocka_unit_test_setup_teardown(test_updateBirdJump, setup_updateBird, teardown_updateBird),
    /*cmocka_unit_test_setup_teardown(test_updateBirdJumpMaxHeight, setup_updateBird, teardown_updateBird),*/


		/* File */
    cmocka_unit_test_setup_teardown(test_readLevelEmpty, setup_readLevelEmpty, teardown_readLevel),
    cmocka_unit_test_setup_teardown(test_readLevel, setup_readLevel, teardown_readLevel),
    cmocka_unit_test_setup_teardown(test_readLevelOut, setup_readLevel, teardown_readLevel),

    cmocka_unit_test_setup_teardown(test_readConfigEmpty, setup_readConfigEmpty, teardown_readConfig),
    cmocka_unit_test_setup_teardown(test_readConfig, setup_readConfig, teardown_readConfig),

    cmocka_unit_test_setup_teardown(test_saveScore, setup_saveScore, teardown_saveScore),
    cmocka_unit_test_setup_teardown(test_saveScore, setup_saveScoreLowScore, teardown_saveScore),
    cmocka_unit_test_setup_teardown(test_saveScoreHighScore, setup_saveScoreHighScore, teardown_saveScore),

		//Could not run the test - check test fixtures
		//cmocka_unit_test_setup_teardown(test_readBestScoreEmpty, setup_readBestScoreEmpty, teardown_readBestScore),
		cmocka_unit_test_setup(test_readBestScore, setup_readBestScore),

		cmocka_unit_test_setup_teardown(test_openGameFiles, setup_openGameFiles, teardown_openGameFiles),

		/* Camera */
		cmocka_unit_test_setup_teardown(test_initCamera, setup_initCamera, teardown_initCamera),

		cmocka_unit_test_setup_teardown(test_modifySpeed, setup_modifySpeed, teardown_modifySpeed),

		/* Game */
		//cmocka_unit_test_setup_teardown(test_cameraScrolling, setup_cameraScrolling, teardown_cameraScrolling),

		/* Obstacle */
    cmocka_unit_test_setup_teardown(test_newObstacle, setup_newObstacle, teardown_newObstacle),
    cmocka_unit_test_setup_teardown(test_newObstacleNegativeGap, setup_newObstacle, teardown_newObstacle),
    cmocka_unit_test_setup_teardown(test_newObstacleMaxHeight, setup_newObstacle, teardown_newObstacle),
    
    cmocka_unit_test_setup_teardown(test_nextBirdObstacle, setup_nextBirdObstacle, teardown_nextBirdObstacle),
    
    cmocka_unit_test_setup_teardown(test_createObstacleFromFile, setup_createObstacleFromFile, teardown_createObstacleFromFile),

		/* Pipe */
    cmocka_unit_test_setup_teardown(test_initPipe, setup_initPipe, teardown_initPipe),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

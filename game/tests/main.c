#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/constants.h"
#include "../src/bird.h"
#include "../src/camera.h"
#include "../src/obstacle.h"

/* initBird */

static int setup_initBird(void ** state) {
  Bird * bird = malloc(sizeof(Bird));

  if (bird == (Bird *) NULL)
    return -1;

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

/* updateBird */

static int setup_updateBird(void ** state) {
  Bird * bird = malloc(sizeof(Bird));

  if (bird == (Bird *) NULL)
    return -1;

  *state = bird;
  initBird(*state);

  return 0;
}

static void test_updateBirdNothing(void ** state) {
  Bird * bird = (Bird *) (* state);

  int y = bird->y;
  int dir_y = bird->dir_y;

  updateBird(*state, NOTHING);

  assert_int_equal(bird->dir_y, dir_y + GRAVITY);
  assert_int_equal(bird->y, y + dir_y + GRAVITY);
}

static void test_updateBirdNothingMaxFallSpeed(void ** state) {
  int i;
  Bird * bird = (Bird *) (* state);

  for (i = 0; i < 100; ++i)
    updateBird(*state, NOTHING);

  assert_int_equal(bird->dir_y, BIRD_MAX_FALL_SPEED);
  assert_int_equal(bird->y, SCREEN_HEIGHT - BIRD_SIZE / 2);
}

static void test_updateBirdJump(void ** state) {
  Bird * bird = (Bird *) (* state);

  int y = bird->y;

  updateBird(*state, JUMP);

  assert_int_equal(bird->dir_y, BIRD_JUMP);
  assert_int_equal(bird->y, y + BIRD_JUMP);
}

static void test_updateBirdJumpMaxHeight(void ** state) {
  int i;
  Bird * bird = (Bird *) (* state);

  for (i = 0; i < 100; ++i)
    updateBird(*state, JUMP);

  assert_int_equal(bird->dir_y, BIRD_JUMP);
  assert_int_equal(bird->y, BIRD_SIZE / 2);
}

static int teardown_updateBird(void ** state) {
  free(*state);
  return 0;
}

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

static void test_readConfig(void ** state) {
  FILE * file = (FILE *) (* state);

  char * path = malloc(sizeof(char) * 100);

  assert_int_equal(readConfig(file, path, "level :\n"), 1);
  assert_string_equal(path, "../../res/files/level.txt");
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
  free(*state);
  return remove("test_saveScore.txt");
}

/* newObstacle */

static int setup_newObstacle(void ** state) {
  Obstacle * obstacle = NULL;

  *state = obstacle;

  return 0;
}

static void test_newObstacle(void ** state) {
  Obstacle * obstacle = (Obstacle *) (* state);

  obstacle = newObstacle(14, 120, 15, NULL);

  assert_non_null(&obstacle->lower);
  assert_non_null(&obstacle->upper);
  assert_int_equal(obstacle->upper.y, 120);
  assert_int_equal(obstacle->gap, 15);
  assert_null(obstacle->next);
}

static int teardown_newObstacle(void ** state) {
  free(*state);
  return 0;
}

static void test_newObstacleNegativeGap(void ** state) {
  Obstacle * obstacle = (Obstacle *) (* state);

  obstacle = newObstacle(14, 120, -15, NULL);

  assert_non_null(&obstacle->lower);
  assert_non_null(&obstacle->upper);
  assert_int_equal(obstacle->upper.y, 120);
  assert_int_equal(obstacle->gap, 0);
  assert_null(obstacle->next);
}

static void test_newObstacleMaxHeight(void ** state) {
  Obstacle * obstacle = (Obstacle *) (* state);

  obstacle = newObstacle(14, SCREEN_HEIGHT, 15, NULL);

  assert_non_null(&obstacle->lower);
  assert_non_null(&obstacle->upper);
  assert_int_equal(obstacle->upper.y, SCREEN_HEIGHT / 2);
  assert_int_equal(obstacle->gap, 15);
  assert_null(obstacle->next);
}

int main() {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test_setup_teardown(test_initBird, setup_initBird, teardown_initBird),

    cmocka_unit_test_setup_teardown(test_updateBirdNothing, setup_updateBird, teardown_updateBird),
    cmocka_unit_test_setup_teardown(test_updateBirdNothingMaxFallSpeed, setup_updateBird, teardown_updateBird),
    cmocka_unit_test_setup_teardown(test_updateBirdJump, setup_updateBird, teardown_updateBird),
    cmocka_unit_test_setup_teardown(test_updateBirdJumpMaxHeight, setup_updateBird, teardown_updateBird),

    cmocka_unit_test_setup_teardown(test_initCamera, setup_initCamera, teardown_initCamera),

    cmocka_unit_test_setup_teardown(test_readLevelEmpty, setup_readLevelEmpty, teardown_readLevel),
    cmocka_unit_test_setup_teardown(test_readLevel, setup_readLevel, teardown_readLevel),
    cmocka_unit_test_setup_teardown(test_readLevelOut, setup_readLevel, teardown_readLevel),

    cmocka_unit_test_setup_teardown(test_readConfig, setup_readConfigEmpty, teardown_readConfig),
    cmocka_unit_test_setup_teardown(test_readConfig, setup_readConfig, teardown_readConfig),

    cmocka_unit_test_setup_teardown(test_saveScore, setup_saveScore, teardown_saveScore),
    cmocka_unit_test_setup_teardown(test_saveScore, setup_saveScoreLowScore, teardown_saveScore),
    cmocka_unit_test_setup_teardown(test_saveScoreHighScore, setup_saveScoreHighScore, teardown_saveScore),

    cmocka_unit_test_setup_teardown(test_newObstacle, setup_newObstacle, teardown_newObstacle),
    cmocka_unit_test_setup_teardown(test_newObstacleNegativeGap, setup_newObstacle, teardown_newObstacle),
    cmocka_unit_test_setup_teardown(test_newObstacleMaxHeight, setup_newObstacle, teardown_newObstacle),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

/*!
* \file file.h
* \brief File containing the prototypes to manage the extern files
*/
#ifndef FILE_H
#define FILE_H

#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int readLevel(FILE * f, int number);
int readConfig(FILE * f, char * config, char * type);
int saveScore(FILE * f, int score);
int readBestScore(FILE * f);

#endif // FILE_H

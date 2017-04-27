/*!
* \file file.c
* \brief File containing the functions to manage the extern files
*/
#include "file.h"

/*!
* \brief Read the predefined level from a file
* \param[out] file the file to read
* \param[in] number the number of the obstacle read
* \return Return the height of the low pipe of the obstacle ; 150 if the file is NULL or if nothing is read
*
* The height of an obstacle is contained in a line, then  go to line to have the next height
*/
int readLevel(FILE * file, int number)
{
    int heightPipe = 0, i = 0;
    fseek(file, sizeof(int), SEEK_SET);
    if (fscanf(file, "%d", &heightPipe) == EOF)
    	return 150;
    fseek(file, 0, SEEK_SET);
    while (i <= number)
    {
        fscanf(file, "%d", &heightPipe);
        ++i;
    }
    return heightPipe;
}

/*!
* \brief Read the configuration file
* \param[out] f the configuration file to read
* \param[out] config the path of the requested parameter
* \param[in] type the requested parameter (examples : level, sprite, sound...)
* \return Return 1 if the configuration file was read, ie the requested path is set in config ; 0 if error
*
* The configuration file is written such as :
*       type :\n
*       config
*/
int readConfig(FILE * f, char * config, char * type)
{
    int i = 0;
    char string[100];
    fseek(f, 0, SEEK_SET);
    while(i < 50)
    {
        fgets(string, sizeof(string), f);
        if(strcmp(string, type) == 0)
        {
            fgets(string, sizeof(string), f);
            strcpy(config, string);
            if(config[strlen(config)-1] == '\n')
            	config[strlen(config)-1] = '\0';
            return 1;
        }
        ++i;
    }
    fprintf(stderr, "Reading the configuration file : failure\n");
    return 0;
}

/*!
* \brief Save the best score in a file
* \param[out] f the file that contains the score
* \param[in] score the score to be saved (or not)
* \return Return 1 if the score was saved, ie if it is biggest than the previous score, 0 otherwise
*/
int saveScore(FILE * f, int score)
{
    int savedScore;
    fseek(f, 0, SEEK_SET);
    if((fscanf(f, "%d", &savedScore) < 1) || (savedScore < score)){
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", score);
        return 1;
    }
    return 0;
}

/*!
* \brief Read the best score in a file
* \param[in] f the file that contains the score
* \return Return the best score saved, -1 if failure
*/
int readBestScore(FILE * f)
{
    if (f == NULL)
    {
        fprintf(stderr, "Reading best score failure");
        return -1;
    }
    int best_score = 0;
    fseek(f, 0, SEEK_SET);
    fscanf(f, "%d", &best_score);
    return best_score;

}

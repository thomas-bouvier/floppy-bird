/*!
* \file file.c
* \brief File containing the functions to manage the extern files
*/
#include "file.h"

/*!
* \brief Read the predefined level from a file
* \param[out] file the file to read
* \param[in] number the number of the obstacle read
* \return Return the height of the low pipe of the obstacle
*/
int readLevel(FILE * file, int number)
{
    int heightPipe = 0, i = 0;
    fseek(file, sizeof(int), SEEK_SET);
    while (i < number)
    {
        fscanf(file, "%d", &heightPipe);
        ++i;
    }
    return heightPipe;
}

/*!
* \brief Read the configuration file
* \param[out] file the configuration file to read
* \param[out] config the path of the requested parameter
* \param[in] type the requested parameter (examples : level, sprite, sound...)
* \return Return 1 if the configuration file was read, ie the requested path is set in config ; 0 if error
*/
int readConfig(FILE * f, char * config, char * type)
{
    int i = 0;
    char string[50];
    fseek(f, 0, SEEK_SET);
    while(1)
    {
        fgets(string, sizeof(string), f);
        if(strcmp(string, type) == 0)
        {
            fgets(string, sizeof(string), f);
            strcpy(config, string);
            return 1;
        }
        if(i > 50)
        {
            fprintf(stderr, "Reading the configuration file : failure\n");
            return 0;
        }
        ++i;
    }
}




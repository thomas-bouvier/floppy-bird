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
    fprintf(stderr, "Reading the configuration file : %s failure\n", type);
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

/*!
* \brief Open the files necessary for the game (predefined level and score file)
* \param[in] config the file that contains the configuration paths
* \param[out] level the file that contains predefined obstacles
* \param[out] scoreFile the file that contains the score
* \return Return 1 if files were well opened, 0 if failure
*/
int openGameFiles(FILE * config, FILE ** level, FILE ** scoreFile)
{
    char path[100];
    /* Open the file that contains the save of the level */
    if (readConfig(config, path, "level :\n"))
        *level = fopen(path, "r");
    if(*level == NULL)
    {
        fprintf(stderr,"Opening level file failure :\n");
        printf("%s\n", path);
        return 0;
    }
    /* Open the file that contains the save of the best score : create it if it does not exist yet */
    if (readConfig(config, path, "score :\n"))
    {
        *scoreFile = fopen(path, "r+");
        if (*scoreFile == NULL)
            *scoreFile = fopen(path, "w+");
    }
    if(*scoreFile == NULL)
    {
        fprintf(stderr,"Opening score file failure :\n");
        printf("%s\n", path);
        return 0;
    }
    return 1;
}

/*!
* \brief Open the sound files for the game
* \param[out] config the file that contains the configuration paths
* \param[out] jump_sound sound for a jump
* \param[out] obstacle_sound the sound played when an obstacle is passed
* \param[out] death_sound the sound played when the bird died
* \return Return 1 if files were well opened, 0 if failure
*/
int openSoundFiles(FILE * config, Mix_Chunk ** jump_sound, Mix_Chunk ** obstacle_sound, Mix_Chunk ** death_sound)
{
    int i;
    char path[100];
    char sounds_type[3][20] = {"jump :\n", "obstacle :\n", "death :\n"};
    Mix_Chunk ** sounds_table[] = {jump_sound, obstacle_sound, death_sound};
    for (i=0 ; i<3 ; ++i)
    {
        if (readConfig(config, path, sounds_type[i]))
        {
            *(sounds_table[i]) = Mix_LoadWAV(path);
            if (*(sounds_table[i]) == NULL)
            {
                fprintf(stderr,"Opening sound failure :\n");
                printf("%s\n", path);
                return 0;
            }
        }
    }
    return 1;
}

/*!
* \brief Open the sprites for the game
* \param[out] config the file that contains the configuration paths
* \param[out] sprites the structure that contains all the Surfaces
* \param[out] renderer the drawing target
* \return Return 1 if files were well opened, 0 if failure
*/
int openSpriteFiles(FILE * config, Sprites * sprites, SDL_Renderer * renderer)
{
    int i;
    char path[100];
    char sprite_types[11][20] = { "play :\n", "quit :\n","background :\n", "ground :\n", "pause :\n","tap :\n","bird1 :\n", "bird2 :\n","bird3 :\n", "pipe1 :\n", "pipe2 :\n"};
    SDL_Texture ** texture_table[] = {&sprites->play, &sprites->quit,&sprites->background,
                                    &sprites->ground,&sprites->pause, &sprites->tap_to_play};
    GenericList ** list_table[] = {&sprites->bird1, &sprites->bird2, &sprites->bird3, &sprites->pipe1, &sprites->pipe2};
    SDL_Surface * surface = NULL;
    for(i= 0 ; i<6 ; ++i)
    {
        if (readConfig(config, path, sprite_types[i]))
        {
          surface = IMG_Load(path);
           *(texture_table[i]) = SDL_CreateTextureFromSurface(renderer, surface);
           if(*(texture_table[i]) == NULL)
            {
                fprintf(stderr, "Opening sprite failure :\n");
                printf("%s\n", path);
                return 0;
            }
        }
    }
    for(i= 0 ; i<5 ; ++i)
    {
        *(list_table[i]) = newGenericList(NULL, (FreeFunction)SDL_DestroyTexture);
        initGenericList(*(list_table[i]));
        if (readConfig(config, path, sprite_types[i+6]))
        {
          surface = IMG_Load(path);
           add(*(list_table[i]), SDL_CreateTextureFromSurface(renderer, surface));
           if(*(texture_table[i]) == NULL)
            {
                fprintf(stderr, "Opening sprite failure :\n");
                printf("%s\n", path);
                return 0;
            }
        }
        int j;
        if(i>2)
        {
            for(j=0; j<OBSTACLE_NUMBER - 1; ++j)
            {
                if (readConfig(config, path, sprite_types[i+6]))
                {
                  surface = IMG_Load(path);
                   add(*(list_table[i]), SDL_CreateTextureFromSurface(renderer, surface));
                   if(*(texture_table[i]) == NULL)
                    {
                        fprintf(stderr, "Opening sprite failure :\n");
                        printf("%s\n", path);
                        return 0;
                    }
                }
            }
        }
    }
    SDL_FreeSurface(surface);
    return 1;
}
/*!
* \brief Open the sprites for a new bird
* \param[out] config the file that contains the configuration paths
* \param[out] sprites the structure that contains all the Surfaces
* \param[out] renderer the drawing target
* \return Return 1 if files were well opened, 0 if failure
*/
int addBirdSprite(FILE * config, Sprites * sprites, SDL_Renderer * renderer)
{
    int i;
    char path[100];
    char sprite_types[3][20] = { "bird1 :\n", "bird2 :\n", "bird3 :\n"};
    GenericList ** sprite_table[] = { &sprites->bird1, &sprites->bird2, &sprites->bird3};
    SDL_Surface * surface = NULL;
    for(i= 0 ; i<3 ; ++i)
    {
        if (readConfig(config, path, sprite_types[i]))
        {
          surface = IMG_Load(path);
           add(*(sprite_table[i]), SDL_CreateTextureFromSurface(renderer, surface));
           if(*(sprite_table[i]) == NULL)
            {
                fprintf(stderr, "Opening sprite failure :\n");
                printf("%s\n", path);
                return 0;
            }
        }
    }
    SDL_FreeSurface(surface);
    return 1;
}
/*!
* \brief Open the font file for the game
* \param[out] config the file that contains the configuration paths
* \param[out] big_font the biggest font used in the game
* \param[out] medium_font the same font, but smaller
* \return Return 1 if file was well opened, 0 if failure
*/
int openFontFiles(FILE * config, TTF_Font ** big_font, TTF_Font ** medium_font)
{
    char path[100];
    if (readConfig(config, path, "font :\n"))
    {
        *big_font = TTF_OpenFont(path, 70);
        *medium_font = TTF_OpenFont(path, 40);
        if(*big_font == NULL || *medium_font == NULL)
        {
            fprintf(stderr, "Missing font : %s\n", TTF_GetError());
            return 0;
        }
    }
    return 1;
}

/*!
* \brief Close all the files
* \param[out] config the file that contains the configuration paths
* \param[out] level the file that contains predefined obstacles
* \param[out] scoreFile the file that contains the score
* \param[out] jump_sound sound for a jump
* \param[out] obstacle_sound the sound played when an obstacle is passed
* \param[out] death_sound the sound played when the bird died
* \param[out] sprites the structure that contains all the Surfaces
* \param[out] font the font used in the game
*/
void closeFiles(FILE * config, FILE * level, FILE * scoreFile, Mix_Chunk * jump_sound, Mix_Chunk * obstacle_sound,
               Mix_Chunk * death_sound, Sprites * sprites, TTF_Font * font)
{
    freeGenericList(sprites->bird1);
    freeGenericList(sprites->bird2);
    freeGenericList(sprites->bird3);
    freeGenericList(sprites->pipe1);
    freeGenericList(sprites->pipe2);
    SDL_DestroyTexture(sprites->background);
    SDL_DestroyTexture(sprites->ground);
    SDL_DestroyTexture(sprites->pause);
    SDL_DestroyTexture(sprites->play);
    SDL_DestroyTexture(sprites->quit);
    SDL_DestroyTexture(sprites->tap_to_play);
    Mix_FreeChunk(jump_sound);
    Mix_FreeChunk(obstacle_sound);
    Mix_FreeChunk(death_sound);
    TTF_CloseFont(font);
    fclose(config);
    fclose(level);
    fclose(scoreFile);
}

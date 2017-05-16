/*!
* \file sdl_files.c
* \brief File containing the functions to open SDL files
*/
#include "sdl_files.h"

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

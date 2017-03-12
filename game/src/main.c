#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "view.h"
#include "control.h"
#include "file.h"
#include <SDL2/SDL.h>

int main(int argc, char ** argv)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    Bird * bird = NULL;
    Obstacle * obstacle[PIPES_ON_SCREEN];
    Camera * camera = NULL;

    initGame(bird, camera, obstacle);
    if (initDisplay(window, renderer) == 0)
        return EXIT_FAILURE;

    int hit = 0;
    int running = 1;

    /* Open the file that contains the save of the level */
    FILE * f = NULL;
    f = fopen("./../res/files/level.txt", "r");
    if(f==NULL)
    {
        fprintf(stderr,"Opening file failure : %d", ferror(f));
        return EXIT_FAILURE;
    }

    /* Wait the first jump to start the game*/
    int init;
    while(init == NOTHING)
    {
        init = detectTouch();
        if(init == JUMP)
            bird->dir_y = BIRD_JUMP;
        if(init == QUIT)
            running = 0;
    }

    while(running)
    {
        int number =0;
        while(!hit&&running)
        {
            int event = detectTouch();
            if(event == QUIT)
                running=QUIT;
            hit=game(bird, camera, obstacle, event, readLevel(f, number), number);
            displayGame(renderer, bird, obstacle[PIPES_ON_SCREEN], camera);
            ++number;
        }
    }

    /* Quit the game */
    freeAll(bird, obstacle, camera);
    quitGame(window, renderer);
    fclose(f);

    return EXIT_SUCCESS;
}

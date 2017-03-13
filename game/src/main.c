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

    int hit;
    int running = 1;
    int init;
    int number;

    Bird bird;
    Obstacle obstacle[PIPES_ON_SCREEN];
    Camera camera;

    while(running)
    {

        initGame(&bird, &camera, obstacle);
        if (initDisplay(window, renderer) == 0)
        {
            fprintf(stderr, "Display initialization failure");
            return EXIT_FAILURE;
        }
        displayGame(renderer, &bird, obstacle, &camera);


        /* Open the file that contains the save of the level */
        FILE * f = NULL;
        f = fopen("./../../res/files/level.txt", "r");
        if(f==NULL)
        {
            fprintf(stderr,"Opening file failure : %d", ferror(f));
            return EXIT_FAILURE;
        }

        /* Wait the first jump to start the game*/
        init = NOTHING;
        while(init == NOTHING)
        {
            init = detectTouch();
            if(init == JUMP)
                bird.dir_y = BIRD_JUMP;
            if(init == QUIT)
                running = 0;
        }


        number = 0;
        hit = 0;
        while(!hit && running)
        {
            int event = detectTouch();
            if(event == QUIT)
                running = 0;
            hit = game(&bird, &camera, obstacle, event, readLevel(f, number), number);
            displayGame(renderer, &bird, obstacle, &camera);
            ++number;
            SDL_Delay(30);
        }
        fclose(f);
    }

    /* Quit the game */
    freeAll(&bird, obstacle, &camera);
    quitGame(window, renderer);

    return EXIT_SUCCESS;
}

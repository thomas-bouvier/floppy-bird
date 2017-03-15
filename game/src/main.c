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
    Camera camera;
    List l;

    /* SDL initialization */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization failure");
        return 0;
    }

    /* Setup window */
    window = SDL_CreateWindow("Floppy Bird",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "Opening window failure %s\n,", SDL_GetError());
        return 0;
    }

    /* Setup renderer */
    renderer =  SDL_CreateRenderer(window,
                                   -1,
                                   SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );

    while(running)
    {
        startGame(&bird, &camera, &l);
        displayGame(renderer, &bird, &l, &camera);

        /* Open the file that contains the save of the level */
        FILE * f = NULL;
        f = fopen("./../../res/files/level.txt", "r");
        if(f==NULL)
        {
            fprintf(stderr,"Opening file failure");
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

        /* Loop of game */
        number = 1;
        hit = 0;
        while(!hit && running)
        {
            int event = detectTouch();
            if(event == QUIT)
                running = 0;
            hit = game(&bird, &camera, &l, event, readLevel(f, number), number);
            displayGame(renderer, &bird, &l, &camera);
            ++number;
            SDL_Delay(1);
        }
        SDL_Delay(1000);
        fclose(f);
    }

    /* Quit the game */
    quitGame(window, renderer);

    return EXIT_SUCCESS;
}

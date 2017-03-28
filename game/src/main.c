#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "view.h"
#include "control.h"
#include "file.h"
#include "constants.h"
#include <SDL2/SDL.h>

int main(int argc, char ** argv)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    int hit;
    int running = 1;
    Action init;
    int number;

    Bird bird;
    Camera camera;
    List l;

    /* SDL initialization */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization failure\n");
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

        /* Open the configuration file (that contains the paths of level, sprites), according to the parameter passed to main (or not)... */
        FILE * config = NULL;
        if (argc == 1)
            config = fopen("conf/config.txt", "r");
        else
            config = fopen(argv[1], "r");
        if(config == NULL)
        {
            fprintf(stderr,"Opening configuration file failure\n");
            return EXIT_FAILURE;
        }

        /* Open the file that contains the save of the level */
        FILE * level = NULL;
        char * levelPath = malloc(sizeof(*levelPath));
        if (readConfig(config, levelPath, "level :\n"))
        {
            if (levelPath[strlen(levelPath)-1] == '\n')
                levelPath[strlen(levelPath)-1] = '\0';
            level = fopen(levelPath, "r");
        }


        if(level == NULL)
        {
            fprintf(stderr,"Opening level file failure :\n");
            printf("%s\n", levelPath);
            return EXIT_FAILURE;
        }

        /* Wait the first jump to start the game*/
        emptyEvent();
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
            Action event = detectTouch();
            if(event == QUIT)
                running = 0;
            hit = game(&bird, &camera, &l, event, readLevel(level, number), number);
            displayGame(renderer, &bird, &l, &camera);
            ++number;
            SDL_Delay(1);
        }
        SDL_Delay(1000);
        fclose(level);
    }

    /* Quit the game */
    quitGame(window, renderer);

    return EXIT_SUCCESS;
}

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

    Uint32 lastFrame;
    int i;

    int hit;
    int running = 1;
    Action init;
    int number;

    Bird bird;
    Camera camera;
    List l;

    FILE * config = NULL;
    FILE * level = NULL;

    int score;
    Obstacle * savedObstacle = NULL;

    /* Open the configuration file (that contains the paths of level, sprites),
    according to the parameter passed to main (or not) */
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
    if(LEVEL_FROM_FILE)
    {
        char levelPath[100];
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
    }

    /* Open the file that contains the save of the best score : create it if it does not exist yet */
    FILE * scoreFile = NULL;
    char scorePath[100];
    if (readConfig(config, scorePath, "score :\n"))
    {
        if (scorePath[strlen(scorePath)-1] == '\n')
            scorePath[strlen(scorePath)-1] = '\0';
        scoreFile = fopen(scorePath, "r+");
        if (scoreFile == NULL)
            scoreFile = fopen(scorePath, "w+");
    }
    if(scoreFile == NULL)
    {
        fprintf(stderr,"Opening score file failure :\n");
        printf("%s\n", scorePath);
        return EXIT_FAILURE;
    }

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
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    while(running)
    {
        startGame(&bird, &camera, &l, level);
        savedObstacle = nextBirdObstacle(&l, &bird);
        drawLowForTI(renderer, &camera);
        running = waitForTI();
        drawUpForTI(renderer, &camera);
        if (running)
            running = waitForTI();
        displayGame(renderer, &bird, &l, &camera);

        /* Wait the first jump to start the game*/
        emptyEvent();
        init = NOTHING;
        if (running)
            while(init == NOTHING)
            {
                init = detectTouch();
                if(init == JUMP)
                    bird.dir_y = BIRD_JUMP;
                if(init == QUIT)
                    running = 0;
            }

        /* Loop of game */
        number = OBSTACLE_NUMBER;
        score = 0;
        hit = 0;
        lastFrame = SDL_GetTicks();

        while(!hit && running)
        {

            for(i = 0; i < (SDL_GetTicks()-lastFrame)/(1000/FRAME_PER_SECOND); ++i)
            {
                Action event = detectTouch();
                if(event == QUIT)
                     running = 0;
                if(event == PAUSE)
                    running = (waiting() != QUIT);
                hit = game(&bird, &camera, &l, level, event, &number, savedObstacle, &score);
                savedObstacle = nextBirdObstacle(&l, &bird);
                displayGame(renderer, &bird, &l, &camera);
                lastFrame = SDL_GetTicks();
            }
            saveScore(scoreFile, score);
        }
        if(hit)
        {
            SDL_Delay(300);
            SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(600);
        }
    }

    /* Quit the game */
    quitGame(window, renderer);
    fclose(config);
    if(LEVEL_FROM_FILE)
		fclose(level);
    fclose(scoreFile);

    return EXIT_SUCCESS;
}

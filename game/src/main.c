#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "view.h"
#include "control.h"
#include "file.h"
#include "constants.h"
#include <SDL2/SDL.h>
#include "../../ai/q_learning/src/q_learning.h"
#include "../../ai/q_learning/src/game_state.h"

int main(int argc, char ** argv)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    Uint32 last_frame;
    int i;

    int hit;
    int running = 1;
    Action init;
    int number;

    Bird bird;
    Camera camera;
    List l;

    FILE * level = NULL;

    int score;
    Obstacle * savedObstacle = NULL;

	/* Load Q matrix data */
	MatrixQ * matrixQ = loadQMatrix();

	/* Number of last states saved / last actions */
	int last_states[NB_SAVED_STATES]; /* [0]current_state [1]last_state */
	int last_action = -1;
	init_array(last_states, NB_SAVED_STATES, -1);
	srand(time(NULL));

    /* Open the configuration file (that contains the paths of level, sprites),
    according to the parameter passed to main (or not) */
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
        if(LEVEL_FROM_FILE)
        {
        char * levelPath = malloc(sizeof(char)*100);
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
    char * scorePath = malloc(sizeof(char)*100);
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
        displayGame(renderer, &bird, &l, &camera);

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
        number = OBSTACLE_NUMBER;
        score = 0;
        hit = 0;
        last_frame = SDL_GetTicks();

        while(!hit && running)
        {

            for(i = 0; i < (SDL_GetTicks()-last_frame)/(1000/FRAME_PER_SECOND); ++i)
            {
                Action event = detectTouch();
                if(event == QUIT)
                     running = 0;
                if(event == PAUSE)
                    running = (waiting() != QUIT);
				q_learning_loop(matrixQ, last_states, &last_action, 200, 100, 1);
				/*if(last_states[0] != -1) event = last_action;*/
                hit = game(&bird, &camera, &l, level, event, &number, savedObstacle, &score);
                savedObstacle = nextBirdObstacle(&l, &bird);
                displayGame(renderer, &bird, &l, &camera);
                last_frame = SDL_GetTicks();
            }
			
			/*show_matrixQ(matrixQ);*/
            saveScore(scoreFile, score);
        }
        if(hit)
        {
            SDL_Delay(300);
            SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(600);
			saveQMatrix(matrixQ, "../res/test.txt");
        }
    }
	
    /* Quit the game */
	freeMatrixQ(matrixQ);
    quitGame(window, renderer);
    fclose(config);
    fclose(level);
    fclose(scoreFile);

    return EXIT_SUCCESS;
}

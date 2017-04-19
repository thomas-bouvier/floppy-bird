#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "view.h"
#include "control.h"
#include "file.h"
#include "sound.h"
#include "menu.h"
#include "constants.h"
#include <SDL2/SDL.h>
#include "../../ai/q_learning/src/q_learning.h"
#include "../../ai/q_learning/src/game_state.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, char ** argv)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    Uint32 lastFrame;
    int i;

    int hit;
    int running = 1;
    Action init;
	Sound sound;
	Mode mode;
    int number;

    Bird bird;
    Camera camera;
    List l;

    FILE * config = NULL;
    FILE * level = NULL;
    FILE * scoreFile = NULL;

	char jump_path[100];
	Mix_Chunk * jump_sound;
	char obstacle_path[100];
	Mix_Chunk * obstacle_sound;
	char death_path[100];
	Mix_Chunk * death_sound;
	char fontPath[100];
    TTF_Font * font = NULL;

	int levelFromFile = 1;
    int score;
    Obstacle * savedObstacle = NULL;

	/* Initialization IA1 */
	MatrixQ * matrixQ = NULL;
	int last_states[NB_SAVED_STATES];
	int last_action[NB_SAVED_ACTIONS];
	char qmatrixPath[100];
	int hit_saved=0;
	srand(time(NULL));

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
    if(levelFromFile)
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
        return EXIT_FAILURE;
    }

	/* SDL_TTF initialization */
    if (TTF_Init() != 0)
    {
        fprintf(stderr, "SDL_TTF initialization failure\n");
        return EXIT_FAILURE;
    }

	/* SDL_mixer initialization */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
    {
        fprintf(stderr, "SDL_mixer initialization failure\n");
        return EXIT_FAILURE;
    }

	/* Setup sounds */
	Mix_AllocateChannels(3);
	if (readConfig(config, jump_path, "jump :\n"))
	{
	    jump_sound = Mix_LoadWAV(jump_path);
	    if (jump_sound == NULL)
        {
            fprintf(stderr,"Opening jump sound failure :\n");
            printf("%s\n", jump_path);
            return EXIT_FAILURE;
        }
	}
     if (readConfig(config, obstacle_path, "obstacle :\n"))
	{
	    obstacle_sound = Mix_LoadWAV(obstacle_path);
	    if (obstacle_sound == NULL)
        {
            fprintf(stderr,"Opening obstacle sound failure :\n");
            printf("%s\n", obstacle_path);
            return EXIT_FAILURE;
        }
	}
	if (readConfig(config, death_path, "death :\n"))
	{
	    death_sound = Mix_LoadWAV(death_path);
	    if (death_sound == NULL)
        {
            fprintf(stderr,"Opening death sound failure :\n");
            printf("%s\n", death_path);
            return EXIT_FAILURE;
        }
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

   	/* Open a font for text */
	if (readConfig(config, fontPath, "font :\n"))
    {
        font = TTF_OpenFont(fontPath, 70);
        if(font == NULL)
        {
            fprintf(stderr, "Missing font : %s\n", TTF_GetError());
            return EXIT_FAILURE;
        }
    }

    mode = WAIT;
    while(mode != PLAY && mode != IA1 && init != QUIT)
    {
        mode = mainMenu(renderer, font);
        init = detectTouch();
    }
    if(init == QUIT)
        running = 0;

	/* Setup IA1 */
	if(mode == IA1)
	{

		/* Open the file that contains qMatrix data */
		if (readConfig(config, qmatrixPath, "qMatrix :\n"))
		{
		    if (qmatrixPath[strlen(qmatrixPath)-1] == '\n')
		        qmatrixPath[strlen(qmatrixPath)-1] = '\0';
		}

		matrixQ = loadQMatrix(qmatrixPath);
		init_array(last_states, NB_SAVED_STATES, -1);
		init_array(last_action, NB_SAVED_ACTIONS, -1);
	}


    while(running)
    {
    	score = 0;
        startGame(&bird, &camera, &l, level, levelFromFile);
        savedObstacle = nextBirdObstacle(&l, &bird);
        drawForTI(renderer, &camera);
        if(mode == PLAY) running = waitForTI();
		if(mode == IA1) running = 1;
        displayGame(renderer, &bird, &l, &camera, score, font);

        if(mode == PLAY) /* Wait the first jump to start the game*/
		{        
			emptyEvent();
		    init = NOTHING;
		    while(init == NOTHING && running)
		    {
		        init = detectTouch();
		        if(init == JUMP)
		        {
		            bird.dir_y = BIRD_JUMP;
		            /*playSound(JUMPSOUND, jump_sound, obstacle_sound, death_sound);*/
		        }
		        if(init == QUIT)
		            running = 0;
		    }
		}

        /* Loop of game */
        number = OBSTACLE_NUMBER;
        score = 0;
        hit = 0;
        lastFrame = SDL_GetTicks();
		int action_break = 0;
        while(!hit && running)
        {
			
            for(i = 0; i < (SDL_GetTicks()-lastFrame)/(1000/FRAME_PER_SECOND); ++i)
            {
                Action event = detectTouch();
                sound = NOSOUND;
                if(event == QUIT)
                     running = 0;
                if(event == PAUSE)
                    running = (waiting() != QUIT);
				if(mode == IA1 && (action_break == 0 || hit_saved == 1))
				{
					q_learning_loop(matrixQ, last_states, last_action, ratioPipeWidth(&bird, &l), ratioBirdHeight(&bird)-ratioPipeHeight(&bird, &l), hit_saved);
					if(last_action[0] != -1) event = last_action[0];
				}
				if(++action_break >= NB_FPS_BEFORE_NEXT_ACTION) action_break=0;

                hit = game(&bird, &camera, &l, level, event, &number, savedObstacle, &score, &sound, levelFromFile);
                hit_saved = hit;
				savedObstacle = nextBirdObstacle(&l, &bird);
                displayGame(renderer, &bird, &l, &camera, score, font);
                /*playSound(sound, jump_sound, obstacle_sound, death_sound);*/
                lastFrame = SDL_GetTicks();
            }
			
            saveScore(scoreFile, score);
        }
        if(hit && mode == PLAY)
        {
            SDL_Delay(300);
            SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
            SDL_RenderClear(renderer);
            displayBestScore(renderer, font, scoreFile);
            SDL_RenderPresent(renderer);
            SDL_Delay(1500);
        }
		if(hit && mode == IA1) 
		{
			saveQMatrix(matrixQ, qmatrixPath);
		}
    }

    /* Quit the game */
	if(mode == IA1) freeMatrixQ(matrixQ);
    quitGame(window, renderer);
    Mix_FreeChunk(jump_sound);
    Mix_FreeChunk(obstacle_sound);
    Mix_FreeChunk(death_sound);
    fclose(config);
    if(levelFromFile)
		fclose(level);
    fclose(scoreFile);

    return EXIT_SUCCESS;
}

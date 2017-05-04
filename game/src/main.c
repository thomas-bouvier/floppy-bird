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
    int menu_loop = 1;
    Action init;
    Sound sound;
    Mode mode;
    int number;

    Bird bird;
    Camera camera;
    List l;

    FILE * config = NULL;
    FILE * level = NULL;
    char scorePath[100];
    FILE * scoreFile = NULL;
    char jump_path[100];
    Mix_Chunk * jump_sound;
    char obstacle_path[100];
    Mix_Chunk * obstacle_sound;
    char death_path[100];
    Mix_Chunk * death_sound;
    char fontPath[100];
    TTF_Font * font = NULL;

    char bird1_path[100];
    char bird2_path[100];
    char bird3_path[100];
    char pipe1_path[100];
    char pipe2_path[100];
    char background_path[100];
    char ground_path[100];
    char tap_path[100];
    char play_path[100];
    char quit_path[100];
    Sprites sprites;

    /* if levelFromFile == 1, the game is run with predefined height of obstacles ; if not, they are generated randomly */
    int levelFromFile = 1;
    /* if simplifiedMode == 1, the game is played in simplified mode ; if not, the normal game is run (with sprites) */
    int simplifiedMode = 0;
    int score;
    Obstacle * savedObstacle = NULL;

    /* Initialization IA1 */
    MatrixQ * matrixQ = NULL;
    int last_states[NB_SAVED_STATES];
    int last_action[NB_SAVED_ACTIONS];
    char qmatrixPath[100];
    int hit_saved=0;
    int action_break=0;
    int nb_fps_before_next_action=NB_FPS_BEFORE_NEXT_ACTION_MIN;
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
    char levelPath[100];
    if (readConfig(config, levelPath, "level :\n"))
        level = fopen(levelPath, "r");
    if(level == NULL)
    {
        fprintf(stderr,"Opening level file failure :\n");
        printf("%s\n", levelPath);
        return EXIT_FAILURE;
    }

    /* Open the file that contains the save of the best score : create it if it does not exist yet */
    if (readConfig(config, scorePath, "score :\n"))
    {
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
    /* Setup sprites */
    if (readConfig(config, bird1_path, "bird1 :\n"))
    {
        sprites.bird1=IMG_Load(bird1_path);
        if(!sprites.bird1)
        {
            fprintf(stderr, "Opening bird1 sprite failure :\n");
            printf("%s\n", bird1_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, bird2_path, "bird2 :\n"))
    {
        sprites.bird2=IMG_Load(bird2_path);
        if(!sprites.bird2)
        {
            fprintf(stderr, "Opening bird2 sprite failure :\n");
            printf("%s\n", bird2_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, bird3_path, "bird3 :\n"))
    {
        sprites.bird3=IMG_Load(bird3_path);
        if(!sprites.bird3)
        {
            fprintf(stderr, "Opening bird3 sprite failure :\n");
            printf("%s\n", bird3_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, pipe1_path, "pipe1 :\n"))
    {
        sprites.pipe1=IMG_Load(pipe1_path);
        if(!sprites.pipe1)
        {
            fprintf(stderr, "Opening pipe1 sprite failure :\n");
            printf("%s\n", pipe1_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, pipe2_path, "pipe2 :\n"))
    {
        sprites.pipe2=IMG_Load(pipe2_path);
        if(!sprites.pipe2)
        {
            fprintf(stderr, "Opening pipe2 sprite failure :\n");
            printf("%s\n", pipe2_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, background_path, "background :\n"))
    {
        sprites.background=IMG_Load(background_path);
        if(!sprites.background)
        {
            fprintf(stderr, "Opening background sprite failure :\n");
            printf("%s\n", background_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, ground_path, "ground :\n"))
    {
        sprites.ground=IMG_Load(ground_path);
        if(!sprites.ground)
        {
            fprintf(stderr, "Opening ground sprite failure :\n");
            printf("%s\n", ground_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, tap_path, "tap :\n"))
    {
        sprites.tap_to_play=IMG_Load(tap_path);
        if(!sprites.tap_to_play)
        {
            fprintf(stderr, "Opening tap sprite failure :\n");
            printf("%s\n", tap_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, play_path, "play :\n"))
    {
        sprites.play=IMG_Load(play_path);
        if(!sprites.play)
        {
            fprintf(stderr, "Opening play sprite failure :\n");
            printf("%s\n", play_path);
            return EXIT_FAILURE;
        }
    }
    if (readConfig(config, quit_path, "quit :\n"))
    {
        sprites.quit=IMG_Load(quit_path);
        if(!sprites.quit)
        {
            fprintf(stderr, "Opening quit sprite failure :\n");
            printf("%s\n", quit_path);
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

    while(menu_loop)
    {
        mode = WAIT;
        init = NOTHING;
        running = 1;
        while((mode != PLAY && mode != IA1) && init != QUIT)
        {
            mode = WAIT;
            mode = mainMenu(renderer, &camera, font, &levelFromFile, &simplifiedMode, &sprites);
            init = detectTouch();
            if(mode == QUITGAME)
                init = QUIT;
            if(init == QUIT)
                menu_loop = 0;
        }

        if(init == QUIT)
            running = 0;

        /* Setup IA1 */
        if(mode == IA1)
        {
            /* Open the file that contains qMatrix data */
            readConfig(config, qmatrixPath, "qMatrix :\n");
            matrixQ = loadQMatrix(qmatrixPath);
            init_array(last_states, NB_SAVED_STATES, -1);
            init_array(last_action, NB_SAVED_ACTIONS, -1);
        }

        while(running)
        {
            score = 0;
            startGame(&bird, &camera, &l, level, levelFromFile);
            savedObstacle = nextBirdObstacle(&l, &bird);
            if (simplifiedMode)
            {
                drawForTI(renderer, &camera);
                if(mode == PLAY)
                    running = waitClick();
                if(mode == IA1)
                    running = 1;
                displayGame(renderer, &bird, &l, &camera, score, font);
            }
            else
                displayRealGame(renderer, &bird, &l, &camera, score, font, &sprites);

            if(mode == PLAY) /* Wait the first jump to start the game*/
            {
                emptyEvent();
                init = NOTHING;
                tapToPlay(renderer, &camera, &sprites);
                SDL_RenderPresent(renderer);

                while(init == NOTHING && running)
                {
                    init = detectTouch();
                    if(init == JUMP)
                    {
                        bird.dir_y = BIRD_JUMP;
                        playSound(JUMPSOUND, jump_sound, obstacle_sound, death_sound);
                    }
                    if(init == QUIT)
                        running = 0;
                }
            }

            number = OBSTACLE_NUMBER;
            hit = 0;
            lastFrame = SDL_GetTicks();
            if(mode == IA1)
                action_break = 0;

            while(!hit && running)
            {
                for(i = 0; i < (SDL_GetTicks()-lastFrame)/(1000/FRAME_PER_SECOND); ++i)
                {
                    /*printf("%d\n", 1000/(SDL_GetTicks()-lastFrame));*/
                    Action event = detectTouch();
                    sound = NOSOUND;
                    if(event == PAUSE)
                    {
                        while(event == PAUSE)
                        {
                            pauseMenu(renderer, &camera, &sprites);
                            event = actionOnPauseMenu();
                        }
                    }
                    if(event == QUIT)
                        running = 0;



                    if(mode == IA1 && (action_break == 0 || hit_saved == 1))
                    {
                        q_learning_loop(matrixQ, last_states, last_action, ratioPipeWidth(&bird, &camera, &l), ratioBirdHeight(&bird)-ratioPipeHeight(&bird, &l), hit_saved);
                        if(last_action[0] != -1)
                            event = last_action[0];
                    }
                    if(mode == IA1 && ++action_break >= nb_fps_before_next_action)
                    {
                        action_break=0;
                        nb_fps_before_next_action=randomInRange(NB_FPS_BEFORE_NEXT_ACTION_MIN, NB_FPS_BEFORE_NEXT_ACTION_MAX);
                    }

                    /* Update of the model */
                    updateBird(&bird, event, &sound);
                    deleteObstacle(&camera, &l);
                    if (createObstacle(&camera, &l, level, number, levelFromFile))
                        number++;
                    score = updateScore(score, &bird, savedObstacle, &sound);
                    if(simplifiedMode == 0)
                        modifySpeed(score, &camera);
                    cameraScrolling(&camera, &bird);
                    hit = detectHit(&bird, nextBirdObstacle(&l, &bird), &sound);
                    hit_saved = hit;
                    savedObstacle = nextBirdObstacle(&l, &bird);

                    if(simplifiedMode)
                        displayGame(renderer, &bird, &l, &camera, score, font);
                    else
                        displayRealGame(renderer, &bird, &l, &camera, score, font, &sprites);
                    playSound(sound, jump_sound, obstacle_sound, death_sound);
                    lastFrame = SDL_GetTicks();
                }
                saveScore(scoreFile, score);
            }

            if(hit && mode == PLAY)
            {
                if(!simplifiedMode)
                {
                    while(bird.y < SCREEN_HEIGHT)
                    {
                        bird.y+=10;
                        displayRealGame(renderer, &bird, &l, &camera, score, font, &sprites);
                        SDL_Delay(16);
                    }
                }
                SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
                SDL_RenderClear(renderer);
                displayBestScore(renderer, font, scoreFile);
                SDL_RenderPresent(renderer);
                running=waitClick();
            }
            if(hit && mode == IA1)
                saveQMatrix(matrixQ, qmatrixPath);
        }
    }

    /* Quit the game */
    if(mode == IA1)
        freeMatrixQ(matrixQ);
    Mix_FreeChunk(jump_sound);
    Mix_FreeChunk(obstacle_sound);
    Mix_FreeChunk(death_sound);
    fclose(config);
    fclose(level);
    fclose(scoreFile);
    quitGame(window, renderer);

    return EXIT_SUCCESS;
}

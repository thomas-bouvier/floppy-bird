#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "view.h"
#include "control.h"
#include "file.h"
#include "sdl_files.h"
#include "sound.h"
#include "menu.h"
#include "constants.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "../../ai/q_learning/src/q_learning.h"
#include "../../ai/q_learning/src/game_state.h"

#include "../../ai/neat/src/generic_list.h"
#include "../../ai/neat/src/population.h"

int evaluate(Genome * genome, double ratioBirdHeight, double ratioPipeWidth, double ratioPipeHeight)
{
    double input[N_INPUTS];
    double * output;
    double res;

    input[0] = ratioBirdHeight;
    input[1] = ratioPipeWidth;
    input[2] = ratioPipeHeight;
    input[3] = 1.0;

    output = evaluateGenome(genome, input);

    res = *output;
    free(output);

    return res > 0.5;
}

void learn(GenericList * bird_list, Bird ** best_bird, MatingPool * pool, int * hit, int * ticks) {
    Bird * bird = NULL;
    double fitness;

    //check if all the birds are dead
    setOnFirstElement(bird_list);
    *best_bird = (Bird *) getCurrent(bird_list);
    while (!outOfGenericList(bird_list)) {

        //current bird
        bird = (Bird *) getCurrent(bird_list);

        if (!bird->dead) {
            *hit = 0;

            fitness = (double) *ticks - (double) bird->flaps * 2;
            if (fitness == 0.0) fitness = -1.0;

            bird->genome->fitness = fitness;
            if (fitness > pool->max_fitness) pool->max_fitness = fitness;

            if (fitness > (*best_bird)->genome->fitness) *best_bird = bird;
        }

        nextElement(bird_list);
    }

    //writeGenome(best_bird->genome);
}

int main(int argc, char ** argv)
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    Uint32 lastFrame;
    int i;

    int hit;
    int running = 1;
    int menu_loop = 1;
    int ticks = 0;

    Action init;
    Sound sound;
    Mode mode;
    int number;

    List obstacle_list;
    GenericList * bird_list = NULL;

    Camera camera;

    FILE * config = NULL;
    FILE * level = NULL;
    FILE * scoreFile = NULL;

    Mix_Chunk * jump_sound = NULL;
    Mix_Chunk * obstacle_sound = NULL;
    Mix_Chunk * death_sound = NULL;

    TTF_Font * big_font = NULL;
    TTF_Font * medium_font = NULL;

    Sprites sprites;

    /* if levelFromFile == 1, the game is run with predefined height of obstacles ; if not, they are generated randomly */
    int levelFromFile = 1;
    /* if simplifiedMode == 1, the game is played in simplified mode ; if not, the normal game is run (with sprites) */
    int simplifiedMode = 0;
    /* if speedAcceleration == 1, the game is accelerated ; if not, the speed stay constant */
    int speedAcceleration = 0;
    int score;

    Obstacle * savedObstacle = NULL;

    int ia1 = 0;
    int ia2 = 0;

    /* Initialization IA1 */
    MatrixQ * matrixQ = NULL;
    int last_states[NB_SAVED_STATES];
    int last_action[NB_SAVED_ACTIONS];
    char qmatrixPath[100];
    int hit_saved = 0;
    int action_break = 0;
    int nb_fps_before_next_action = NB_FPS_BEFORE_NEXT_ACTION_MIN;

    /* Initialization IA2 */
    MatingPool * pool = NULL;
    Bird * best_bird = NULL;

    srand(time(NULL));

    /* Open the configuration file (that contains the paths of level, sprites...),
    according to the parameter passed to main (or not) */
    if (argc == 1)
        config = fopen("conf/config.txt", "r");
    else
        config = fopen(argv[1], "r");

    if (config == NULL)
    {
        fprintf(stderr,"Opening configuration file failure\n");
        return EXIT_FAILURE;
    }

    /* Open the files necessary for the game (predefined level file, score file) */
    if (!openGameFiles(config, &level, &scoreFile))
        return EXIT_FAILURE;

    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");

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

    /* Setup sounds */
    Mix_AllocateChannels(3);
    if (!openSoundFiles(config, &jump_sound, &obstacle_sound, &death_sound))
        return EXIT_FAILURE;

    /* Setup sprites */
    if (!openSpriteFiles(config, &sprites, renderer))
        return EXIT_FAILURE;

    /* Setup font */
    if (!openFontFiles(config, &big_font, &medium_font))
        return EXIT_FAILURE;


    while (menu_loop)
    {
        mode = WAIT;
        init = NOTHING;
        running = 1;

        mode = IA2;

        while (mode != PLAY && mode != IA1 && mode != IA2 && init != QUIT)
        {
            mode = WAIT;
            mode = mainMenu(renderer, &camera, big_font, medium_font, &levelFromFile, &simplifiedMode, &speedAcceleration, &sprites);
            init = detectTouch();

            if (mode == QUITGAME)
                init = QUIT;
            if (init == QUIT)
                menu_loop = 0;
        }

        if (init == QUIT)
            running = 0;

        /* Setup IA1 */
        if (mode == IA1) {
            /* Open the file that contains qMatrix data */
            readConfig(config, qmatrixPath, "qMatrix :\n");
            matrixQ = loadQMatrix(qmatrixPath);
            init_array(last_states, NB_SAVED_STATES, -1);
            init_array(last_action, NB_SAVED_ACTIONS, -1);

            ia1 = 1;
        }

        /* Setup IA2 */
        if (mode == IA2) {
            if (pool)
                freeMatingPool(pool);

            pool = newMatingPool();

            if (pool == (MatingPool *) NULL)
                return EXIT_FAILURE;

            if (!populateMatingPool(pool))
                return EXIT_FAILURE;

            ia2 = 1;
        }

        while (running) {
            if (bird_list)
                freeGenericList(bird_list);

            bird_list = newGenericList(NULL, (FreeFunction) freeBird);
            initGenericList(bird_list);

            score = 0;

            if (mode == IA2) {
                startGameNeat(bird_list, &camera, &obstacle_list, level, levelFromFile, pool);

                setOnFirstElement(bird_list);
                while(!outOfGenericList(bird_list)) {
                    addBirdSprite(config, &sprites, renderer);

                    nextElement(bird_list);
                }
            }
            else
                startGame(bird_list, &camera, &obstacle_list, level, levelFromFile);

            savedObstacle = nextBirdObstacle(&obstacle_list, (Bird*)bird_list->first->data);

            if (simplifiedMode)
            {
                drawForTI(renderer, &camera);

                if (mode == PLAY)
                    running = waitClick();
                if (mode == IA1)
                    running = 1;
                if (mode == IA2)
                    running = 1;

                displayGame(renderer, bird_list, &obstacle_list, &camera, score, big_font, &sprites);
            }
            else
                displayRealGame(renderer, bird_list, &obstacle_list, &camera, score, big_font, &sprites);

            /* If we're in PLAY mode, wait the first jump to start the game */

            if (mode == PLAY)
            {
                emptyEvent();
                init = NOTHING;
                tapToPlay(renderer, &camera, &sprites);
                SDL_RenderPresent(renderer);

                while (init == NOTHING && running)
                {
                    init = detectTouch();

                    if(init == JUMP)
                    {
                        /* Faire sauter toute la liste */
                        setOnFirstElement(bird_list);
                        while(!outOfGenericList(bird_list))
                        {
                            ((Bird*)bird_list->current->data)->dir_y = BIRD_JUMP;
                            nextElement(bird_list);
                        }

                        playSound(JUMPSOUND, jump_sound, obstacle_sound, death_sound);
                    }

                    if (init == MENU || init == QUIT)
                        running = 0;

                    if (init == QUIT)
                        menu_loop = 0;
                }
            }

            number = OBSTACLE_NUMBER;
            hit = 0;
            lastFrame = SDL_GetTicks();

            if (mode == IA1)
                action_break = 0;

            ticks = 0;

            /* while the game is running and the birds are not all dead */

            while (!hit && running)
            {
                for (i = 0; i < (SDL_GetTicks() - lastFrame) / (1000 / FRAME_PER_SECOND); ++i)
                {
                    ++ticks;

                    Action event = detectTouch();
                    sound = NOSOUND;
                    if (event == PAUSE)
                    {
                        while (event == PAUSE)
                        {
                            pauseMenu(renderer, &camera, &sprites);
                            event = actionOnPauseMenu();
                        }
                    }

                    if (event == MENU || event == QUIT)
                        running = 0;
                    if (event == QUIT)
                        menu_loop = 0;

                    /* IA1 */

                    if (mode == IA1 && (action_break == 0 || hit_saved == 1))
                    {
                        q_learning_loop(matrixQ, last_states, last_action, ratioPipeWidth((Bird*)bird_list->first->data, &camera, &obstacle_list), ratioPipeHeight((Bird*)bird_list->first->data, &obstacle_list)-ratioBirdHeight((Bird*)bird_list->first->data), ratioPipeHeight((Bird*)bird_list->first->data, &obstacle_list), hit_saved);

                        if(last_action[0] != -1)
                            event = last_action[0];
                    }

                    if (mode == IA1 && ++action_break >= nb_fps_before_next_action)
                    {
                        action_break=0;
                        nb_fps_before_next_action = randomInRange(NB_FPS_BEFORE_NEXT_ACTION_MIN, NB_FPS_BEFORE_NEXT_ACTION_MAX);
                    }

                    /* Update of the model */

                    setOnFirstElement(bird_list);
                    if (mode == IA2)
                    {
                        while(!outOfGenericList(bird_list))
                        {
                            Bird * bird = (Bird *) getCurrent(bird_list);

                            if (evaluate(bird->genome, ratioBirdHeight(bird), ratioPipeWidth(bird, &camera, &obstacle_list), ratioPipeHeight(bird, &obstacle_list)))
                                updateBird(bird, JUMP, &sound);
                            else
                                updateBird(bird, NOTHING, &sound);

                            nextElement(bird_list);
                        }
                    }
                    else
                    {
                        while (!outOfGenericList(bird_list))
                        {
                            updateBird((Bird*) bird_list->current->data, event, &sound);
                            nextElement(bird_list);
                        }
                    }

                    deleteObstacle(&camera, &obstacle_list);
                    if (createObstacle(&camera, &obstacle_list, level, number, levelFromFile))
                        number++;

                    setOnFirstElement(bird_list);

                    score = updateScore(score, (Bird*) bird_list->current->data, savedObstacle, &sound);

                    if (simplifiedMode == 0 && speedAcceleration == 1)
                        modifySpeed(score, &camera);

                    cameraScrolling(&camera, bird_list);
                    setOnFirstElement(bird_list);

                    while (!outOfGenericList(bird_list))
                    {
                        if (detectHit((Bird*)bird_list->current->data, nextBirdObstacle(&obstacle_list, (Bird*)bird_list->current->data), &sound))
                            ((Bird*) bird_list->current->data)->dead = 1;

                        nextElement(bird_list);
                    }

                    hit = 1;

                    if (mode != IA2) {
                        setOnFirstElement(bird_list);
                        while (!outOfGenericList(bird_list))
                        {
                            if (((Bird*) bird_list->current->data)->dead == 0)
                                hit = 0;

                            nextElement(bird_list);
                        }
                    }
                    else {
                        learn(bird_list, &best_bird, pool, &hit, &ticks);

                        printf("fitness: %f / %f\n", best_bird->genome->fitness, pool->max_fitness);
                    }

                    hit_saved = hit;
                    savedObstacle = nextBirdObstacle(&obstacle_list, (Bird*)bird_list->first->data);

                    /* Update of the view */

                    if (simplifiedMode)
                        displayGame(renderer, bird_list, &obstacle_list, &camera, score, big_font, &sprites);
                    else
                        displayRealGame(renderer, bird_list, &obstacle_list, &camera, score, big_font, &sprites);

                    drawPause(renderer, &camera, &sprites);
                    SDL_RenderPresent(renderer);
                    playSound(sound, jump_sound, obstacle_sound, death_sound);

                    lastFrame = SDL_GetTicks();
                }
                saveScore(scoreFile, score);
            }

            /* all birds are dead, and we're in PLAY mode */

            if (hit && mode == PLAY)
            {
                while (birdFall((Bird*)bird_list->first->data, simplifiedMode))
                {
                    displayRealGame(renderer, bird_list, &obstacle_list, &camera, score, big_font, &sprites);

                    SDL_RenderPresent(renderer);
                    SDL_Delay(16);
                }

                if (simplifiedMode > 0)
                {
                    SDL_Delay(200);
                    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                    SDL_RenderClear(renderer);
                    displayScore(renderer, score, big_font);
                    displayBestScore(renderer, big_font, scoreFile);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(1000);
                }
                emptyEvent();

                Action end_of_game = NOTHING;
                while (end_of_game == NOTHING)
                {
                    SDL_RenderClear(renderer);

                    if (simplifiedMode == 0)
                        drawBackground(renderer, &camera, &sprites);

                    end_of_game = endOfGame(renderer, &camera, medium_font);

                    displayScore(renderer, score, big_font);
                    displayBestScore(renderer, big_font, scoreFile);
                    SDL_RenderPresent(renderer);
                }

                if (end_of_game == MENU || end_of_game == QUIT)
                    running = 0;
                if (end_of_game == QUIT)
                    menu_loop = 0;
            }

            /* all birds are dead, and we're in AI1 mode */

            if (hit && mode == IA1)
                saveQMatrix(matrixQ, qmatrixPath);

            /* all birds are dead, and we're in AI2 mode */

            if (hit && mode == IA2)
                newGeneration(pool, 0);
        }
    }

    /* Quit the game */
    if (ia1 > 0)
        freeMatrixQ(matrixQ);

    if (ia2 > 0)
        freeMatingPool(pool);

    closeFiles(config, level, scoreFile, jump_sound, obstacle_sound, death_sound, &sprites, big_font);
    quitGame(window, renderer);

    return EXIT_SUCCESS;
}

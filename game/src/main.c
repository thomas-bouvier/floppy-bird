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

    SDL_Event * event;

    initGame(bird, camera, obstacle);
    initDisplay(window, renderer);


    int hit = 0;
    int c = 1;

    FILE * f = NULL;
    f = fopen("./../res/files/level.txt", "r");
    if(f==NULL)
    {
        fprintf(stderr,"Opening file failure : %d", ferror(f));
        return EXIT_FAILURE;
    }

    if(initControl() == 0)
        c = 0;

    while(c)
    {
        while(!hit&&c)
        {


            int n = 0;
            int e = detectTouch(event);
            if(e == 0)
                c=0;
            updateBird(bird, e);
            obstacleCreation(camera,obstacle,n,readLevel(f,n),100);
            cameraScrolling(camera, bird);
            displayGame(bird, obstacle, camera);
            hit=detectHit(bird, nextObstacle(obstacle, bird));
            ++n;
        }
    }
    freeAll(bird, obstacle, camera);
    quitGame(window, renderer);
    fclose(f);
    return EXIT_SUCCESS;
}

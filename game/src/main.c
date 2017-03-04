#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include <SDL2/SDL.h>

int main(int argc, char ** argv)
{
    //SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO != 0))
    {
        fprintf(stderr, "SDL initialzation failure %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Creation of the game window
    SDL_Window * pWindow = NULL;
    pWindow = SDL_CreateWindow("Floppy Bird",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               800,
                               600,
                               SDL_WINDOW_RESIZABLE);
    if (pWindow == NULL)
    {
        fprintf(stderr, "Opening window failure : %s\n,", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Renderer creation
    SDL_Renderer * pRenderer = SDL_CreateRenderer(pWindow,
                                                  -1,
                                                  SDL_RENDERER_ACCELERATED);
    /*if (pRenderer == NULL);
    {
        fprintf(stderr, "Renderer creation failure : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }*/

    //Sprite creation
    SDL_Surface * pSprite = SDL_LoadBMP("C:/Users/Alexis/Documents/INSA 3EII/Info/SDL/Projet_C/floppy-bird-S6-2016/game/res/sprites/simplified pipe.bmp");
    if (pSprite == NULL)
    {
        fprintf(stderr, "Sprite creation failure : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Texture creation
    SDL_Texture * pTexture = SDL_CreateTextureFromSurface(pRenderer, pSprite);
    if (pTexture == NULL)
    {
        fprintf(stderr, "Texture creation failure : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Parameters of the rectangle destination for the sprite
    Pipe * pipe = newPipe(800 - pSprite->w, 600-pSprite->h, NULL, 3);
    SDL_Rect dest = {pipe->x, pipe->y, pSprite->w, pSprite->h};
    SDL_RenderCopy(pRenderer, pTexture, NULL, &dest);
    SDL_RenderPresent(pRenderer);



    //Game loop
    int end = 1;
    while(end)
    {
        SDL_RenderClear(pRenderer);
        pipeScrolling(pipe);
        dest.x = pipe->x;
        SDL_RenderCopy(pRenderer, pTexture, NULL, &dest);
        SDL_RenderPresent(pRenderer);
        SDL_Delay(5);

        if(pipe->x == - pSprite->w)
        {
            SDL_Delay(100);
            end = 0;
        }
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}

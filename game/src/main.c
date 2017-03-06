#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include <SDL2/SDL.h>

int main(int argc, char ** argv)
{
    //SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO != 0))
    {
        fprintf(stderr, "SDL initialzation   failure %s\n", SDL_GetError());
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
    SDL_Renderer * pRenderer = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED);
    /*if (pRenderer == NULL);
    {
        fprintf(stderr, "Renderer creation failure : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }*/



    //Parameters of the rectangle destination for the sprite
    Pipe * pipe = newPipe(400, 300,"./../../res/sprites/simplified pipe.bmp");
    if(pipe==NULL)
    {
        fprintf(stderr, "Pipe creation failure : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    //Texture creation
    SDL_Texture * pTexture = SDL_CreateTextureFromSurface(pRenderer, pipe->surface);
    if (pTexture == NULL)
    {
        fprintf(stderr, "Texture creation failure : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_Rect dest = {pipe->x, pipe->y, pipe->surface->w, pipe->surface->h};
    SDL_RenderCopy(pRenderer, pTexture, NULL, &dest);
    SDL_RenderPresent(pRenderer);



    //Game loop
    int end = 1;
    while(end)
    {
        SDL_RenderClear(pRenderer);
        dest.x = pipe->x;
        SDL_RenderCopy(pRenderer, pTexture, NULL, &dest);
        SDL_RenderPresent(pRenderer);
        SDL_Delay(5);
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return EXIT_SUCCESS;
}

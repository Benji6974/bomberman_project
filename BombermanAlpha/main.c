#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

int main(int agrc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_version version;
    SDL_VERSION(&version)

    SDL_Window *fenetre = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);


    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, 0);

    char message[50];
    sprintf(message, "Version SDL: %d.%d.%d", version.major, version.minor, version.patch);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, fenetre);

    int stop = 0;
    SDL_Event event;
    while(!stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;


    }

    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    return 0;
}

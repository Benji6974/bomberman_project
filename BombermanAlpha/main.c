#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#define graphismes 0

SDL_Texture* charger_sprite(SDL_Renderer *renderer, char *chemin)
{
    SDL_Texture *finale = NULL;
    SDL_Surface *image_chargee = SDL_LoadBMP(chemin);
    if(image_chargee != NULL)
    {
        finale = SDL_CreateTextureFromSurface(renderer, image_chargee);
        if(finale == NULL)
            printf("Erreur lors de la creation de %s\n", chemin);
        SDL_FreeSurface(image_chargee);
    }
    else
        printf("Erreur lors du chargement de %s\n", chemin);

    return finale;
}

int main(int agrc, char** argv)
{

    init_jeu();
    if (graphismes)
    {
        SDL_Init(SDL_INIT_VIDEO);

        SDL_version version;
        SDL_VERSION(&version)

        SDL_Window *fenetre = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);


        SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, 0);

        SDL_RendererInfo info;
        SDL_GetRenderDriverInfo(0, &info);

        char message[50];
        sprintf(message, "Version SDL: %d.%d.%d\nDriver: %s", version.major, version.minor, version.patch, info.name);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, fenetre);

        SDL_Texture *image = charger_sprite(renderer, "bomberman.bmp");
        SDL_Rect pos;
        pos.x = 30;
        pos.y = 30;
        pos.w = 128;
        pos.h = 128;

        int stop = 0, current_time = 0, previous_time = 0, frame_compte = 0;
        SDL_Event event;
        while(!stop)
        {
            SDL_PollEvent(&event);
            if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                stop = 1;

            switch(event.type)
            {
            case SDL_MOUSEMOTION:
                pos.x = event.motion.x - pos.w/2;
                pos.y = event.motion.y - pos.h/2;
                break;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, image, NULL, &pos);
            SDL_RenderPresent(renderer);

            current_time = SDL_GetTicks();
            frame_compte++;
            if(current_time - previous_time >= 1000)
            {
                printf("FPS: %d\n", frame_compte);
                frame_compte = 0;
                previous_time = current_time;
            }
        }

        SDL_DestroyTexture(image);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
    }

    return 0;
}

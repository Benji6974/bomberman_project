#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "constantes.h"
#include "jeu.h"
#include "graphismes.h"

#define graphismes 1

void affiche_carte(Tile ***carte)
{
    int i, j;
    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            printf("%d ", carte[i][j]->type);
        }
        printf("\n");
    }
}

int main(int agrc, char** argv)
{

    Game *jeu = init_jeu(0, 0, 0);

    affiche_carte(jeu->carte);

    #if graphismes
        /* Ceci est un test de la SDL2 */
        SDL_Init(SDL_INIT_VIDEO);

        SDL_version version;
        SDL_VERSION(&version)

        SDL_Window *fenetre = init_fenetre("Bomberman Alpha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN, 0);
        SDL_Renderer *renderer = SDL_GetRenderer(fenetre);

        SDL_RendererInfo info;
        SDL_GetRenderDriverInfo(0, &info);

        char message[50];
        sprintf(message, "Version SDL: %d.%d.%d\nMoteur de rendu: %s\nTaille de la fenetre: %d*%d\n", version.major, version.minor, version.patch, info.name, WINDOW_WIDTH, WINDOW_HEIGHT);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, fenetre);

        SDL_Texture *feuille_tiles = charger_sprite(renderer, "img/tileset.bmp");
        SDL_Rect pos;
        pos.x = 0;
        pos.y = 0;
        pos.w = TILE_WIDTH;
        pos.h = TILE_HEIGHT;

        /* Permet de récupérer une region spécifique de la feuille de tiles */
        SDL_Rect clip;
        clip.x = 0;
        clip.y = 0;
        clip.w = TILE_WIDTH;
        clip.h = TILE_HEIGHT;

        int stop = 0, current_time = 0, previous_time = 0, frame_compte = 0, i, j;
        SDL_Event event;
        while(!stop)
        {
            SDL_PollEvent(&event);
            if(event.window.event == SDL_WINDOWEVENT_CLOSE || event.type == SDL_KEYDOWN)
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

            /* Test dessin de la carte */
            for(i = 0; i < MAP_HEIGHT; i++)
            {
                for(j = 0; j < MAP_WIDTH; j++)
                {
                    pos.x = j*TILE_WIDTH;
                    pos.y = i*TILE_HEIGHT;

                    if(jeu->carte[i][j] == NULL)
                        continue;

                    switch(jeu->carte[i][j]->type)
                    {
                    /* Herbe */
                    default:
                    case 0:
                        clip.x = 0;
                        clip.y = 0;
                        break;
                    /* Mur indestructible */
                    case 1:
                        clip.x = 0;
                        clip.y = 10;
                        break;
                    /* Mur desctructible */
                    case 2:
                        clip.x = 0;
                        clip.y = 13;
                        break;
                    }

                    clip.x *= TILE_WIDTH;
                    clip.y *= TILE_HEIGHT;
                    SDL_RenderCopy(renderer, feuille_tiles, &clip, &pos);
                }
            }

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

        SDL_DestroyTexture(feuille_tiles);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
    #endif // graphismes

    detruire_jeu(jeu);
    return 0;
}

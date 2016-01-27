#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "constantes.h"
#include "jeu.h"
#include "graphismes.h"

#define GRAPHISMES 1

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

    Game *jeu = init_jeu(0, NB_JOUEURS, 0);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_version version;
    SDL_VERSION(&version)

    Graphismes *g = init_graphismes("Bomberman Beta",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    MAP_WIDTH*TILE_WIDTH,
                                    MAP_HEIGHT*TILE_HEIGHT,
                                    SDL_WINDOW_SHOWN,
                                    SDL_RENDERER_PRESENTVSYNC);

    SDL_RendererInfo info;
    SDL_GetRenderDriverInfo(0, &info);

    char message[50];
    sprintf(message, "Version SDL: %d.%d.%d\nMoteur de rendu: %s\nTaille de la fenetre: %d*%d\n", version.major, version.minor, version.patch, info.name, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, g->fenetre);


    int stop = 0, current_time = 0, previous_time = 0, previous_time2 = 0, frame_compte = 0, i;
    int dt = 0;
    SDL_Event event;
    while(!stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;

        switch(event.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            for(i = 0; i < jeu->touches.num_keys; i++)
            {
                if(event.key.keysym.sym == jeu->touches.key_map[i])
                    jeu->touches.keys_pressed[i] = (event.type == SDL_KEYDOWN) ? 1 : 0;
            }
            break;
        }

        /* MISE A JOUR DE L'ETAT DU JEU */

        current_time = SDL_GetTicks();
        dt = current_time - previous_time;
        if(dt >= 1000/MAJ_PAR_SEC)
        {
            stop |= maj_jeu(jeu, dt);
            previous_time = current_time;
        }

        /* MISE A JOUR DES GRAPHISMES */
    #if GRAPHISMES
        maj_graphismes(jeu, g);
    #endif // GRAPHISMES

        /* Compteur de FPS */
        current_time = SDL_GetTicks();
        frame_compte++;
        if(current_time - previous_time2 >= 1000)
        {
            printf("FPS: %d\n", frame_compte);
            frame_compte = 0;
            previous_time2 = current_time;
        }

    }

    free_graphismes(g);
    detruire_jeu(jeu);

    SDL_Quit();
    return 0;
}

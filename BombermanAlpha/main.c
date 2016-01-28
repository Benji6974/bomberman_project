#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "constantes.h"
#include "jeu.h"
#include "graphismes.h"

#define GRAPHISMES 1

void maj_controles(Controls *controles, SDL_Event *event)
{
    if(event->type != SDL_KEYDOWN && event->type != SDL_KEYUP)
        return;

    int i;
    for(i = 0; i < controles->num_keys; i++)
    {
        if(event->key.keysym.sym == controles->key_map[i])
            controles->keys_pressed[i] = event->type == SDL_KEYDOWN;
    }
}

int main(int agrc, char** argv)
{
    int pause = 0, stop = 0, pause_b = 0, current_time = 0, previous_time = 0, previous_time2 = 0, frame_compte = 0;
    int dt = 0;
    SDL_Event event;
    Game *jeu = NULL;
    Graphismes *g = NULL;
    char message[50];

    SDL_Init(SDL_INIT_VIDEO);

    SDL_version version;
    SDL_VERSION(&version)

    jeu = init_jeu(0, NB_JOUEURS, DUREE_DEFAUT_PARTIE);
    g   = init_graphismes("Bomberman Beta",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH,
                                    WINDOW_HEIGHT,
                                    SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE,
                                    SDL_RENDERER_PRESENTVSYNC);

    SDL_RendererInfo info;
    SDL_GetRenderDriverInfo(0, &info);

    sprintf(message, "Version SDL: %d.%d.%d\nMoteur de rendu: %s\nTaille de la fenetre: %d*%d\n", version.major, version.minor, version.patch, info.name, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, g->fenetre);

    while(!stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;

        maj_controles(&jeu->touches, &event);

        switch(event.type)
        {
        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
            case SDLK_p:
                pause_b = 0;
                break;
            default:;
            }
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                stop = 1;
                break;
            case SDLK_p:
                if(!pause_b)
                {
                    pause ^= 1;
                    pause_b = 1;
                }
            break;
            default:;
            }
            break;
        }

        /* MISE A JOUR DE L'ETAT DU JEU */

        current_time = SDL_GetTicks();
        dt = current_time - previous_time;
        if(dt >= 1000/MAJ_PAR_SEC && !pause)
        {
            stop |= maj_jeu(jeu, 1000/MAJ_PAR_SEC);
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

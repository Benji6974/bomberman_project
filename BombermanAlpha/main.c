#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "constantes.h"
#include "jeu.h"
#include "graphismes.h"
#include "sound.h"

#define DEMARRER_JEU 0

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





    init_audio(NB_CANAUX);

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
    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, g->fenetre);


    Menu *m = NULL;
    m = init_menu();



    while(!m->lancer_jeu)

    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;
        maj_menu(g,m->nb_joueurs,m->temps,m->map_jeu);
        maj_control_menu(&event,m);
        printf("nb_joueur %d",m->nb_joueurs);
    }


    jeu = init_jeu(0, m->nb_joueurs, m->temps,m->map_jeu);

    stop = 0;



    while(!stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;

        SDL_AddEventWatch(maj_audio, NULL);

        switch(event.type)
        {
        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
            case SDLK_p:
                pause_b = 0;
                break;
            default:
                ;
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
                    jeu->en_pause ^=1;
                    pause_b = 1;

                }
                break;
            default:
                ;
            }
            break;
        }

        /* MISE A JOUR DES CONTROLES */

        maj_controles(&jeu->touches, &event);

        /* MISE A JOUR DE L'ETAT DU JEU */

        current_time = SDL_GetTicks();
        dt = current_time - previous_time;
        if(dt >= 1000/MAJ_PAR_SEC && !pause)
        {
            stop |= maj_jeu(jeu, 1000/MAJ_PAR_SEC);
            previous_time = current_time;
        }

        /* MISE A JOUR DES GRAPHISMES */

        maj_graphismes(jeu, g);
        printf("%d\n", jeu->players[0]->score);


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
    detruire_audio();

    SDL_Quit();
    return 0;
}

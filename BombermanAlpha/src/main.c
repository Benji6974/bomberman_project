#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
    #include <SDL2/SDL.h>
#elif _WIN32
    #include <SDL.h>
#else

#endif

#include "constantes.h"
#include "jeu.h"
#include "graphismes.h"
#include "sound.h"
#include "menu.h"

#define DEMARRER_JEU 0

int main(int argc, char *argv[])
{
    int pause = 0, stop = 0, partie_terminee = 0, pause_b = 0, current_time = 0, previous_time = 0, previous_time2 = 0, frame_compte = 0;
    int dt = 0;
    SDL_Event event;
    Game *jeu = NULL;
    Graphismes *g = NULL;
    char message[50];

    SDL_Init(SDL_INIT_VIDEO);

    SDL_version version;
    SDL_VERSION(&version)





    init_audio(NB_CANAUX);

    g = init_graphismes("Bomberman",
                        SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED,
                        WINDOW_WIDTH,
                        WINDOW_HEIGHT,
                        SDL_WINDOW_SHOWN,
                        SDL_RENDERER_PRESENTVSYNC); /* on active la syncro verticale*/

    SDL_RendererInfo info;
    SDL_GetRenderDriverInfo(0, &info);

    sprintf(message, "Version SDL: %d.%d.%d\nMoteur de rendu: %s\nTaille de la fenetre: %d*%d\n", version.major, version.minor, version.patch, info.name, WINDOW_WIDTH, WINDOW_HEIGHT);
    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, g->fenetre);


    Menu *m = NULL;

    do
    {

    partie_terminee = 0;
    m = init_menu();
    while(!m->lancer_jeu && !stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;

        maj_menu(g,m->nb_joueurs,m->temps,m->map_jeu,m->volume_son);
        changer_volume(m->volume_son);
        maj_control_menu(&event,m);
    }
    jeu = init_jeu(0, m->nb_joueurs, m->temps, m->map_jeu);

    free(m);
    m = NULL;

    while(!partie_terminee && !stop)
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
                    jeu->en_pause ^=1;
                    pause_b = 1;
                }
                break;
            default:;
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
            partie_terminee = maj_jeu(jeu, 1000/MAJ_PAR_SEC);
            previous_time = current_time;
        }

        /* MISE A JOUR DES GRAPHISMES */

        maj_graphismes(jeu, g);

        if(partie_terminee)
        {
            char temps_ecoule[50] = "";

            if(jeu->nb_joueurs > 1)
            {
                if(jeu->time <= 0)
                    sprintf(temps_ecoule, "Temps ecoule!\n");
                if(jeu->gagnant != NULL)
                    sprintf(message, "%sGagnant: %s avec %d points!", temps_ecoule, jeu->gagnant->nom, jeu->gagnant->score);
                else
                    sprintf(message, "%sIl y a egalite!", temps_ecoule);
            }
            else /* Partie solo */
            {
                if(jeu->time <= 0)
                    sprintf(message, "Temps ecoule!\nVous avez perdu!");
                else if(jeu->players[0]->est_mort)
                    sprintf(message, "Vous etes mort!\nVous avez perdu!");
                else
                    sprintf(message, "Vous avez gagne!\n");
            }

            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", message, g->fenetre);
        }


        /* Compteur de FPS */
       /* current_time = SDL_GetTicks();
        frame_compte++;
        if(current_time - previous_time2 >= 1000)
        {
            printf("FPS: %d\n", frame_compte);
            frame_compte = 0;
            previous_time2 = current_time;
        }*/

    }

    SDL_DelEventWatch(maj_audio, NULL);
    detruire_jeu(jeu);
    jeu = NULL;

    }while(!stop);

    detruire_audio();
    free_graphismes(g);

    SDL_Quit();
    return 0;
}

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

/* Fonction utilisée pour trier le tableau des joueurs en fonction de leur position verticale
 * Cela permet de changer leur ordre d'affichage et ainsi quel joueur est affiché au dessus d'un autre
 */
void trier_par_y(Player **tab, int taille)
{
    int i, j;
    Player *tmp;
    for(i = 1; i < taille; i++)
    {
        j = i;
        while(j > 0 && tab[j-1]->pos.y > tab[j]->pos.y)
        {
            tmp = tab[j];
            tab[j] = tab[j-1];
            tab[j-1] = tmp;
            j--;
        }
    }
}

int main(int agrc, char** argv)
{

    Game *jeu = init_jeu(0, NB_JOUEURS, 0);

    affiche_carte(jeu->carte);

#if graphismes
    /* Ceci est un test de la SDL2 */
    SDL_Init(SDL_INIT_VIDEO);

    SDL_version version;
    SDL_VERSION(&version)

    SDL_Window *fenetre = init_fenetre("Bomberman Beta", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN, 0);
    SDL_Renderer *renderer = SDL_GetRenderer(fenetre);

    SDL_RendererInfo info;
    SDL_GetRenderDriverInfo(0, &info);

    char message[50];
    sprintf(message, "Version SDL: %d.%d.%d\nMoteur de rendu: %s\nTaille de la fenetre: %d*%d\n", version.major, version.minor, version.patch, info.name, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, fenetre);

    SDL_Texture *feuille_tiles = charger_sprite(renderer, "img/tileset.bmp");
    SDL_Texture *feuille_perso = charger_sprite(renderer, "img/character_silver.bmp");
    SDL_Texture *feuille_objets = charger_sprite(renderer, "img/atlas5_32.bmp");

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

    SDL_Rect clip_perso;
    clip_perso.x = PLAYER_INDEX_X;
    clip_perso.y = PLAYER_INDEX_Y;
    clip_perso.w = 23;
    clip_perso.h = 38;

    SDL_Rect pos_perso;
    pos_perso.x = 0;
    pos_perso.y = 0;
    pos_perso.w = 23;
    pos_perso.h = 38;

    int stop = 0, current_time = 0, previous_time = 0, previous_time2 = 0, frame_compte = 0, i, j, en_vie = 0, joueur = 0;
    int dt = 0;
    SDL_Event event;

    SDL_Rect pos_pres;

    Player **blit_order = malloc(jeu->nb_joueurs*sizeof(int));

    while(!stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;

        switch(event.type)
        {
        case SDL_MOUSEMOTION:
            /*
            pos_pres = jeu->players[0]->pos;
            jeu->players[0]->pos.x = event.motion.x - pos.w/2;
            jeu->players[0]->pos.y = event.motion.y - pos.h/2;
            if(collision_joueur(jeu, 0))
                jeu->players[0]->pos = pos_pres;
            */
            break;
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
            stop = update_jeu(jeu, dt);
            previous_time = current_time;
        }

        /* ----------------------------- */

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* ----- AFFICHAGE DE LA CARTE ----- */

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
                case MUR_METAL:
                    clip.x = 6;
                    clip.y = 12;
                    break;
                /* Herbe */
                default:
                case HERBE:
                    clip.x = 0;
                    clip.y = 0;
                    break;
                /* Mur indestructible */
                case MUR_INDESTRUCTIBLE:
                    clip.x = 0;
                    clip.y = 10;
                    break;
                /* Mur desctructible */
                case MUR_BRIQUES:
                    clip.x = 0;
                    clip.y = 13;
                    /* Sprite alternatif si la case juste en dessous n'est pas vide */
                    if(i+1 < MAP_HEIGHT && jeu->carte[i+1][j] != NULL && jeu->carte[i+1][j]->type == 0)
                        clip.x = 1;
                    break;
                /* Mur desctructible a plusieurs états */
                case MUR_SOLIDE:
                    clip.x = 2*((4-jeu->carte[i][j]->etat)-1);
                    clip.y = 12;
                    if(i+1 < MAP_HEIGHT && jeu->carte[i+1][j] != NULL && jeu->carte[i+1][j]->type == 0)
                        clip.x++;
                    break;
                }

                clip.x *= TILE_WIDTH;
                clip.y *= TILE_HEIGHT;
                SDL_RenderCopy(renderer, feuille_tiles, &clip, &pos);
            }
        }

        /* ----- AFFICHAGE DES BOMBES ----- */

        for(i = 0; i < jeu->nb_bombs; i++)
        {
            clip.x = BOMB_INDEX_X*TILE_WIDTH;
            clip.y = BOMB_INDEX_Y*TILE_HEIGHT;
            pos.x = jeu->bombs[i]->pos.x*TILE_WIDTH;
            pos.y = jeu->bombs[i]->pos.y*TILE_HEIGHT;
            SDL_RenderCopy(renderer, feuille_objets, &clip, &pos);
        }

        /* ----- AFFICHAGE DES JOUEURS ----- */

        memcpy(blit_order, jeu->players, jeu->nb_joueurs*sizeof(int));
        trier_par_y(blit_order, jeu->nb_joueurs);

        for(i = 0; i < jeu->nb_joueurs; i++)
        {
            switch(blit_order[i]->direction)
            {
            case DOWN:
                clip_perso.y = 0;
                break;
            case RIGHT:
                clip_perso.y = 38;
                break;
            case UP:
                clip_perso.y = 2*38;
                break;
            case LEFT:
                clip_perso.y = 3*38;
                break;
            }
            pos_perso.x = blit_order[i]->pos.x;
            pos_perso.y = blit_order[i]->pos.y - (38 - HITBOX_PLAYER_H);

            if(blit_order[i]->vie > 0)
                SDL_RenderCopy(renderer, feuille_perso, &clip_perso, &pos_perso);
        }

        /* ----- FIN AFFICHAGE ----- */

        SDL_RenderPresent(renderer);

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

    SDL_DestroyTexture(feuille_tiles);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
#endif // graphismes

    detruire_jeu(jeu);
    free(blit_order);
    return 0;
}

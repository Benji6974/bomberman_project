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

    Game *jeu = init_jeu(0, 2, 0);

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
    SDL_Texture *feuille_perso = charger_sprite(renderer, "img/character_silver.bmp");
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
    clip_perso.x = 0;
    clip_perso.y = 0;
    clip_perso.w = 23;
    clip_perso.h = 38;

    SDL_Rect pos_perso;
    pos_perso.x = 0;
    pos_perso.y = 0;
    pos_perso.w = 23;
    pos_perso.h = 38;

    int stop = 0, current_time = 0, previous_time = 0, frame_compte = 0, i, j;
    SDL_Event event;
    while(!stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;
        int i;
        for (i=0; i< jeu->nb_joueurs; i++)
        {
            maj_player(jeu,&event,jeu->players[i]);
        }

        // faire une boucle for

        switch(event.type)
        {
        case SDL_MOUSEMOTION:
            pos.x = event.motion.x - pos.w/2;
            pos.y = event.motion.y - pos.h/2;
            break;
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

        for(i = 0; i < jeu->nb_joueurs; i++)
        {
            pos_perso.x = jeu->players[i]->pos.x;
            pos_perso.y = jeu->players[i]->pos.y - 16;
            SDL_RenderCopy(renderer, feuille_perso, &clip_perso, &pos_perso);
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
        //system("cls");
        //printf("pos x : %d pos y : %d\n",jeu->players[0]->pos.x,jeu->players[0]->pos.y);

    }

    SDL_DestroyTexture(feuille_tiles);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
#endif // graphismes

    detruire_jeu(jeu);
    return 0;
}

int collision(Game* jeu,Player * p)
{
    int i,x,y,res;
    int posjoueurmapx ;
    int posjoueurmapy;
    int posjoueurmapx2 ;
    int posjoueurmapy2;
    switch(p->direction)
    {
    //
    case RIGHT:
        posjoueurmapx = (p->pos.x+p->pos.w+1)/TILE_WIDTH;
        posjoueurmapy = (p->pos.y)/TILE_HEIGHT;
        posjoueurmapy2 = (p->pos.y+p->pos.h)/TILE_HEIGHT;
        if (p->pos.x+p->pos.w+1 >= TILE_WIDTH*MAP_WIDTH || (jeu->carte[posjoueurmapy][posjoueurmapx]->type != 0 || jeu->carte[posjoueurmapy2][posjoueurmapx]->type != 0))
            return 1;
        break;
    case LEFT:
        posjoueurmapx = (p->pos.x-1)/TILE_WIDTH;
        posjoueurmapy = (p->pos.y)/TILE_HEIGHT;
        posjoueurmapy2 = (p->pos.y+p->pos.h)/TILE_HEIGHT;
        if (p->pos.x-1 < 0 || (jeu->carte[posjoueurmapy][posjoueurmapx]->type != 0 || jeu->carte[posjoueurmapy2][posjoueurmapx]->type != 0))
            return 1;
        break;
    case UP:
        posjoueurmapx = (p->pos.x)/TILE_WIDTH;
        posjoueurmapx2 = (p->pos.x+p->pos.w)/TILE_WIDTH;
        posjoueurmapy = (p->pos.y-1)/TILE_HEIGHT;
        if (p->pos.y-1 < 0 || (jeu->carte[posjoueurmapy][posjoueurmapx]->type != 0 || jeu->carte[posjoueurmapy][posjoueurmapx2]->type != 0))
            return 1;
        break;
    case DOWN:
        posjoueurmapx = (p->pos.x)/TILE_WIDTH;
        posjoueurmapx2 = (p->pos.x+p->pos.w)/TILE_WIDTH;
        posjoueurmapy = (p->pos.y+p->pos.h+1)/TILE_HEIGHT;
        if (p->pos.y+p->pos.h+1 >= TILE_HEIGHT*MAP_HEIGHT || (jeu->carte[posjoueurmapy][posjoueurmapx]->type != 0 || jeu->carte[posjoueurmapy][posjoueurmapx2]->type != 0))
            return 1;
        break;
    }
    //printf("%d / %d = x = %d\n",p->pos.x,TILE_WIDTH,posjoueurmapx);
    //printf("%d / %d = y = %d\n",p->pos.y,TILE_HEIGHT,posjoueurmapy);

    return 0;

}
void maj_player(Game* jeu,SDL_Event* event,Player* p)
{


    int res_collision=0;
    switch(event->type)
    {

    case SDL_KEYDOWN:
        if (p->id == 0)
        {
            switch(event->key.keysym.sym)
            {
            case SDLK_RIGHT:
                p->direction = RIGHT;
                p->se_deplace = 1;

                break;
            case SDLK_LEFT:
                p->direction = LEFT;
                p->se_deplace = 1;
                break;
            case SDLK_UP:
                p->direction = UP;
                p->se_deplace = 1;
                break;
            case SDLK_DOWN:
                p->direction = DOWN;
                p->se_deplace = 1;
                break;
            default:
                return;
                break;
            }

        }
        else if (p->id== 1)
        {
            switch(event->key.keysym.sym)
            {
            case SDLK_d:
                p->direction = RIGHT;
                p->se_deplace = 1;
                break;
            case SDLK_q:
                p->direction = LEFT;
                p->se_deplace = 1;
                break;
            case SDLK_z:
                p->direction = UP;
                p->se_deplace = 1;
                break;
            case SDLK_s:
                p->direction = DOWN;
                p->se_deplace = 1;
                break;
            default:
                return;
                break;
            }
        }
        if(p->se_deplace)
        {
            res_collision = collision(jeu,p);
            printf("colision = %d direction = %d\n",res_collision,p->direction);
            if (res_collision != 1)
                if (res_collision == 0)
                    deplacement(p);
        }
        break;

    case SDL_KEYUP:

        if (p->id == 0)
        {
            switch(event->key.keysym.sym)
            {
            case SDLK_RIGHT:
                p->direction = RIGHT;
                p->se_deplace = 0;
                break;
            case SDLK_LEFT:
                p->direction = LEFT;
                p->se_deplace = 0;
                break;
            case SDLK_UP:
                p->direction = RIGHT;
                p->se_deplace = 0;
                break;
            case SDLK_DOWN:
                p->direction = RIGHT;
                p->se_deplace = 0;
                break;
            default:
                return;
                break;
            }
        }
        else if (p->id== 1)
        {
            switch(event->key.keysym.sym)
            {
            case SDLK_d:
                p->direction = RIGHT;
                p->se_deplace = 0;
                break;
            case SDLK_q:
                p->direction = LEFT;
                p->se_deplace = 0;
                break;
            case SDLK_z:
                p->direction = UP;
                p->se_deplace = 0;
                break;
            case SDLK_s:
                p->direction = DOWN;
                p->se_deplace = 0;
                break;
            default:
                return;
                break;
            }
        }
        break;



    }
}
    void deplacement(Player * p)
    {
        switch(p->direction)
        {
        case RIGHT:
            p->pos.x++;
            break;
        case LEFT:
            p->pos.x--;
            break;
        case UP:
            p->pos.y--;
            break;
        case DOWN:
            p->pos.y++;
            break;

        }
    }

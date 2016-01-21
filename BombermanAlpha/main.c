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

/* Verification collisions rectangle-rectangle avec le décor */
int collision_joueur_decor(Game *jeu, int joueur)
{
    int i=0, x=0, y=0, x1=0, x2=0, y1=0, y2=0;
    const SDL_Rect pos = jeu->players[joueur]->pos, *pos_bomb;

    x1 = pos.x/TILE_WIDTH;
    y1 = pos.y/TILE_HEIGHT;
    x2 = (pos.x + pos.w - 1)/TILE_WIDTH;
    y2 = (pos.y + pos.h - 1)/TILE_HEIGHT;

    for(x = x1; x <= x2; x++)
    {
        for(y = y1; y <= y2; y++)
        {
            if(jeu->carte[y][x]->type != 0)
            {
                return 1;
            }

            for(i = 0; i < jeu->nb_bombs; i++)
            {

                pos_bomb = &jeu->bombs[i]->pos;

                if(x == pos_bomb->x && y == pos_bomb->y)
                {
                    if(jeu->bombs[i]->posee)
                        return 0;
                    else
                        return 1;
                }
                else
                {
                    jeu->bombs[i]->posee = 0;
                    return 0;
                }

            }
        }
    }
    return 0;
}

int collision_joueur_objet(Game *jeu, int joueur)
{
    int i=0, x=0, y=0, x1=0, x2=0, y1=0, y2=0;
    SDL_Rect *pos_bomb;
    Player *p = jeu->players[joueur];

    x1 = p->pos.x/TILE_WIDTH;
    y1 = p->pos.y/TILE_HEIGHT;
    x2 = (p->pos.x + p->pos.w)/TILE_WIDTH;
    y2 = (p->pos.y + p->pos.h)/TILE_HEIGHT;

    /* Collision bombe */
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        pos_bomb = &jeu->bombs[i]->pos;
        for(x = x1; x <= x2; x++)
        {
            for(y = y1; y <= y2; y++)
            {
                if(x == pos_bomb->x && y == pos_bomb->y)
                    return 1;
            }
        }

    }
    return 0;
}

int poser_bomb(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    int i = 0;
    if (p->nb_bomb_jeu < p->nb_bomb_max)
    {
        Bomb *b = malloc(sizeof(Bomb));
        *b = *(p->typebomb);
        int x1 = p->pos.x/TILE_WIDTH;
        int y1 = p->pos.y/TILE_HEIGHT;
        b->pos.x = x1;
        b->pos.y = y1;

        for(i = 0; jeu->bombs[i] != NULL; i++)
        {
            if(jeu->bombs[i]->pos.x == b->pos.x && jeu->bombs[i]->pos.y == b->pos.y)
            {
                free(b);
                return;
            }
        }
        jeu->bombs[i] = b;
        jeu->nb_bombs++;
        p->nb_bomb_jeu++;
        b->posee = 1;
    }

}
void deplacer_joueur(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    int k = p->keymap_offset;

    /* On déplace le joueur en fonction des touches appuyées */
    if(jeu->touches.keys_pressed[k])
    {
        p->pos.y -= p->vitesse;
        while(collision_joueur_decor(jeu, joueur)) /* tant qu'il y a collision, on annule le déplacement de manière incrémentielle */
            p->pos.y++;
    }
    if(jeu->touches.keys_pressed[k+1])
    {
        p->pos.y += p->vitesse;
        while(collision_joueur_decor(jeu, joueur))
            p->pos.y--;
    }
    if(jeu->touches.keys_pressed[k+2])
    {
        p->pos.x -= p->vitesse;
        while(collision_joueur_decor(jeu, joueur))
            p->pos.x++;
    }
    if(jeu->touches.keys_pressed[k+3])
    {
        p->pos.x += p->vitesse;
        while(collision_joueur_decor(jeu, joueur))
            p->pos.x--;
    }
    if(jeu->touches.keys_pressed[k+4])
    {
        poser_bomb(jeu, joueur);
    }
}

int main(int agrc, char** argv)
{

    Game *jeu = init_jeu(0, 3, 0);

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

    SDL_Rect pos_pres;

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

        for(i = 0; i < jeu->nb_joueurs; i++)
        {
            deplacer_joueur(jeu, i);
        }

        SDL_Delay(4);


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

        for(i = 0; i < jeu->nb_bombs; i++)
        {
            clip.x = 2*TILE_WIDTH;
            clip.y = 0*TILE_HEIGHT;
            pos.x = jeu->bombs[i]->pos.x*TILE_WIDTH;
            pos.y = jeu->bombs[i]->pos.y*TILE_HEIGHT;
            SDL_RenderCopy(renderer, feuille_objets, &clip, &pos);
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



/* Quel bazar!

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

*/

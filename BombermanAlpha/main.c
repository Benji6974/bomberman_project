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

/* Verification collisions rectangle-rectangle avec le d�cor
 * Renvoie 1 si la collision est totale, 2 si elle est partielle
 */
int collision_joueur_decor(Game *jeu, int joueur)
{
    if(!ACTIVER_COLLISIONS)
        return 0;

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
            /* test collision avec les blocs de la carte */
            if(jeu->carte[y][x]->type != 0)
            {
                return 1;

            }
        }
    }

    return 0;
}

int collision_joueur_objets(Game *jeu, int joueur)
{
    if(!ACTIVER_COLLISIONS)
        return 0;

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
            /* test collision avec les bombes */
            for(i = 0; i < jeu->nb_bombs; i++)
            {
                pos_bomb = &jeu->bombs[i]->pos;
                if(x == pos_bomb->x && y == pos_bomb->y)
                {
                    return jeu->players[joueur]->id_player;
                }
            }
        }
    }

    return 0;
}

int poser_bomb(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    int i = 0;
    if (p->nb_bomb_jeu < p->nb_bomb_max && jeu->nb_bombs < NB_BOMBES_MAX)
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
    }

}
void deplacer_joueur(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    int k = p->keymap_offset, collision_decor = 0, collision_objets = 0;
    int move_x = 0, move_y = 0;

    collision_decor = collision_joueur_decor(jeu, joueur);
    collision_objets = collision_joueur_objets(jeu, joueur);

    /* On d�place le joueur en fonction des touches appuy�es */
    if(jeu->touches.keys_pressed[k])
        move_y += -1;
    if(jeu->touches.keys_pressed[k+1])
        move_y += 1;
    if(jeu->touches.keys_pressed[k+2])
        move_x += -1;
    if(jeu->touches.keys_pressed[k+3])
        move_x += 1;

    p->pos.x += move_x*p->vitesse;
    while(move_x
        && (collision_joueur_decor(jeu, joueur) && !collision_decor
        || collision_joueur_objets(jeu, joueur) && !collision_objets))
        p->pos.x -= move_x;

    p->pos.y += move_y*p->vitesse;
    while(move_y
        && (collision_joueur_decor(jeu, joueur) && !collision_decor
        || collision_joueur_objets(jeu, joueur) && !collision_objets))
        p->pos.y -= move_y;

    /* Si le joueur appuie sur la touche pour poser une bombe */
    if(jeu->touches.keys_pressed[k+4])
        poser_bomb(jeu, joueur);
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

    /* Permet de r�cup�rer une region sp�cifique de la feuille de tiles */
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

        for(i = 0; i < jeu->nb_bombs; i++)
        {
            clip.x = 2*TILE_WIDTH;
            clip.y = 0*TILE_HEIGHT;
            pos.x = jeu->bombs[i]->pos.x*TILE_WIDTH;
            pos.y = jeu->bombs[i]->pos.y*TILE_HEIGHT;
            SDL_RenderCopy(renderer, feuille_objets, &clip, &pos);
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

    }

    SDL_DestroyTexture(feuille_tiles);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
#endif // graphismes

    detruire_jeu(jeu);
    return 0;
}

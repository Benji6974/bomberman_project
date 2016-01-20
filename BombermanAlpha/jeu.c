#include <stdlib.h>
#include <stdio.h>

#include "constantes.h"
#include "jeu.h"

int gKeys[KEYS_PER_PLAYER*3] = {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_z, SDLK_s, SDLK_q, SDLK_d, SDLK_t, SDLK_g, SDLK_f, SDLK_h};

Game* init_jeu(int type, int nb_joueurs, int temps)
{
    Game *jeu;
    Tile ***carte;
    int i, j;

    /* - Tableau pour tester la carte - */

    int carte_data[TILE_HEIGHT][TILE_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 1, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 2, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 2, 1, 2, 1, 0, 1, 2, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    /* -------------------------------- */

    jeu = malloc(sizeof(Game));

    jeu->type = type;
    jeu->time = temps;
    jeu->nb_joueurs = nb_joueurs;

    /* Génération de la carte */
    carte = (Tile***)malloc(MAP_HEIGHT*sizeof(Tile**)); /* tableau 2D de pointeurs sur Tiles */
    for(i = 0; i < MAP_HEIGHT; i++)
    {
        carte[i] = malloc(MAP_WIDTH*sizeof(Tile*));
        for(j = 0; j < MAP_WIDTH; j++)
        {
            carte[i][j] = malloc(sizeof(Tile));
            carte[i][j]->type = carte_data[i][j];
        }
    }

    jeu->carte = carte;

    /* Initialisation controles */
    jeu->touches.num_keys = KEYS_PER_PLAYER*jeu->nb_joueurs;
    jeu->touches.key_map = malloc(jeu->touches.num_keys*sizeof(int));
    jeu->touches.keys_pressed = calloc(jeu->touches.num_keys, sizeof(int));

    for(i = 0; i < jeu->touches.num_keys; i++)
    {
        jeu->touches.key_map[i] = gKeys[i];
    }

    /* Generation du/des players*/

    jeu->players = (Player**)malloc(nb_joueurs*sizeof(Player*));
    for(i = 0; i < nb_joueurs; i++)
    {
        jeu->players[i] = init_player("Joueur");
        jeu->players[i]->keymap_offset = i*KEYS_PER_PLAYER;
        if (i==1)
        {
            jeu->players[i]->pos.x = TILE_WIDTH*(MAP_WIDTH-2);
        }
        if(i==2)
        {
            jeu->players[i]->pos.y = TILE_HEIGHT*9;
        }
    }

    return jeu;
}

void detruire_jeu(Game* jeu)
{
    int i, j;

    /* Vidage de la carte */
    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            free(jeu->carte[i][j]);
        }
        free(jeu->carte[i]);
    }
    free(jeu->carte);

    /* Libération du tableau des joueurs */
    for(i = 0; i < jeu->nb_joueurs; i++)
        free(jeu->players[i]);

    free(jeu->touches.key_map);
    free(jeu->touches.keys_pressed);

    /* Destruction du jeu */
    free(jeu);
}

void init_bomb(Bomb* b,int type, int puissance, int sprite_index, int sprite_no, int x , int y)
{
    b->type = type;
    b->puissance = puissance;
    b->pos.x = x;
    b->pos.y = y;
}

void init_tile(Tile* t,int type, int etat, int sprite_index,int sprite_no)
{
    t->type = type;
    t->etat = etat;
}

Player* init_player(char *name)
{
    Player *p = malloc(sizeof(Player));
    //strcpy(name, p->nom);
    p->vie = 1;
    p->score = 0;
    p->bouclier = 0;
    p->vitesse = 1; /* pixels par seconde */

    p->pos.x = TILE_WIDTH;
    p->pos.y = TILE_HEIGHT;
    p->pos.h = HITBOX_PLAYER;
    p->pos.w = HITBOX_PLAYER;

    return p;
}

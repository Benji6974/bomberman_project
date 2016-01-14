#include <stdlib.h>
#include <stdio.h>

#include "constantes.h"
#include "jeu.h"


Game* init_jeu(int type, int nb_joueurs, int temps)
{
    Game *jeu;
    Tile ***carte;
    int i, j;

    /* - Tableau pour tester la carte - */

    int carte_data[TILE_HEIGHT][TILE_WIDTH] = {
        {0},
        {0, 1, 2, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0, 1, 0},
        {0, 2, 2, 2, 2, 2},
        {2, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0},
        {0, 1, 0, 1, 2, 1, 2, 1, 0, 1, 2, 1, 0, 1, 0},
        {0},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {0}
    };

    /* -------------------------------- */

    jeu = malloc(sizeof(Game));

    jeu->type = type;
    jeu->time = temps;

    /* Génération de la carte */
    carte = (int***)malloc(MAP_HEIGHT*sizeof(Tile**)); /* tableau 2D de pointeurs sur Tiles */
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
    /* A faire */

    /* Destruction du jeu */
    free(jeu);
}

/*void affiche_jeu(Tile** t)
{
    for (i=0; i<map_x ; i++)
    {
        for (y=0; y< map_y; y++)
        {
            printf(t->type);
        }
        printf("\n");
    }
}*/ //marche pas <- (JM) bah oui ça marche pas car Tile n'existe que dans la boucle for dans init_jeu, faudrait faire une alloc dynamique je pense

void init_bomb(Bomb* b,int type, int puissance, int sprite_index, int sprite_no, int x , int y)
{
    b->type = type;
    b->puissance = puissance;
//    b->sprite_index = sprite_index;
//    b->sprite_no = sprite_no;
    b->pos.x = x;
    b->pos.y = y;
}

void init_tile(Tile* t,int type, int etat, int sprite_index,int sprite_no)
{
    t->type = type;
    t->etat = etat;
//    t->sprite_index = sprite_index;
//    t->sprite_no = sprite_no;
}

void init_player(Player* p ,char* name,int vie,int score, int bouclier, int sprite_index,int sprite_no,int x,int y)
{
    strcpy(name, p->nom);
    p->vie = vie;
    p->score = score;
    p->bouclier = bouclier;
//    p->sprite_index = sprite_index;
//    p->sprite_no = sprite_no;
    p->pos.x = x;
    p->pos.y = y;
}

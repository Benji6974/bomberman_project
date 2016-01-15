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
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 2, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1},
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



    /* Generation du/des players*/

    jeu->players = (Player**)malloc(nb_joueurs*sizeof(Player*));
    for(i = 0; i < nb_joueurs; i++)
    {
        jeu->players[i] = init_player("Joueur",i);
        if (i==1)
        {
            jeu->players[i]->pos.y= TILE_HEIGHT*9;
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

Player* init_player(char *name, int id)
{
    Player *p = malloc(sizeof(Player));
    p->id = id;
    //strcpy(name, p->nom);
    p->vie = 1;
    p->score = 0;
    p->bouclier = 0;
    p->se_deplace = 0;
//    p->sprite_index = sprite_index;
//    p->sprite_no = sprite_no;
    p->pos.x = TILE_WIDTH;
    p->pos.y = TILE_HEIGHT;
    p->pos.h = HITBOX_PLAYER;
    p->pos.w = HITBOX_PLAYER;


    return p;
}

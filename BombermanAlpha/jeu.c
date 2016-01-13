#include <stdlib.h>
#include <stdio.h>
#include "jeu.h"


#define map_x 50
#define map_y 50
int init_jeu()
{
    Tile *map[map_x][map_y];
    int i,y;
    for (i=0; i<map_x ; i++)
    {
        for (y=0; y< map_y; y++)
        {
            Tile *t = malloc(sizeof(Tile));
            init_tile(t,0,0,0,0);
            map[i][y] = t;
        }
    }
    printf("coucou");
    //affiche_jeu(&map);
    return 1;


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

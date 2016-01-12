#ifndef JEU_H
#define JEU_H

#include "graphismes.h"

typedef struct Coordonnees
{
    int x, y;
}
Coordonnees;

typedef struct Player{
    char nom[256];
    int vie;
    int score;
    int bouclier;
    Sprite sprite;
    Coordonnees pos;
}
Player;

typedef struct Bomb{
    int type;
    int puissance;
    Sprite sprite;
    Coordonnees pos;
}
Bomb;

typedef struct Tile{
    int type;
    int etat;
    Sprite sprite;
}
Tile;

typedef struct Game{
    int type;
    Player* players;
    int time;
    Tile** table;
}Game;

void init_tile();
void init_bomb();
void init_player();
int init_game();
void affiche_jeu();

#endif // JEU_H

#ifndef JEU_H
#define JEU_H

#include "graphismes.h"

typedef struct Player{
    char nom[256];
    int vie;
    int score;
    int bouclier;
    int direction;
    int se_deplace;
    int vitesse;
    SDL_Rect pos;
    int id;
}
Player;

typedef struct Bomb{
    int type;
    int puissance;
    int delai;
    Sprite sprite;
    SDL_Rect pos;
}
Bomb;

typedef struct Tile{
    int type; // faire enumeration
    int etat; /* pour les murs à plusieurs états de destruction */
}
Tile;

typedef struct Game{
    int type;
    Player** players;
    int nb_joueurs;
    int time;
    Tile*** carte;
}Game;

Game* init_jeu(int type, int nb_joueurs, int temps);

int update_jeu(Game *jeu, int tick);

void init_tile();
void init_bomb();
Player* init_player();
void affiche_jeu();

void detruire_jeu(Game *jeu);

#endif // JEU_H

#ifndef JEU_H
#define JEU_H

#include "graphismes.h"



typedef struct Bomb{
    int type;
    int puissance;
    int delai;
    Sprite sprite;
    SDL_Rect pos;
    int id_proprietaire;
    int posee;
}
Bomb;

typedef struct Player{
    int id_player;
    char nom[256];
    int vie;
    int score;
    int bouclier;
    int direction;
    int vitesse;
    SDL_Rect pos;
    int keymap_offset; /* Touches à assigner au joueur */
    Bomb* typebomb;
    int nb_bomb_max;
    int nb_bomb_jeu;
}
Player;

typedef struct Tile{
    int type; // faire enumeration
    int etat; /* pour les murs à plusieurs états de destruction */
}
Tile;

/* Structure gérant les touches claviers */
typedef struct Controls{
    int num_keys;      /* nombre de touches à gérer */
    int *keys_pressed; /* touches actuellement pressées */
    int *key_map;      /* ID de chaque touches (ex. SDLK_UP); correspond avec keys_pressed */
}Controls;

typedef struct Game{
    int type;
    Player** players;
    int nb_joueurs;
    int time;
    Tile*** carte;
    Controls touches;
    Bomb** bombs;
    int nb_bombs;
}Game;

Game* init_jeu(int type, int nb_joueurs, int temps);

int update_jeu(Game *jeu, int tick);

void init_tile(Tile* t,int type, int etat, int sprite_index,int sprite_no);
Bomb* init_bomb(int type, int id_player);
Player* init_player(char *name, int id_player);
void affiche_jeu();

void detruire_jeu(Game *jeu);

#endif // JEU_H

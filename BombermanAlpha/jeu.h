#ifndef JEU_H
#define JEU_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>

/* Structures du jeu */

typedef struct Bomb{
    int type;
    int puissance;        /* Nombre de cases que peut atteindre la bombe dans chaque directions */
    int delai;            /* Temps en millisecondes avant explosion */
    SDL_Rect pos;
    int id_proprietaire;
}
Bomb;

typedef struct Player{
    int id_player;
    char nom[256];
    int vie;
    int est_mort;
    int score;
    int bouclier;
    int direction;
    int vitesse;
    SDL_Rect pos;
    int keymap_offset; /* Touches à assigner au joueur */
    Bomb typebomb;    /* Type de bombes à créer lorsqu'on appele poser_bomb */
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
    int *key_map;      /* ID de chaque touches (ex. SDLK_UP); correspond avec keys_pressed (même taille) */
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

/* Prototypes des fonctions */

/* Initialiser le jeu: carte, joueurs, etc */
Game* init_jeu(int type, int nb_joueurs, int temps);
/* mettre à jour le jeu de dt millisecondes */
int maj_jeu(Game *jeu, int dt);

/* fonctions de collisions avec carte et entre entités */
int collision_tile_rect(int x, int y, SDL_Rect rect);
int collision_joueur_objets(Game *jeu, int joueur, int last_col);
int collision_joueur_decor(Game *jeu, int joueur);

/* Fonctions sur les bombes */
Bomb* init_bomb(int type, int id_player);
int degats_case(Game *jeu, int x, int y);
int exploser_bombe(Game *jeu, int bombe);
int poser_bomb(Game *jeu, int joueur);
void maj_bombs(Game *jeu, int dt);

/* Fonctions sur les joueurs */
Player* init_player(char *name, int id_player);
void maj_joueur(Game *jeu, int joueur);

void init_tile(Tile* t,int type, int etat);

void detruire_jeu(Game *jeu);

#endif // JEU_H

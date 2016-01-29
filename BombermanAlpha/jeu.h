#ifndef JEU_H
#define JEU_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
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
    int keymap_offset; /* Touches � assigner au joueur */
    Bomb typebomb;    /* Type de bombes � cr�er lorsqu'on appele poser_bomb */
    int nb_bomb_max;
    int nb_bomb_jeu;
}
Player;

typedef struct Tile{
    int type; // faire enumeration
    int etat; /* pour les murs � plusieurs �tats de destruction */
}
Tile;

/* Structure g�rant les touches claviers */
typedef struct Controls{
    int num_keys;      /* nombre de touches � g�rer */
    int *keys_pressed; /* touches actuellement press�es */
    int *key_map;      /* ID de chaque touches (ex. SDLK_UP); correspond avec keys_pressed (m�me taille) */
}Controls;

typedef struct Objet{
    int type;
    SDL_Rect pos;
}Objet;

typedef struct Game{
    int type;
    Player** players;
    int nb_joueurs;
    int time;
    Tile*** carte;
    Controls touches;
    Bomb** bombs;
    int nb_bombs;
    Objet** objets;
    int nb_objets;
    SDL_Event **events;
}Game;


/* Prototypes des fonctions */

/* Initialiser le jeu: carte, joueurs, etc */
Game* init_jeu(int type, int nb_joueurs, int temps,int typemap);
SDL_Event** init_events(int);
/* mettre � jour le jeu de dt millisecondes */
int maj_jeu(Game *jeu, int dt);

/* fonctions de collisions avec carte et entre entit�s */
int collision_tile_rect(int x, int y, SDL_Rect rect);
int collision_rect_rect(SDL_Rect a, SDL_Rect b);
int collision_joueur_objets(Game *jeu, int joueur, int last_col);
int collision_joueur_decor(Game *jeu, int joueur);
int collision_joueur_items(Game*, int);

/* Fonctions sur les bombes */
Bomb* init_bomb(int type, int id_player);
int degats_case(Game *jeu, Bomb *origine, int x, int y);
int exploser_bombe(Game *jeu, int bombe);
int poser_bomb(Game *jeu, int joueur);
void maj_bombs(Game *jeu, int dt);

/* Fnctions sur les objets*/
Objet* init_objet(int type);
void generer_bonus(Game*, int, int, int);
int donner_bonus(Game*, int, int);

/* Fonctions sur les joueurs */
Player* init_player(char *name, int id_player);
void maj_joueur(Game *jeu, int joueur);
void maj_controles(Controls*, SDL_Event*);

void init_tile(Tile* t,int type, int etat);
int** genere_map(int **carte_data, int nb_joueurs);

void detruire_jeu(Game *jeu);

#endif // JEU_H

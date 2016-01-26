#ifndef CONSTANTES_H
#define CONSTANTES_H

/* Taille de la carte et de la fenêtre */
#define TILE_WIDTH    32
#define TILE_HEIGHT   32
#define MAP_WIDTH     17
#define MAP_HEIGHT    11
#define WINDOW_WIDTH  TILE_WIDTH*MAP_WIDTH
#define WINDOW_HEIGHT TILE_HEIGHT*MAP_HEIGHT
#define HITBOX_PLAYER_W 23
#define HITBOX_PLAYER_H 15

/* Index sprites */
#define PLAYER_INDEX_X 0
#define PLAYER_INDEX_Y 0

#define BOMB_INDEX_X 2
#define BOMB_INDEX_Y 0

#define TILE_INDESTRUCTIBLE_INDEX

#define TILE_DESTRUCTIBLE_INDEX

/* Constantes de jeu */
#define DELAI_DEFAUT_BOMBE 2000

/* Autres */
#define NB_JOUEURS 4
#define MAJ_PAR_SEC 60
#define ACTIVER_COLLISIONS 1
#define KEYS_PER_PLAYER 5    /* nombres de touches à assigner par joueur */
#define NB_BOMBES_MAX 100

/* Types de murs */
enum TYPE_MUR {MUR_METAL = -1, HERBE, MUR_INDESTRUCTIBLE, MUR_BRIQUES, MUR_SOLIDE};

/* Actions des touches */
enum TOUCHES {UP, DOWN, LEFT, RIGHT, BOMB};

#endif // CONSTANTES_H

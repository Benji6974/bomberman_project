#ifndef CONSTANTES_H
#define CONSTANTES_H

/* Taille de la carte et de la fenêtre */
#define HUD_HEIGHT    48
#define TILE_WIDTH    32
#define TILE_HEIGHT   32
#define MAP_WIDTH     17
#define MAP_HEIGHT    11
#define RENDER_SCALE  2 /* Taille des pixels  */
#define WINDOW_WIDTH  RENDER_SCALE*TILE_WIDTH*MAP_WIDTH
#define WINDOW_HEIGHT RENDER_SCALE*(TILE_HEIGHT*MAP_HEIGHT + HUD_HEIGHT)

/* Constantes de jeu */
#define MAJ_PAR_SEC        60
#define ACTIVER_COLLISIONS 1
#define KEYS_PER_PLAYER    5    /* nombres de touches à assigner par joueur */

#define HITBOX_PLAYER_W 23
#define HITBOX_PLAYER_H 15
#define HITBOX_ITEM_W TILE_WIDTH/8
#define HITBOX_ITEM_H TILE_HEIGHT/8

#define NB_JOUEURS          4
#define NB_BOMBES_MAX       100
#define NB_OBJETS_MAX       100
#define DUREE_DEFAUT_PARTIE 180  /* secondes */
#define DELAI_DEFAUT_BOMBE  2000 /* secondes */
#define BONUS_MAX_RANGE     5
#define BONUS_MAX_BOMB      5
#define BONUS_MAX_SPEED     5
#define PROBA_MUR_BRIQUES   20
#define PROBA_MUR_SOLIDE    30

#define SCORE_MUR_BRIQUES 10
#define SCORE_MUR_SOLIDE  3*SCORE_MUR_BRIQUES
#define SCORE_JOUEUR_HIT  30
#define SCORE_JOUEUR_KILL 100
#define SCORE_ITEM_GET    20

/* Types de murs */
enum TYPE_MUR {MUR_METAL = -1, HERBE, MUR_INDESTRUCTIBLE, MUR_BRIQUES, MUR_SOLIDE};

/* Objets */
enum {ITEM_SHIELD, ITEM_RANGE, ITEM_BOMB, ITEM_SPEED};
enum {P_SHIELD = 10, P_RANGE = 30, P_BOMB = 30, P_SPEED = 30};
enum {P_HERBE = 10,P_MUR_INDESTRUCTIBLE =  20,P_MUR_BRIQUES = 50,P_MUR_SOLIDE = 20};
/* Actions des touches */
enum TOUCHES {UP, DOWN, LEFT, RIGHT, BOMB};

#endif // CONSTANTES_H

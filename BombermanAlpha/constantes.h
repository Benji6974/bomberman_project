#ifndef CONSTANTES_H
#define CONSTANTES_H

/* Taille de la carte et de la fenêtre */
#define TILE_WIDTH    32
#define TILE_HEIGHT   32
#define MAP_WIDTH     17
#define MAP_HEIGHT    11
#define WINDOW_WIDTH  TILE_WIDTH*MAP_WIDTH
#define WINDOW_HEIGHT TILE_HEIGHT*MAP_HEIGHT
#define HITBOX_PLAYER 23

/* Index sprites */

#define PLAYER_INDEX
#define BOMB_INDEX
#define TILE_INDESTRUCTIBLE_INDEX
#define TILE_DESTRUCTIBLE_INDEX

enum{UP, DOWN, LEFT, RIGHT};

#endif // CONSTANTES_H

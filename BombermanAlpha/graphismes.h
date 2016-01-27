#ifndef GRAPHISMES_H
#define GRAPHISMES_H

#include <SDL.h>

#include "constantes.h"
#include "jeu.h"

#define NB_FEUILLES_SPRITES 3
#define FEUILLE_TILES       "img/tileset.bmp"
#define FEUILLE_PERSO       "img/character_silver.bmp"
#define FEUILLE_OBJETS      "img/atlas5_32.bmp"

#define CLIP_PERSO_X 0
#define CLIP_PERSO_Y 0

#define CLIP_BOMBE_X 2
#define CLIP_BOMBE_Y 0

#define CLIP_HERBE_X       1
#define CLIP_HERBE_Y       0
#define CLIP_MUR_METAL_X   6
#define CLIP_MUR_METAL_Y   12
#define CLIP_MUR_INDEST_X  0
#define CLIP_MUR_INDEST_Y  10
#define CLIP_MUR_BRIQUES_X 0
#define CLIP_MUR_BRIQUES_Y 13
#define CLIP_MUR_SOLIDE_X  0
#define CLIP_MUR_SOLIDE_Y  12

#define SPRITE_PERSO_W 23
#define SPRITE_PERSO_H 38

typedef struct Graphismes{
    SDL_Window *fenetre;
    SDL_Renderer *renderer;
    SDL_Texture *feuilles_sprites[NB_FEUILLES_SPRITES];
}
Graphismes;

Graphismes* init_graphismes(char *titre, int x, int y, int l, int h, Uint32 flags_fenetre, Uint32 flags_renderer);

int maj_graphismes(Game *jeu, Graphismes *g);
int maj_graph_carte(Game *jeu, Graphismes *g);
int maj_graph_entites(Game *jeu, Graphismes *g);

void trier_par_y(Player **tab, int taille);

void free_graphismes(Graphismes *g);

/*
 * Fonction:    charger_sprite
 * Description: charge une image BMP en la mettant dans une texture SDL
 * Paramètres:  pointeur vers un SDL_Renderer sur lequel charger la texture SDL
 * Sortie:      pointeur vers la texture SDL créée
 */
SDL_Texture* charger_sprite(SDL_Renderer *renderer, char *chemin);

#endif // GRAPHISMES_H

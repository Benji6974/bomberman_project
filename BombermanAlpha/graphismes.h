#ifndef GRAPHISMES_H
#define GRAPHISMES_H

#include <SDL.h>
#include <math.h>

#include "constantes.h"
#include "jeu.h"

#define NB_FEUILLES_SPRITES 6
#define FEUILLE_TILES       "img/tileset.bmp"
#define FEUILLE_PERSO       "img/character_silver.bmp"
#define FEUILLE_OBJETS      "img/atlas5_32.bmp"
#define FEUILLE_BONUS       "img/items.bmp"
#define FEUILLE_TEXTE       "img/text.bmp"
#define FEUILLE_EXPLOSIONS  "img/explosion.bmp"
#define FEUILLE_COMMANDES  "img/comandes.bmp"
#define FEUILLE_LOGO  "img/preview-bomberman_logo.bmp"
#define FEUILLE_VOLUME  "img/Low-Volume.bmp"

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

#define CLIP_ITEM_SHIELD_X 0
#define CLIP_ITEM_SHIELD_Y 3
#define CLIP_ITEM_RANGE_X  0
#define CLIP_ITEM_RANGE_Y  1
#define CLIP_ITEM_BOMB_X   0
#define CLIP_ITEM_BOMB_Y   0
#define CLIP_ITEM_SPEED_X  0
#define CLIP_ITEM_SPEED_Y  2

#define SPRITE_PERSO_W 23
#define SPRITE_PERSO_H 38

#define SPRITE_CHAR_W 16
#define SPRITE_CHAR_H 16

#define SPRITE_SON_H 439
#define SPRITE_SON_W 386

#define SCORE_NB_CHIFFRES 4

typedef struct Graphismes
{
    SDL_Window *fenetre;
    SDL_Renderer *renderer;
    SDL_Texture *feuilles_sprites[NB_FEUILLES_SPRITES];
}
Graphismes;

Graphismes* init_graphismes(char *titre, int x, int y, int l, int h, Uint32 flags_fenetre, Uint32 flags_renderer);

int maj_menu(Graphismes *g,int nb_joueurs,int temps,int map_jeu, int volume);
int maj_graphismes(Game *jeu, Graphismes *g);
int maj_graph_carte(Game *jeu, Graphismes *g);
int maj_graph_entites(Game *jeu, Graphismes *g);
int maj_HUD(Game*, Graphismes*);

int afficher(Graphismes *g, int feuille_sprite, SDL_Rect *clip, SDL_Rect *dest);
int afficher_char(Graphismes *g, char c, SDL_Rect pos);
void ecrire_mot(Graphismes *g,char *mot,SDL_Rect blit);

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

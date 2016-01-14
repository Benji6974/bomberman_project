#ifndef GRAPHISMES_H
#define GRAPHISMES_H

#include <SDL.h>

/*
 * Structure Sprite
 * Permet de d�finir un sprite (anim� ou non)
 * Attributs: - index_sprite (entier): coordonn�e du tableau de sprite � la partir de laquelle r�cup�rer les sprites
 *            - no_sprites (entier):   nombres de sprites � r�cup�rer
 *            - frame (entier):        frame � laquelle se trouve l'animation
 *            - fps (entier):           frames par secondes de l'animation
 *            - rect (rectangle SDL):  position et taille du sprite
 */
typedef struct Sprite{
    int index_sprite;
    int no_sprites;
    int frame;
    int fps;
    SDL_Rect rect;
}
Sprite;

SDL_Window* init_fenetre(char *titre, int x, int y, int l, int h, Uint32 flags_fenetre, Uint32 flags_renderer);

/*
 * Fonction:    charger_sprite
 * Description: charge une image BMP en la mettant dans une texture SDL
 * Param�tres:  pointeur vers un SDL_Renderer sur lequel charger la texture SDL
 * Sortie:      pointeur vers la texture SDL cr��e
 */
SDL_Texture* charger_sprite(SDL_Renderer *renderer, char *chemin);

#endif // GRAPHISMES_H

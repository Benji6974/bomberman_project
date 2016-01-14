#ifndef GRAPHISMES_H
#define GRAPHISMES_H

#include <SDL.h>

/*
 * Structure Sprite
 * Permet de définir un sprite (animé ou non)
 * Attributs: - index_sprite (entier): coordonnée du tableau de sprite à la partir de laquelle récupérer les sprites
 *            - no_sprites (entier):   nombres de sprites à récupérer
 *            - frame (entier):        frame à laquelle se trouve l'animation
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
 * Paramètres:  pointeur vers un SDL_Renderer sur lequel charger la texture SDL
 * Sortie:      pointeur vers la texture SDL créée
 */
SDL_Texture* charger_sprite(SDL_Renderer *renderer, char *chemin);

#endif // GRAPHISMES_H

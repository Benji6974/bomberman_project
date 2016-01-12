#ifndef GRAPHISMES_H
#define GRAPHISMES_H

#include <SDL.h>

/*
 * Fonction:    charger_sprite
 * Description: charge une image BMP en la mettant dans une texture SDL
 * Param�tres:  pointeur vers un SDL_Renderer sur lequel charger la texture SDL
 * Sortie:      pointeur vers la texture SDL cr��e
 */
SDL_Texture* charger_sprite(SDL_Renderer *renderer, char *chemin);

#endif // GRAPHISMES_H

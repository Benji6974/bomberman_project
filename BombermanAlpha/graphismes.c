#include "graphismes.h"

SDL_Texture* charger_sprite(SDL_Renderer *renderer, char *chemin)
{
    SDL_Texture *finale = NULL;
    SDL_Surface *image_chargee = SDL_LoadBMP(chemin); // on charge l'image en tant que surface
    if(image_chargee != NULL)
    {
        finale = SDL_CreateTextureFromSurface(renderer, image_chargee); // on créer une texture à partir de la surface
        if(finale == NULL)
            printf("Erreur lors de la creation de %s\n", chemin);
        SDL_FreeSurface(image_chargee); // on détruit la surface
    }
    else
        printf("Erreur lors du chargement de %s\n", chemin);

    return finale; // on renvoie la nouvelle texture contenant l'image
}

#include <stdio.h>
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

SDL_Window* init_fenetre(char *titre, int x, int y, int w, int h, Uint32 flags_fenetre, Uint32 flags_renderer)
{
    SDL_Window* fenetre = NULL;
    SDL_Renderer *renderer = NULL;

    /* Initialisation fenêtre */
    fenetre = SDL_CreateWindow(titre, x, y, w, h, flags_fenetre);
    if(fenetre == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur", "Impossible d'instancier une fenetre.", NULL);
        printf("Erreur: Impossible d'instancier une fenetre.\n");
        return NULL;
    }
    /* Initialisation moteur de rendu */
    renderer = SDL_CreateRenderer(fenetre, -1, flags_renderer);
    if(renderer == NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur", "Impossible d'instancier un moteur de rendu.\nVotre systeme semble ne pas supporter une version recente de OpenGL ou Direct3D.", fenetre);
        printf("Erreur: Impossible d'instancier un moteur de rendu.\n");
        return NULL;
    }

    return fenetre; // pour avoir le renderer, suffit d'utiliser SDL_GetRenderer
}

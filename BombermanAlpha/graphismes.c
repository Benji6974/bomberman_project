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

Graphismes* init_graphismes(char *titre, int x, int y, int w, int h, Uint32 flags_fenetre, Uint32 flags_renderer)
{
    SDL_Window* fenetre = NULL;
    SDL_Renderer *renderer = NULL;
    Graphismes *g = NULL;

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

    g = malloc(sizeof(Graphismes));

    g->fenetre = fenetre;
    g->renderer = renderer;

    g->feuilles_sprites[0] = charger_sprite(renderer, FEUILLE_TILES);
    g->feuilles_sprites[1] = charger_sprite(renderer, FEUILLE_PERSO);
    g->feuilles_sprites[2] = charger_sprite(renderer, FEUILLE_OBJETS);
    g->feuilles_sprites[3] = charger_sprite(renderer, FEUILLE_BONUS);


    return g; // pour avoir le renderer, suffit d'utiliser SDL_GetRenderer
}

int maj_graphismes(Game *jeu, Graphismes *g)
{
    int erreur = 0;

    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);

    if(maj_graph_carte(jeu, g) == -1 || maj_graph_entites(jeu,g) == -1)
        erreur = 1;

    SDL_RenderPresent(g->renderer);

    return erreur;
}

int maj_graph_carte(Game *jeu, Graphismes *g)
{
    int i, j, erreur = 0;
    SDL_Rect clip, pos;

    clip.x = 0;
    clip.y = 0;
    clip.w = TILE_WIDTH;
    clip.h = TILE_HEIGHT;

    pos = clip;

    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            pos.x = j*TILE_WIDTH;
            pos.y = i*TILE_HEIGHT;

            if(jeu->carte[i][j] == NULL)
                continue;

            switch(jeu->carte[i][j]->type)
            {
            case MUR_METAL:
                clip.x = CLIP_MUR_METAL_X;
                clip.y = CLIP_MUR_METAL_Y;
                break;
            default:
            case HERBE:
                clip.x = CLIP_HERBE_X;
                clip.y = CLIP_HERBE_Y;
                break;
            case MUR_INDESTRUCTIBLE:
                clip.x = CLIP_MUR_INDEST_X;
                clip.y = CLIP_MUR_INDEST_Y;
                break;
            case MUR_BRIQUES:
                clip.x = CLIP_MUR_BRIQUES_X;
                clip.y = CLIP_MUR_BRIQUES_Y;
                /* Sprite alternatif si la case juste en dessous n'est pas vide */
                if(i+1 < MAP_HEIGHT && jeu->carte[i+1][j] != NULL && jeu->carte[i+1][j]->type == 0)
                    clip.x++;
                break;
            case MUR_SOLIDE:
                clip.x = 2*((4-jeu->carte[i][j]->etat)-1) + CLIP_MUR_SOLIDE_X;
                clip.y = CLIP_MUR_SOLIDE_Y;
                if(i+1 < MAP_HEIGHT && jeu->carte[i+1][j] != NULL && jeu->carte[i+1][j]->type == 0)
                    clip.x++;
                break;
            }

            clip.x *= TILE_WIDTH;
            clip.y *= TILE_HEIGHT;

            if(afficher(g, 0, &clip, &pos) != 0)
                erreur = 1;
        }
    }
    return erreur;
}

int maj_graph_entites(Game *jeu, Graphismes *g)
{
    int i, erreur = 0;
    SDL_Rect clip, pos;
    Player **blit_order = malloc(jeu->nb_joueurs*sizeof(int));

    /* ----- AFFICHAGE DES OBJETS ------ */

    for(i = 0; i < jeu->nb_objets; i++)
    {
        if(jeu->objets[i] == NULL)
            continue;

        pos.x = jeu->objets[i]->pos.x - (TILE_WIDTH - jeu->objets[i]->pos.w)/2;
        pos.y = jeu->objets[i]->pos.y - (TILE_HEIGHT - jeu->objets[i]->pos.h)/2 - 4;

        switch(jeu->objets[i]->type)
        {
        case ITEM_SHIELD:
            clip.x = CLIP_ITEM_SHIELD_X;
            clip.y = CLIP_ITEM_SHIELD_Y;
            break;
        case ITEM_RANGE:
            clip.x = CLIP_ITEM_RANGE_X;
            clip.y = CLIP_ITEM_RANGE_Y;
            break;
        case ITEM_BOMB:
            clip.x = CLIP_ITEM_BOMB_X;
            clip.y = CLIP_ITEM_BOMB_Y;
            break;
        case ITEM_SPEED:
            clip.x = CLIP_ITEM_SPEED_X;
            clip.y = CLIP_ITEM_SPEED_Y;
            break;
        }

        clip.x *= TILE_WIDTH;
        clip.y *= TILE_HEIGHT;

        if(afficher(g, 3, &clip, &pos) != 0)
            erreur = 1;
    }

    /* ----- AFFICHAGE DES BOMBES ----- */

    clip.x = 2*TILE_WIDTH;
    clip.y = 0*TILE_HEIGHT;
    clip.w = TILE_WIDTH;
    clip.h = TILE_HEIGHT;

    pos = clip;

    for(i = 0; i < jeu->nb_bombs; i++)
    {
        pos.x = jeu->bombs[i]->pos.x*TILE_WIDTH;
        pos.y = jeu->bombs[i]->pos.y*TILE_HEIGHT;

        if(afficher(g, 2, &clip, &pos) != 0)
            erreur = 1;
    }

    /* ----- AFFICHAGE DES JOUEURS ----- */

    memcpy(blit_order, jeu->players, jeu->nb_joueurs*sizeof(int));
    trier_par_y(blit_order, jeu->nb_joueurs);

    clip.x = CLIP_PERSO_X;
    clip.y = CLIP_PERSO_Y;
    clip.w = SPRITE_PERSO_W;
    clip.h = SPRITE_PERSO_H;

    pos = clip;

    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        switch(blit_order[i]->direction)
        {
        case DOWN:
            clip.y = 0;
            break;
        case RIGHT:
            clip.y = 1;
            break;
        case UP:
            clip.y = 2;
            break;
        case LEFT:
            clip.y = 3;
            break;
        }

        clip.y *= SPRITE_PERSO_H;

        pos.x = blit_order[i]->pos.x;
        pos.y = blit_order[i]->pos.y - (SPRITE_PERSO_H - HITBOX_PLAYER_H);

        if(!blit_order[i]->est_mort)
        {
            if(afficher(g, 1, &clip, &pos) != 0)
                erreur = 1;
        }
    }

    free(blit_order);
    return erreur;
}

int afficher(Graphismes *g, int feuille_sprite, SDL_Rect *clip, SDL_Rect *dest)
{
    SDL_Rect blit;

    blit.x = RENDER_SCALE*dest->x;
    blit.y = RENDER_SCALE*(dest->y + HUD_HEIGHT);
    blit.w = RENDER_SCALE*dest->w;
    blit.h = RENDER_SCALE*dest->h;

    return SDL_RenderCopy(g->renderer, g->feuilles_sprites[feuille_sprite], clip, &blit);
}

/* Fonction utilisée pour trier le tableau des joueurs en fonction de leur position verticale
 * Cela permet de changer leur ordre d'affichage et ainsi quel joueur est affiché au dessus d'un autre
 */
void trier_par_y(Player **tab, int taille)
{
    int i, j;
    Player *tmp;
    for(i = 1; i < taille; i++)
    {
        j = i;
        while(j > 0 && tab[j-1]->pos.y > tab[j]->pos.y)
        {
            tmp = tab[j];
            tab[j] = tab[j-1];
            tab[j-1] = tmp;
            j--;
        }
    }
}

void free_graphismes(Graphismes *g)
{
    int i;

    SDL_DestroyWindow(g->fenetre);
    SDL_DestroyRenderer(g->renderer);

    for(i = 0; i < NB_FEUILLES_SPRITES; i++)
        SDL_DestroyTexture(g->feuilles_sprites[i]);
}

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
    g->feuilles_sprites[4] = charger_sprite(renderer, FEUILLE_TEXTE);
    g->feuilles_sprites[5] = charger_sprite(renderer, FEUILLE_EXPLOSIONS);
    g->feuilles_sprites[6] = charger_sprite(renderer, FEUILLE_COMMANDES);


    return g; // pour avoir le renderer, suffit d'utiliser SDL_GetRenderer
}

int maj_menu(Graphismes *g)
{
    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);
    int erreur;
    /* Mettre ici les fonction de mise à jour du menu */
    SDL_Rect blit;
        blit.y = (HUD_HEIGHT - SPRITE_CHAR_H)/2;
        blit.x = (MAP_WIDTH*TILE_WIDTH - 8*SPRITE_CHAR_W*2)/2;
        blit.h = SPRITE_CHAR_H*2;
        blit.w = SPRITE_CHAR_W*2;
        char txt_pause[50] = {"BOMBERMAN"};
        ecrire_mot(g,txt_pause,blit);

        blit.h = SPRITE_CHAR_H;
        blit.w = SPRITE_CHAR_W;
        blit.y = (MAP_HEIGHT*TILE_HEIGHT - TILE_HEIGHT + HUD_HEIGHT);
        blit.x = (MAP_WIDTH*TILE_WIDTH - 10*SPRITE_CHAR_W);
        char jouer[50] = {"Jouer =>"};
        ecrire_mot(g,jouer,blit);

        blit.y = (2*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 0;
        char type_map[50] = {"Type de map : "};
        ecrire_mot(g,type_map,blit);

        blit.y = (2*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 14*SPRITE_CHAR_W;
        char type_map2[50] = {"< Aleatoire >"};
        ecrire_mot(g,type_map2,blit);

        blit.y = (4*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 0;
        char nombrejoueurs[50] = {"Nombre de Joueurs : "};
        ecrire_mot(g,nombrejoueurs,blit);

        blit.y = (4*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 20*SPRITE_CHAR_W;
        char nombrejoueurs_int[50];
        sprintf(nombrejoueurs_int, "- %d +", NB_JOUEURS);
        ecrire_mot(g,nombrejoueurs_int,blit);

        blit.y = (8*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 0;
        char controls[50] = {"Controles : "};
        ecrire_mot(g,controls,blit);

        blit.y = (6*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 0;
        char temps_partie[50] = {"Temps de jeu : "};
        ecrire_mot(g,temps_partie,blit);

        blit.y = (6*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 15*SPRITE_CHAR_W;
        char char_heure[50];
        copie_heure(char_heure,DUREE_DEFAUT_PARTIE);
        ecrire_mot(g,char_heure,blit);


        blit.y = (8*TILE_HEIGHT + HUD_HEIGHT);
        blit.x = 0;
        blit.h = 4*SPRITE_CHAR_H;
        blit.w = 20*SPRITE_CHAR_W;
         if(afficher(g, 6, NULL, &blit) != 0)
                erreur = 1;
        /*1632*414*/


    SDL_RenderPresent(g->renderer);

    return erreur;
}

void copie_heure(char* char_heure,int heure)
{
    sprintf(char_heure, "- %02d:%02d +", heure/60,heure%60);
}

int maj_graphismes(Game *jeu, Graphismes *g)
{
    int erreur = 0;

    SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
    SDL_RenderClear(g->renderer);

    if(maj_graph_carte(jeu, g) == -1 || maj_graph_entites(jeu,g) == -1 || maj_HUD(jeu, g) == -1)
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

    /* --- AFFICHAGE DES EXPLOSIONS --- */

    clip.x = 0;
    clip.y = 3*TILE_HEIGHT;
    clip.w = TILE_WIDTH;
    clip.h = TILE_HEIGHT;

    pos = clip;

    for(i = 0; i < jeu->nb_explosions; i++)
    {
        if(jeu->explosions[i] == NULL)
            continue;

        clip.x = jeu->explosions[i]->aspect*TILE_WIDTH;
        clip.y = floor(4*jeu->explosions[i]->temps_restant/(float)jeu->explosions[i]->duree_de_vie)*TILE_HEIGHT; /* Anime l'explosion en fonction de son temps restant */

        pos.x = jeu->explosions[i]->pos.x;
        pos.y = jeu->explosions[i]->pos.y;
        afficher(g, 5, &clip, &pos);
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

int maj_HUD(Game *jeu, Graphismes *g)
{
    SDL_Rect blit;
    int i, j;
    int temps = jeu->time/1000;
    int minutes  = temps/60;
    int secondes = temps%60;

    blit.w = SPRITE_CHAR_W;
    blit.h = SPRITE_CHAR_H;

    /* Horloge */
    blit.x = (WINDOW_WIDTH/RENDER_SCALE - 5*SPRITE_CHAR_W)/2;
    blit.y = (HUD_HEIGHT - SPRITE_CHAR_W)/2;
    afficher_char(g, minutes/10 + 48, blit);
    blit.x += SPRITE_CHAR_W;
    afficher_char(g, minutes%10 + 48, blit);
    blit.x += SPRITE_CHAR_W;
    afficher_char(g, ':', blit);
    blit.x += SPRITE_CHAR_W;
    afficher_char(g, secondes/10 + 48, blit);
    blit.x += SPRITE_CHAR_W;
    afficher_char(g, secondes%10 + 48, blit);

    /* Scores */
    blit.y = (HUD_HEIGHT - SPRITE_CHAR_W)/2;
    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        if(i < 2)
            blit.x = HUD_HEIGHT/2 + (SCORE_NB_CHIFFRES-1)*SPRITE_CHAR_W*(1+2*i);
        else
            blit.x = SPRITE_CHAR_W/2+MAP_WIDTH*TILE_WIDTH - HUD_HEIGHT/2 - (1+2*(jeu->nb_joueurs-1-i)*(SCORE_NB_CHIFFRES-1)*SPRITE_CHAR_W) - SPRITE_CHAR_W*(SCORE_NB_CHIFFRES-1)/2.;

        for(j = 0; j < SCORE_NB_CHIFFRES; j++)
        {
            afficher_char(g, (jeu->players[i]->score/(int)pow(10, j))%10 + 48, blit);
            blit.x -= SPRITE_CHAR_W;
        }
    }
    /* nom des perso test */

    int z;
    blit.x = 0;
    SDL_Rect clip;

    int erreur;
    char nb_bomb[10];
    char nb_vie[10];
    char nb_vitesse[10];
    char nb_puissance[10];
    char mort[10] = {"mort"};
    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        /* ecrire le nom du joueur*/
        blit.y = 0;
        blit.w = SPRITE_CHAR_W/2;
        blit.h = SPRITE_CHAR_H/2;
        if(i == 0)
            blit.x = SPRITE_CHAR_W;
        else if(i == 1)
            blit.x = SPRITE_CHAR_W*7;
        else if(i == 2)
            blit.x = SPRITE_CHAR_W*23;
        else
            blit.x = SPRITE_CHAR_W*29;
        ecrire_mot(g,jeu->players[i]->nom,blit);

        if (!jeu->players[i]->est_mort)
        {


            /* afficher les nombre d'options*/
            blit.y = HUD_HEIGHT-SPRITE_CHAR_H/2;
            blit.w = SPRITE_CHAR_W/2;
            blit.h = SPRITE_CHAR_H/2;
            blit.x -= SPRITE_CHAR_W*0.5;
            sprintf(nb_bomb, "%d", jeu->players[i]->nb_bomb_max);
            sprintf(nb_vie, "%d", jeu->players[i]->bouclier);
            sprintf(nb_vitesse, "%d", jeu->players[i]->vitesse);
            sprintf(nb_puissance, "%d", jeu->players[i]->typebomb.puissance);
            ecrire_mot(g,nb_bomb,blit);
            blit.x += SPRITE_CHAR_W*1.5;
            ecrire_mot(g,nb_puissance,blit);
            blit.x += SPRITE_CHAR_W*1.5;
            ecrire_mot(g,nb_vitesse,blit);
            blit.x += SPRITE_CHAR_W*1.5;
            ecrire_mot(g,nb_vie,blit);

            /* affiche les images items*/
            if(i == 0)
                blit.x = 0;
            else if(i == 1)
                blit.x = SPRITE_CHAR_W*6;
            else if(i == 2)
                blit.x = SPRITE_CHAR_W*22;
            else
                blit.x = SPRITE_CHAR_W*28;
            clip.w = TILE_WIDTH;
            clip.h = TILE_HEIGHT;
            blit.y = TILE_WIDTH/1.35-HUD_HEIGHT;
            blit.w = SPRITE_CHAR_W/2;
            blit.h = SPRITE_CHAR_H/2;

            clip.x = CLIP_ITEM_BOMB_X*TILE_WIDTH;
            clip.y = CLIP_ITEM_BOMB_Y*TILE_HEIGHT;
            if(afficher(g, 3, &clip, &blit) != 0)
                erreur = 1;
            blit.x += SPRITE_CHAR_W*1.5;
            clip.x = CLIP_ITEM_RANGE_X*TILE_WIDTH;
            clip.y = CLIP_ITEM_RANGE_Y*TILE_HEIGHT;
            if(afficher(g, 3, &clip, &blit) != 0)
                erreur = 1;
            blit.x += SPRITE_CHAR_W*1.5;
            clip.x = CLIP_ITEM_SPEED_X*TILE_WIDTH;
            clip.y = CLIP_ITEM_SPEED_Y*TILE_HEIGHT;
            if(afficher(g, 3, &clip, &blit) != 0)
                erreur = 1;
            blit.x += SPRITE_CHAR_W*1.5;
            clip.x = CLIP_ITEM_SHIELD_X*TILE_WIDTH;
            clip.y = CLIP_ITEM_SHIELD_Y*TILE_HEIGHT;
            if(afficher(g, 3, &clip, &blit) != 0)
                erreur = 1;

        }else
        {
            blit.y = HUD_HEIGHT-SPRITE_CHAR_H/2;
            ecrire_mot(g,mort,blit);

        }
    }

    if (jeu->en_pause)
    {
        blit.y = (HUD_HEIGHT - SPRITE_CHAR_H)/2;
        blit.x = (MAP_WIDTH*TILE_WIDTH - 8*SPRITE_CHAR_W)/2;
        blit.h = SPRITE_CHAR_H;
        blit.w = SPRITE_CHAR_W;
        char txt_pause[50] = {"EN PAUSE"};
        ecrire_mot(g,txt_pause,blit);
    }



    return 0;
}

void ecrire_mot(Graphismes *g,char *mot,SDL_Rect blit)
{
    int z;
    for(z = 0; mot[z] != 0 ; z++)
    {
        afficher_char(g, mot[z], blit);
        blit.x += blit.w;
    }
}

int afficher_char(Graphismes *g, char c, SDL_Rect pos)
{
    SDL_Rect clip;

    clip.w = SPRITE_CHAR_W;
    clip.h = SPRITE_CHAR_H;

    switch(c)
    {
    case '0':
        clip.x = 0;
        clip.y = 1;
        break;
    case '1':
        clip.x = 1;
        clip.y = 1;
        break;
    case '2':
        clip.x = 2;
        clip.y = 1;
        break;
    case '3':
        clip.x = 3;
        clip.y = 1;
        break;
    case '4':
        clip.x = 4;
        clip.y = 1;
        break;
    case '5':
        clip.x = 5;
        clip.y = 1;
        break;
    case '6':
        clip.x = 6;
        clip.y = 1;
        break;
    case '7':
        clip.x = 7;
        clip.y = 1;
        break;
    case '8':
        clip.x = 8;
        clip.y = 1;
        break;
    case '9':
        clip.x = 9;
        clip.y = 1;
        break;
    case 'A':
        clip.x = 1;
        clip.y = 2;
        break;
    case 'B':
        clip.x = 2;
        clip.y = 2;
        break;
    case 'C':
        clip.x = 3;
        clip.y = 2;
        break;
    case 'D':
        clip.x = 4;
        clip.y = 2;
        break;
    case 'E':
        clip.x = 5;
        clip.y = 2;
        break;
    case 'F':
        clip.x = 6;
        clip.y = 2;
        break;
    case 'G':
        clip.x = 7;
        clip.y = 2;
        break;
    case 'H':
        clip.x = 8;
        clip.y = 2;
        break;
    case 'I':
        clip.x = 9;
        clip.y = 2;
        break;
    case 'J':
        clip.x = 10;
        clip.y = 2;
        break;
    case 'K':
        clip.x = 11;
        clip.y = 2;
        break;
    case 'L':
        clip.x = 12;
        clip.y = 2;
        break;
    case 'M':
        clip.x = 13;
        clip.y = 2;
        break;
    case 'N':
        clip.x = 14;
        clip.y = 2;
        break;
    case 'O':
        clip.x = 15;
        clip.y = 2;
        break;
    case 'P':
        clip.x = 0;
        clip.y = 3;
        break;
    case 'Q':
        clip.x = 1;
        clip.y = 3;
        break;
    case 'R':
        clip.x = 2;
        clip.y = 3;
        break;
    case 'S':
        clip.x = 3;
        clip.y = 3;
        break;
    case 'T':
        clip.x = 4;
        clip.y = 3;
        break;
    case 'U':
        clip.x = 5;
        clip.y = 3;
        break;
    case 'V':
        clip.x = 6;
        clip.y = 3;
        break;
    case 'W':
        clip.x = 7;
        clip.y = 3;
        break;
    case 'X':
        clip.x = 8;
        clip.y = 3;
        break;
    case 'Y':
        clip.x = 9;
        clip.y = 3;
        break;
    case 'Z':
        clip.x = 10;
        clip.y = 3;
        break;
    case 'a':
        clip.x = 1;
        clip.y = 4;
        break;
    case 'b':
        clip.x = 2;
        clip.y = 4;
        break;
    case 'c':
        clip.x = 3;
        clip.y = 4;
        break;
    case 'd':
        clip.x = 4;
        clip.y = 4;
        break;
    case 'e':
        clip.x = 5;
        clip.y = 4;
        break;
    case 'f':
        clip.x = 6;
        clip.y = 4;
        break;
    case 'g':
        clip.x = 7;
        clip.y = 4;
        break;
    case 'h':
        clip.x = 8;
        clip.y = 4;
        break;
    case 'i':
        clip.x = 9;
        clip.y = 4;
        break;
    case 'j':
        clip.x = 10;
        clip.y = 4;
        break;
    case 'k':
        clip.x = 11;
        clip.y = 4;
        break;
    case 'l':
        clip.x = 12;
        clip.y = 4;
        break;
    case 'm':
        clip.x = 13;
        clip.y = 4;
        break;
    case 'n':
        clip.x = 14;
        clip.y = 4;
        break;
    case 'o':
        clip.x = 15;
        clip.y = 4;
        break;
    case 'p':
        clip.x = 0;
        clip.y = 5;
        break;
    case 'q':
        clip.x = 1;
        clip.y = 5;
        break;
    case 'r':
        clip.x = 2;
        clip.y = 5;
        break;
    case 's':
        clip.x = 3;
        clip.y = 5;
        break;
    case 't':
        clip.x = 4;
        clip.y = 5;
        break;
    case 'u':
        clip.x = 5;
        clip.y = 5;
        break;
    case 'v':
        clip.x = 6;
        clip.y = 5;
        break;
    case 'w':
        clip.x = 7;
        clip.y = 5;
        break;
    case 'x':
        clip.x = 8;
        clip.y = 5;
        break;
    case 'y':
        clip.x = 9;
        clip.y = 5;
        break;
    case 'z':
        clip.x = 10;
        clip.y = 5;
        break;
    case ':':
        clip.x = 10;
        clip.y = 1;
        break;
    case '!':
        clip.x = 1;
        clip.y = 0;
        break;
    case '-':
        clip.x = 6;
        clip.y = 7;
        break;
    case '_':
        clip.x = 15;
        clip.y = 3;
        break;
    case '?':
        clip.x = 15;
        clip.y = 1;
        break;
    case 'à':
        clip.x = 0;
        clip.y = 12;
        break;
    case 'é':
        clip.x = 9;
        clip.y = 12;
        break;
    case 'è':
        clip.x = 8;
        clip.y = 12;
        break;
    case 'ç':
        clip.x = 7;
        clip.y = 12;
        break;
        case '=':
        clip.x = 13;
        clip.y = 1;
        break;
        case '>':
        clip.x = 14;
        clip.y = 1;
        break;
        case '<':
        clip.x = 12;
        clip.y = 1;
        break;
         case '+':
        clip.x = 11;
        clip.y = 0;
        break;


    default:
        clip.x = 0;
        clip.y = 0;
    }

    clip.x *= SPRITE_CHAR_W;
    clip.y *= SPRITE_CHAR_H;

    pos.x *= RENDER_SCALE;
    pos.y *= RENDER_SCALE;
    pos.w *= RENDER_SCALE;
    pos.h *= RENDER_SCALE;
    return SDL_RenderCopy(g->renderer, g->feuilles_sprites[4], &clip, &pos);
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

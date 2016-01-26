#include "constantes.h"
#include "jeu.h"

int gKeys[KEYS_PER_PLAYER*NB_JOUEURS] = {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RCTRL,      /* Joueur 1 */
                                         SDLK_z, SDLK_s, SDLK_q, SDLK_d, SDLK_LCTRL,                 /* Joueur 2 */
                                         SDLK_y, SDLK_h, SDLK_g, SDLK_j, SDLK_SPACE,                 /* Joueur 3 */
                                         SDLK_KP_8, SDLK_KP_5, SDLK_KP_4, SDLK_KP_6, SDLK_KP_ENTER}; /* Joueur 4 */


Game* init_jeu(int type, int nb_joueurs, int temps)
{
    Game *jeu;
    Tile ***carte;
    int i, j;

    /* - Tableau pour tester la carte - */


    int carte_data[TILE_HEIGHT][TILE_WIDTH] = {
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, 0, 0, 2, 0, 2, 0, 2, 2, 0, 0, 2, 0, 2, 0, 0, -1},
        {-1, 0, 1, 3, 1, 0, 1, 2, 1, 0, 1, 2, 1, 0, 1, 0, -1},
        {-1, 3, 2, 0, 2, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 3, -1},
        {-1, 2, 1, 2, 1, 2, 1, 0, 1, 0, 1, 2, 1, 0, 1, 0, -1},
        {-1, 0, 2, 0, 0, 2, 2, 2, 3, 2, 2, 2, 2, 2, 0, 0, -1},
        {-1, 0, 1, 0, 1, 2, 1, 2, 1, 0, 1, 2, 1, 2, 1, 0, -1},
        {-1, 3, 0, 0, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 3, -1},
        {-1, 0, 1, 2, 1, 0, 1, 2, 1, 2, 1, 2, 1, 2, 1, 0, -1},
        {-1, 0, 0, 2, 0, 0, 2, 2, 0, 2, 2, 0, 0, 2, 0, 0, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
    };

    /* -------------------------------- */

    jeu = malloc(sizeof(Game));

    /* Tableau des bombes en jeu */
    jeu->bombs = (Bomb**)malloc(NB_BOMBES_MAX*sizeof(Bomb*));
    memset(jeu->bombs, 0, NB_BOMBES_MAX*sizeof(Bomb*));
    jeu->nb_bombs = 0;

    jeu->type = type;
    jeu->time = temps;
    jeu->nb_joueurs = nb_joueurs;

    /* Génération de la carte */
    carte = (Tile***)malloc(MAP_HEIGHT*sizeof(Tile**)); /* tableau 2D de pointeurs sur Tiles */
    for(i = 0; i < MAP_HEIGHT; i++)
    {
        carte[i] = malloc(MAP_WIDTH*sizeof(Tile*));
        for(j = 0; j < MAP_WIDTH; j++)
        {
            carte[i][j] = malloc(sizeof(Tile));
            carte[i][j]->type = carte_data[i][j];
            if(carte[i][j]->type == MUR_SOLIDE)
                carte[i][j]->etat = 3;
        }
    }

    jeu->carte = carte;

    /* Initialisation controles */
    jeu->touches.num_keys = KEYS_PER_PLAYER*jeu->nb_joueurs;
    jeu->touches.key_map = malloc(jeu->touches.num_keys*sizeof(int));
    jeu->touches.keys_pressed = calloc(jeu->touches.num_keys, sizeof(int));

    for(i = 0; i < jeu->touches.num_keys; i++)
    {
        jeu->touches.key_map[i] = gKeys[i];
    }

    /* Generation du/des players*/

    jeu->players = (Player**)malloc(nb_joueurs*sizeof(Player*));
    for(i = 0; i < nb_joueurs; i++)
    {
        jeu->players[i] = init_player("Joueur", i);
        jeu->players[i]->keymap_offset = i*KEYS_PER_PLAYER;
        if (i==1)
        {
            jeu->players[i]->pos.x = TILE_WIDTH*(MAP_WIDTH-2);
        }
        if(i==2)
        {
            jeu->players[i]->pos.y = TILE_HEIGHT*9;
        }
        if(i==3)
        {
            jeu->players[i]->pos.y = TILE_HEIGHT*9;
            jeu->players[i]->pos.x = TILE_WIDTH*(MAP_WIDTH-2);
        }
    }

    return jeu;
}

int update_jeu(Game *jeu, int dt)
{
    int i, joueur = 0, en_vie = 0;

    maj_bombs(jeu, dt);

    /* Détermination du gagnant */
    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        maj_joueur(jeu, i);
        if(!jeu->players[i]->est_mort)
        {
            en_vie++;
            joueur = i+1;
        }
    }
    if(en_vie == 1)
    {
        char nom_joueur[256];
        sprintf(nom_joueur, "Joueur %d gagne!", joueur);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fin de partie", nom_joueur, NULL);
        return 1;
    }
    else if(en_vie == 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fin de partie", "Tout le monde est mort!", NULL);
        return 1;
    }
    return 0;
}

/* Fonction générale de collision entre une tile quelconque et un rectangle */
int collision_tile_rect(int x, int y, SDL_Rect rect)
{
    int i, j, x1=0, x2=0, y1=0, y2=0;

    x1 = rect.x/TILE_WIDTH;
    y1 = rect.y/TILE_HEIGHT;

    x2 = (rect.x + rect.w - 1)/TILE_WIDTH;
    y2 = (rect.y + rect.h - 1)/TILE_HEIGHT;

    for(i = x1; i <= x2; i++)
    {
        for(j = y1; j <= y2; j++)
        {
            if(x == i && y == j)
                return 1;
        }
    }

    return 0;
}

/* Verification collisions rectangle-rectangle avec le décor
 * Renvoie 1 si la collision est totale, 2 si elle est partielle
 */
int collision_joueur_decor(Game *jeu, int joueur)
{
    if(!ACTIVER_COLLISIONS)
        return 0;

    int x=0, y=0, x1=0, x2=0, y1=0, y2=0;
    const SDL_Rect pos = jeu->players[joueur]->pos;

    x1 = pos.x/TILE_WIDTH;
    y1 = pos.y/TILE_HEIGHT;

    x2 = (pos.x + pos.w - 1)/TILE_WIDTH;
    y2 = (pos.y + pos.h - 1)/TILE_HEIGHT;

    for(x = x1; x <= x2; x++)
    {
        for(y = y1; y <= y2; y++)
        {
            /* test collision avec les blocs de la carte */
            if(jeu->carte[y][x]->type != 0)
            {
                return 1;

            }
        }
    }

    return 0;
}

int collision_joueur_objets(Game *jeu, int joueur, int last_col)
{
    if(!ACTIVER_COLLISIONS)
        return 0;

    int i=0;

    /* test collision avec les bombes */
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        if(collision_tile_rect(jeu->bombs[i]->pos.x, jeu->bombs[i]->pos.y, jeu->players[joueur]->pos)
        &&(jeu->bombs[i]->id_proprietaire+1 != last_col || last_col == 0))
            return jeu->bombs[i]->id_proprietaire+1; /* permet d'eviter que les joueurs utilisent leurs propres bombes pour traverser une bombe adverse */
    }

    return 0;
}

int poser_bomb(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    Bomb *b = NULL;
    int i, x, y;
    if (p->nb_bomb_jeu < p->nb_bomb_max && jeu->nb_bombs < NB_BOMBES_MAX)
    {
        x = p->pos.x/TILE_WIDTH;
        y = p->pos.y/TILE_HEIGHT;
        for(i = 0; jeu->bombs[i] != NULL; i++)
        {
            if(jeu->bombs[i]->pos.x == x
            && jeu->bombs[i]->pos.y == y)
            {
                return -1;
            }
        }

        b = malloc(sizeof(Bomb));
        *b = p->typebomb; /* copie de la bombe interne du joueur */

        b->pos.x = x;
        b->pos.y = y;

        jeu->bombs[i] = b;
        jeu->nb_bombs++;
        p->nb_bomb_jeu++;
    }
    else
        return -1;
    return 0;
}

/* A faire: prendre en compte les murs a plusieurs états */
int degats_case(Game *jeu, int x, int y)
{
    int i, detruit_mur = 1;
    Tile *tile = NULL;
    Player *p = NULL;

    /* si la case est hors de la map, on arrête tout et on renvoie 1 */
    if(x < 0 || y < 0 || x > MAP_WIDTH || y > MAP_HEIGHT)
        return 1;

    tile = jeu->carte[y][x];

    /* dommages aux murs */
    switch(tile->type)
    {
    case HERBE:
        detruit_mur = 0;
        break;
    case MUR_BRIQUES:
        tile->type = 0;
        break;
    case MUR_SOLIDE:
        tile->etat--;
        if(tile->etat <= 0)
           tile->type = 0;
        break;
    }

    /* dommages aux joueurs */
    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        p = jeu->players[i];
        if(collision_tile_rect(x, y, p->pos) && !p->est_mort)
        {
            if(p->bouclier)
            {
                p->bouclier = 0;
            }
            else
            {
                p->vie--;
                if(p->vie <= 0)
                    p->est_mort = 1;
            }
        }
    }

    /* Renvoie 1 si un mur a été détruit pour que si c'est une bombe qui a détruit le mur, on arrete de verifier les cases plus loin */
    return detruit_mur;
}

int exploser_bombe(Game *jeu, int bombe)
{
    int i, x, y;
    Player *p = jeu->players[jeu->bombs[bombe]->id_proprietaire];
    Bomb *b = jeu->bombs[bombe];

    if(jeu->nb_bombs > 0 && b != NULL)
    {
        x = b->pos.x;
        y = b->pos.y;

        /* destruction du décor et des joueurs */

        /* à la position de la bombe */
        degats_case(jeu, x, y);

        /* vers le haut */
        for(i = 1; i <= b->puissance && !degats_case(jeu, x, y-i); i++);

        /* vers le bas */
        for(i = 1; i <= b->puissance && !degats_case(jeu, x, y+i); i++);

        /* vers la gauche */
        for(i = 1; i <= b->puissance && !degats_case(jeu, x-i, y); i++);

        /* vers la droite */
        for(i = 1; i <= b->puissance && !degats_case(jeu, x+i, y); i++);


        /* destruction de la bombe */
        free(b);
        jeu->bombs[bombe] = NULL;

        /* reorganisation du tableau des bombes */
        for(i = bombe; i < jeu->nb_bombs; i++)
        {
            if(jeu->bombs[i+1] != NULL)
            {
                jeu->bombs[i] = jeu->bombs[i+1];
                jeu->bombs[i+1] = NULL;
            }
        }
    }
    else
        return -1;

    /* décrémentation nombres de bombes du joueur et jeu */
    if(jeu->nb_bombs > 0)
        jeu->nb_bombs--;
    if(p->nb_bomb_jeu > 0)
        p->nb_bomb_jeu--;
    return 0;
}

void maj_bombs(Game *jeu, int dt)
{
    int i;
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        if(jeu->bombs[i]->delai > 0)
        {
            /* gestion du delai avant explosion des bombes */
            jeu->bombs[i]->delai -= dt;
            if(jeu->bombs[i]->delai <= 0)
                exploser_bombe(jeu, i);
        }
    }
}

void maj_joueur(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    int k = p->keymap_offset, collision_decor = 0, collision_objets = 0;
    int move_x = 0, move_y = 0;

    if(p->est_mort)
        return;

    /* collision avant deplacement */
    collision_decor = collision_joueur_decor(jeu, joueur);
    collision_objets = collision_joueur_objets(jeu, joueur, 0);

    /* On déplace le joueur en fonction des touches appuyées */
    if(jeu->touches.keys_pressed[k+UP])
    {
        move_y += -1;
        p->direction = UP;
    }
    if(jeu->touches.keys_pressed[k+DOWN])
    {
        move_y += 1;
        p->direction = DOWN;
    }
    if(jeu->touches.keys_pressed[k+LEFT])
    {
        move_x += -1;
        p->direction = LEFT;
    }
    if(jeu->touches.keys_pressed[k+RIGHT])
    {
        move_x += 1;
        p->direction = RIGHT;
    }

    p->pos.x += move_x*p->vitesse;
    while(move_x
      && ((collision_joueur_decor(jeu, joueur) && !collision_decor)
      || (collision_joueur_objets(jeu, joueur, collision_objets) ) ) )
        p->pos.x -= move_x;

    p->pos.y += move_y*p->vitesse;
    while(move_y
      && ((collision_joueur_decor(jeu, joueur) && !collision_decor)
      || (collision_joueur_objets(jeu, joueur, collision_objets) ) ) )
        p->pos.y -= move_y;

    /* Si le joueur appuie sur la touche pour poser une bombe */
    if(jeu->touches.keys_pressed[k+BOMB])
        poser_bomb(jeu, joueur);
}

void detruire_jeu(Game* jeu)
{
    int i, j;

    /* Vidage de la carte */
    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            free(jeu->carte[i][j]);
        }
        free(jeu->carte[i]);
    }
    free(jeu->carte);

    /* Libération du tableau des joueurs */
    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        free(jeu->players[i]);
    }

    free(jeu->touches.key_map);
    free(jeu->touches.keys_pressed);

    /* Libération du tableau des bombes (au cas où) */
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        free(jeu->bombs[i]);
    }

    /* Destruction du jeu */
    free(jeu);
}

Bomb* init_bomb(int type, int id_proprietaire)
{
    Bomb *b = malloc(sizeof(Bomb));
    b->type = type;
    b->puissance = 1;
    b->pos.x = -1;
    b->pos.y = -1;
    b->delai = DELAI_DEFAUT_BOMBE;
    b->id_proprietaire = id_proprietaire;
    return b;
}

void init_tile(Tile* t,int type, int etat)
{
    t->type = type;
    t->etat = etat;
}

Player* init_player(char *name, int id_player)
{
    Player *p = malloc(sizeof(Player));
    //strcpy(name, p->nom);
    p->vie = 1;
    p->est_mort = 0;
    p->score = 0;
    p->bouclier = 0;
    p->vitesse = 2; /* pixels par ticks */
    p->id_player = id_player;
    p->direction = DOWN;
    p->pos.x = TILE_WIDTH;
    p->pos.y = TILE_HEIGHT;
    p->pos.h = HITBOX_PLAYER_H;
    p->pos.w = HITBOX_PLAYER_W;
    p->typebomb = *init_bomb(1, id_player);
    p->nb_bomb_max = 99;
    p->nb_bomb_jeu = 0;

    return p;
}

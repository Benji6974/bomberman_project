#include "constantes.h"
#include "jeu.h"

int gKeys[KEYS_PER_PLAYER*NB_JOUEURS] = {SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RCTRL,      /* Joueur 1 */
                                         SDLK_z, SDLK_s, SDLK_q, SDLK_d, SDLK_LCTRL,                 /* Joueur 2 */
                                         SDLK_i, SDLK_k, SDLK_j, SDLK_l, SDLK_b,                 /* Joueur 3 */
                                         SDLK_KP_8, SDLK_KP_5, SDLK_KP_4, SDLK_KP_6, SDLK_KP_ENTER}; /* Joueur 4 */

Game* init_jeu(int type, int nb_joueurs, int temps, int typemap)
{
    int i, j;
    Game *jeu = NULL;
    Tile ***carte = NULL;
    Player *p = NULL;
    /* - Tableau pour tester la carte - */
    srand(time(NULL));

    int **carte_data;

    if (typemap == -1)
    {
        carte_data =  genere_map(carte_data,nb_joueurs);
    }
    else
    {
        carte_data = lire_map_fichier(carte_data,nb_joueurs,typemap);

    }


    /* -------------------------------- */

    jeu = malloc(sizeof(Game));

    jeu->events = init_events(NB_EVENTS);
    jeu->en_pause = 0;

    /* Initialisation tableaux des entités */
    jeu->bombs = (Bomb**)malloc(NB_BOMBES_MAX*sizeof(Bomb*));
    jeu->nb_bombs = 0;
    memset(jeu->bombs, 0, NB_BOMBES_MAX*sizeof(Bomb*));

    jeu->explosions = (Explosion**)malloc(NB_EXPLOSIONS_MAX*sizeof(Explosion*));
    jeu->nb_explosions = 0;
    memset(jeu->explosions, 0, NB_EXPLOSIONS_MAX*sizeof(Explosion*));

    jeu->objets = (Objet**)malloc(NB_OBJETS_MAX*sizeof(Objet*));
    jeu->nb_objets = 0;
    memset(jeu->objets, 0, NB_OBJETS_MAX*sizeof(Objet*));

    jeu->type = type;
    jeu->time = temps*1000;
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
    detruire_map(carte_data);
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

        char name[128] = "";
        sprintf(name, "Joueur %d", i+1);
        p = init_player(name, i);
        p->keymap_offset = i*KEYS_PER_PLAYER;

        /* Positionne les joueurs aux 4 coins de la carte au centre de la case */

        p->pos.x = MAP_WIDTH-2;
        p->pos.y = MAP_HEIGHT-2;

        if(i==0)
        {
            p->pos.x = 1;
            p->pos.y = 1;
        }
        if(i==1)
        {
            p->pos.y = 1;
        }
        if(i==2)
        {
            p->pos.x = 1;
        }

        p->pos.x *= TILE_WIDTH;
        p->pos.y *= TILE_HEIGHT;

        p->pos.x += (TILE_WIDTH - p->pos.w)/2;
        p->pos.y += (TILE_HEIGHT - p->pos.h)/2;

        jeu->players[i] = p;
    }

    return jeu;
}

int** lire_map_fichier(int **carte_data, int nb_joueurs, int typemap)
{
    carte_data = (int**)malloc(MAP_HEIGHT*sizeof(int*));
    int z;
    for (z=0; z<MAP_HEIGHT; z++)
    {
        carte_data[z] = (int*)malloc(MAP_WIDTH*sizeof(int));
    }
    char fichier[128] = "";
    snprintf(fichier, sizeof fichier, "map/map%d.txt", typemap);
    FILE* fi=fopen(fichier,"r");
    int n,m;

    if(fi==NULL)
    {
        fprintf(stderr,("Erreur ouverture fichier\n" ));
        exit(0);
    }
    for(n=0; n<MAP_HEIGHT; n++)
    {
        for(m=0; m<MAP_WIDTH; m++)
        {
            fscanf(fi,"%d",&carte_data[n][m]);
        }
        fscanf(fi,"\n");
    }
    fclose(fi); /* on ferme le fichier */

    /* on remplace les cases par de l'herbe autour des joueurs*/
    int y,x;
    for (y=0; y<MAP_HEIGHT; y++)
    {
        for (x=0; x<MAP_WIDTH; x++)
        {

            if (x==0 || y == 0 || x == MAP_WIDTH-1 || y == MAP_HEIGHT-1 )
                carte_data[y][x] = -1;
            else if(nb_joueurs >= 1 && ((x == 1 && y == 1) || (x == 2 && y == 1) || (x == 1 && y == 2)))
                carte_data[y][x] = HERBE;
            else if(nb_joueurs >= 2 && ((x == MAP_WIDTH-2 && y == 1) || (x == MAP_WIDTH-3 && y == 1) || (x == MAP_WIDTH-2 && y == 2)))
                carte_data[y][x] = HERBE;
            else if(nb_joueurs >= 3 && ((x == 1 && y == MAP_HEIGHT-2) || (x == 2 && y == MAP_HEIGHT-2) || (x == 1 && y == MAP_HEIGHT-3)))
                carte_data[y][x] = HERBE;
            else if(nb_joueurs >= 4 && ((x == MAP_WIDTH-2 && y == MAP_HEIGHT-2) || (x == MAP_WIDTH-3 && y == MAP_HEIGHT-2) || (x == MAP_WIDTH-2 && y == MAP_HEIGHT-3)))
                carte_data[y][x] = HERBE;

        }
    }
    return carte_data;
}

int** genere_map(int **carte_data, int nb_joueurs)
{
    carte_data = (int**)malloc(MAP_HEIGHT*sizeof(int*));
    int z;
    for (z=0; z<MAP_HEIGHT; z++)
    {
        carte_data[z] = (int*)malloc(MAP_WIDTH*sizeof(int));
    }
    int proba;
    int x,y;
    for (y=0; y<MAP_HEIGHT; y++)
    {
        for (x=0; x<MAP_WIDTH; x++)
        {
            proba = rand()%100;
            if (x==0 || y == 0 || x == MAP_WIDTH-1 || y == MAP_HEIGHT-1 )
                carte_data[y][x] = -1;
            else if(nb_joueurs >= 1 && ((x == 1 && y == 1) || (x == 2 && y == 1) || (x == 1 && y == 2)))
                carte_data[y][x] = HERBE;
            else if(nb_joueurs >= 2 && ((x == MAP_WIDTH-2 && y == 1) || (x == MAP_WIDTH-3 && y == 1) || (x == MAP_WIDTH-2 && y == 2)))
                carte_data[y][x] = HERBE;
            else if(nb_joueurs >= 3 && ((x == 1 && y == MAP_HEIGHT-2) || (x == 2 && y == MAP_HEIGHT-2) || (x == 1 && y == MAP_HEIGHT-3)))
                carte_data[y][x] = HERBE;
            else if(nb_joueurs >= 4 && ((x == MAP_WIDTH-2 && y == MAP_HEIGHT-2) || (x == MAP_WIDTH-3 && y == MAP_HEIGHT-2) || (x == MAP_WIDTH-2 && y == MAP_HEIGHT-3)))
                carte_data[y][x] = HERBE;
            else
            {
                if (proba <P_HERBE)
                    carte_data[y][x] = HERBE;
                else if(proba < P_HERBE+P_MUR_INDESTRUCTIBLE)
                {
                    carte_data[y][x] = MUR_INDESTRUCTIBLE;
                    if(carte_data[y-1][x-1] == MUR_INDESTRUCTIBLE
                            || carte_data[y][x-1] == MUR_INDESTRUCTIBLE
                            ||  carte_data[y-1][x] == MUR_INDESTRUCTIBLE
                            || carte_data[y-1][x+1] == MUR_INDESTRUCTIBLE)
                        carte_data[y][x] = MUR_BRIQUES;
                }
                else if(proba <P_HERBE+P_MUR_INDESTRUCTIBLE+P_MUR_BRIQUES)
                    carte_data[y][x] = MUR_BRIQUES;
                else if(proba <P_HERBE+P_MUR_INDESTRUCTIBLE+P_MUR_BRIQUES+P_MUR_SOLIDE)
                    carte_data[y][x] = MUR_SOLIDE;

            }

        }
    }
    return carte_data;
}

int maj_jeu(Game *jeu, int dt)
{
    int i;

    maj_bombs(jeu, dt);
    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        maj_joueur(jeu, i, dt);
    }

    jeu->time -= dt;

    return verif_fin_de_jeu(jeu);
}

int verif_fin_de_jeu(Game *jeu)
{
    int i, joueur, egalite = 0, en_vie = 0;
    char message_fin[256];
    Player *meilleur_score = NULL;

    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        if(!jeu->players[i]->est_mort)
        {
            en_vie++;
            joueur = i+1;
            if(meilleur_score == NULL || jeu->players[i]->score > meilleur_score->score)
            {
                meilleur_score = jeu->players[i];
            }
            egalite = meilleur_score == NULL || jeu->players[i]->score == meilleur_score->score;
        }
    }
    if(en_vie == 1)
    {
        sprintf(message_fin, "%s gagne!", jeu->players[joueur]->nom);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", message_fin, NULL);
        return 1;
    }
    else if(en_vie == 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "Tout le monde est mort!", NULL);
        return 1;
    }

    if(jeu->time <= 0)
    {
        if(!egalite)
            sprintf(message_fin, "Temps ecoule!\nGagnant: %s avec %d points", meilleur_score->nom, meilleur_score->score);
        else
            sprintf(message_fin, "Temps ecoule!\nIl y a egalite!");
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", message_fin, NULL);
        return 1;
    }
    return 0;
}

/* Fonction qui renvoie 1 si les deux rectangles a et b se superposent d'au moins un pixel, 0 sinon */
int collision_rect_rect(SDL_Rect a, SDL_Rect b)
{
    return !(a.x > b.x+b.w-1
            || a.x+a.w-1 < b.x
            || a.y > b.y+b.h-1
            || a.y+a.h-1 < b.y);
}

/* Fonction qui renvoie 1 si le rectangle du joueur se superpose avec une des cases de la carte, 0 sinon */
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
            if(jeu->carte[y][x]->type != 0)
                return 1;
        }
    }

    return 0;
}

/* Fonction qui renvoie 1 si le rectangle du joueur se superpose avec celui d'une bombe, 0 sinon
 * Si le joueur était déjà sur une bombe avant (comme au moment de la poser), elle ne prend pas en compte la superposition
 */
int collision_joueur_bombes(Game *jeu, int joueur, int last_col)
{
    if(!ACTIVER_COLLISIONS)
        return 0;

    int i;
    SDL_Rect pos_pixel;

    pos_pixel.w = TILE_WIDTH;
    pos_pixel.h = TILE_HEIGHT;

    /* test collision avec les bombes */
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        pos_pixel.x = jeu->bombs[i]->pos.x*TILE_WIDTH;
        pos_pixel.y = jeu->bombs[i]->pos.y*TILE_HEIGHT;

        if(collision_rect_rect(pos_pixel, jeu->players[joueur]->pos)
                &&(jeu->bombs[i]->id_proprietaire+1 != last_col || last_col == 0))
            return jeu->bombs[i]->id_proprietaire+1; /* permet d'eviter que les joueurs utilisent leurs propres bombes pour traverser une bombe adverse */
    }

    return 0;
}

/* Fonction qui vérifie la superposition du rectangle du joueur avec celui d'un objet bonus, pour éventuellement appeller donner_bonus */
int collision_joueur_items(Game *jeu, int joueur)
{
    int i, j;
    for(i = 0; i < jeu->nb_objets; i++)
    {
        if (jeu->objets[i] != NULL)
        {
            if(collision_rect_rect(jeu->objets[i]->pos, jeu->players[joueur]->pos))
            {
                if(donner_bonus(jeu, joueur, jeu->objets[i]->type))
                {
                    free(jeu->objets[i]);
                    jeu->objets[i] = NULL;

                    for(j = i; j < jeu->nb_objets; j++)
                    {
                        if(jeu->objets[j+1] != NULL)
                        {
                            jeu->objets[j] = jeu->objets[j+1];
                            jeu->objets[j+1] = NULL;
                        }
                    }

                    jeu->nb_objets--;
                    jeu->players[joueur]->score += SCORE_ITEM_GET;

                    SDL_PushEvent(jeu->events[BONUS_OBTENU]);

                    return 1;
                }
            }
        }

    }
    return 0;
}

/* Fonction qui modifie les stats du joueur en fonction de l'objet bonus ramassé */
int donner_bonus(Game *jeu, int joueur, int type)
{
    int succes = 0;
    Player *p = jeu->players[joueur];

    switch(type)
    {
    case ITEM_SHIELD:
        if(p->bouclier < 1)
        {
            p->bouclier = 1;
            succes = 1;
        }
        break;
    case ITEM_RANGE:
        if(p->typebomb.puissance < BONUS_MAX_RANGE)
        {
            p->typebomb.puissance++;
            succes = 1;
        }
        break;
    case ITEM_BOMB:
        if(p->nb_bomb_max < BONUS_MAX_BOMB)
        {
            p->nb_bomb_max++;
            succes = 1;
        }
        break;
    case ITEM_SPEED:
        if(p->vitesse < BONUS_MAX_SPEED)
        {
            p->vitesse++;
            succes = 1;
        }
        break;
    }

    return succes;
}

/* Fonction qui copie la structure Bomb du joueur pour ensuite la poser sur la case ou il se trouve */
int poser_bomb(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    Bomb *b = NULL;
    int i, x, y;
    if (p->nb_bomb_jeu < p->nb_bomb_max && jeu->nb_bombs < NB_BOMBES_MAX)
    {
        /* Milieu de la hitbox du joueur */
        x = (p->pos.x + p->pos.w/2)/TILE_WIDTH;
        y = (p->pos.y + p->pos.h/2)/TILE_HEIGHT;

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

        SDL_PushEvent(jeu->events[BOMBE_POSEE]);
    }
    else
        return -1;
    return 0;
}

/* Fonction qui applique des effets sur une case, tel que détruire un mur ou blesser un joueur */
int degats_case(Game *jeu, Bomb *origine, int x, int y)
{
    int i, obstacle = 2;
    Tile   *t = NULL;
    Player *p = NULL;
    Bomb   *b = NULL;
    SDL_Rect rect_case;

    rect_case.x = x*TILE_WIDTH;
    rect_case.y = y*TILE_HEIGHT;
    rect_case.w = TILE_WIDTH;
    rect_case.h = TILE_HEIGHT;

    /* si la case est hors de la map, on arrête tout et on renvoie 1 */
    if(x < 0 || y < 0 || x > MAP_WIDTH || y > MAP_HEIGHT)
        return 1;

    t = jeu->carte[y][x];

    /* dommages aux murs */
    switch(t->type)
    {
    case HERBE:
        obstacle = 0;
        break;
    case MUR_BRIQUES:
        jeu->players[origine->id_proprietaire]->score += SCORE_MUR_BRIQUES;
        generer_bonus(jeu, x, y, t->type);
        t->type = 0;
        obstacle = 1;
        break;
    case MUR_SOLIDE:
        t->etat--;
        if(t->etat <= 0)
        {
            jeu->players[origine->id_proprietaire]->score += SCORE_MUR_SOLIDE;
            generer_bonus(jeu, x, y, t->type);
            t->type = 0;
            obstacle = 1;
        }
        break;
    }

    /* dommages aux joueurs */
    for(i = 0; i < jeu->nb_joueurs; i++)
    {
        p = jeu->players[i];
        if(collision_rect_rect(rect_case, p->pos) && !p->est_mort)
        {
            if(p->invincible > 0)
                continue;

            if(p->bouclier)
            {
                p->bouclier = 0;
                p->invincible = DUREE_INVINCIBILITE;
            }
            else
            {
                p->vie--;
                p->invincible = DUREE_INVINCIBILITE;
                if(p->vie <= 0)
                {
                    p->est_mort = 1;
                    jeu->players[origine->id_proprietaire]->score += (origine->id_proprietaire != p->id_player) ? SCORE_JOUEUR_KILL : 0;
                }
                else
                    jeu->players[origine->id_proprietaire]->score += (origine->id_proprietaire != p->id_player) ? SCORE_JOUEUR_HIT : 0;
            }
        }
    }

    /* reaction en chaine bombes */
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        b = jeu->bombs[i];
        if(b->pos.x == x && b->pos.y == y && b->delai > 100)
            b->delai = 100;
    }

    /* Renvoie 0 si aucun mur n'a été touché, 1 si un mur a été détruit et 2 si il mur a été rencontré mais pas détruit */
    return obstacle;
}

/* Fonction qui fait apparaître un objet bonus sur une case avec une cartaine probabilité */
void generer_bonus(Game *jeu, int x, int y, int t)
{
    int i, type;
    int chanceMur = rand()%100;
    int chanceItem = rand()%100;
    Objet* o = NULL;

    int proba_mur = 0;

    switch(t)
    {
    case MUR_BRIQUES:
        proba_mur = PROBA_MUR_BRIQUES;
        break;
    case MUR_SOLIDE:
        proba_mur = PROBA_MUR_SOLIDE;
        break;
    }

    if(chanceMur < proba_mur)
    {
        if(chanceItem < P_SHIELD)
            type = 0;
        else if(chanceItem < P_RANGE+P_SHIELD)
            type = 1;
        else if(chanceItem < P_RANGE+P_SHIELD+P_BOMB)
            type = 2;
        else
            type = 3;

        o = init_objet(type);
        o->pos.x = x*TILE_WIDTH + (TILE_WIDTH - o->pos.w)/2;
        o->pos.y = y*TILE_HEIGHT + (TILE_HEIGHT - o->pos.h)/2;

        i = jeu->nb_objets;

        jeu->objets[i] = o;
        jeu->nb_objets++;
    }
}

/* Fonction qui détruit une bombe et appelle degats_case sur les cases adjacentes (plus si le joueur possède un bonus portée) */
int exploser_bombe(Game *jeu, int bombe)
{
    int i, x, y, d;
    Player *p = jeu->players[jeu->bombs[bombe]->id_proprietaire];
    Bomb *b = jeu->bombs[bombe];
    Explosion *e;

    if(jeu->nb_bombs > 0 && b != NULL)
    {

        x = b->pos.x;
        y = b->pos.y;

        SDL_PushEvent(jeu->events[BOMBE_EXPLOSE]);

        /* destruction du décor et des joueurs */

        /* Explosion à la position de la bombe */
        degats_case(jeu, b, x, y);
        init_explosion(jeu, b, x, y, 0, DUREE_DEFAUT_EXPLOSION);


        /* vers le haut */
        for(i = 1; i <= b->puissance; i++)
        {
            d = degats_case(jeu, b, x, y-i);
            if(d != 2)
            {
                e = init_explosion(jeu, b, x, y-i, 1, DUREE_DEFAUT_EXPLOSION);
                if(d == 1 || i == b->puissance)
                {
                    e->aspect = 3;
                    break;
                }
            }
            else
                break;
        }

        /* vers le bas */
        for(i = 1; i <= b->puissance; i++)
        {
            d = degats_case(jeu, b, x, y+i);
            if(d != 2)
            {
                e = init_explosion(jeu, b, x, y+i, 1, DUREE_DEFAUT_EXPLOSION);
                if(d == 1 || i == b->puissance)
                {
                    e->aspect = 4;
                    break;
                }
            }
            else
                break;
        }

        /* vers la droite */
        for(i = 1; i <= b->puissance; i++)
        {
            d = degats_case(jeu, b, x+i, y);
            if(d != 2)
            {
                e = init_explosion(jeu, b, x+i, y, 2, DUREE_DEFAUT_EXPLOSION);
                if(d == 1 || i == b->puissance)
                {
                    e->aspect = 5;
                    break;
                }
            }
            else
                break;
        }

        /* vers la gauche */
        for(i = 1; i <= b->puissance; i++)
        {
            d = degats_case(jeu, b, x-i, y);
            if(d != 2)
            {
                e = init_explosion(jeu, b, x-i, y, 2, DUREE_DEFAUT_EXPLOSION);
                if(d == 1 || i == b->puissance)
                {
                    e->aspect = 6;
                    break;
                }
            }
            else
                break;
        }


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

/* Fonction qui met à jour l'attribut delai des bombes en jeu, puis les fait exploser si il atteint 0 */
void maj_bombs(Game *jeu, int dt)
{
    int i, j;
    Explosion *e = NULL;

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

    /* Mise a jour des explosions */
    for(i = 0; i < jeu->nb_explosions; i++)
    {
        e = jeu->explosions[i];
        if(e != NULL && e->temps_restant > 0)
        {
            e->temps_restant -= dt;
            degats_case(jeu, e->origine, e->pos.x/TILE_WIDTH, e->pos.y/TILE_HEIGHT); /* degats persistants seulement aux joueurs */
            if(e->temps_restant <= 0)
            {
                free(e);
                jeu->explosions[i] = NULL;

                for(j = i; j < jeu->nb_explosions; j++)
                {
                    if(jeu->explosions[j+1] != NULL)
                    {
                        jeu->explosions[j] = jeu->explosions[j+1];
                        jeu->explosions[j+1] = NULL;
                    }
                }

                jeu->nb_explosions--;
            }
        }
    }
}

/* Fonction qui met à jour la position et direction du joueur en fonction des touches pressées
 * Appelle poser_bomb si la touche correspondante est pressée
 */
void maj_joueur(Game *jeu, int joueur, int dt)
{
    Player *p = jeu->players[joueur];
    int k = p->keymap_offset, collision_decor = 0, collision_bombes = 0;
    int move_x = 0, move_y = 0, dx, dy;

    dx = p->pos.x;
    dy = p->pos.y;

    if(p->est_mort)
        return;

    if(p->invincible > 0)
        p->invincible -= dt;

    /* collision avant deplacement */
    collision_decor = collision_joueur_decor(jeu, joueur);
    collision_bombes = collision_joueur_bombes(jeu, joueur, 0);
    collision_joueur_items(jeu, joueur);

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
                || (collision_joueur_bombes(jeu, joueur, collision_bombes) ) ) )
        p->pos.x -= move_x;

    p->pos.y += move_y*p->vitesse;
    while(move_y
            && ((collision_joueur_decor(jeu, joueur) && !collision_decor)
                || (collision_joueur_bombes(jeu, joueur, collision_bombes) ) ) )
        p->pos.y -= move_y;

    /* Correction direction si le joueur longe un mur */
    dx = p->pos.x - dx;
    dy = p->pos.y - dy;

    if(dx == 0 && dy > 0)
        p->direction = DOWN;
    else if(dx == 0 && dy < 0)
        p->direction = UP;
    else if(dx < 0 && dy == 0)
        p->direction = LEFT;
    else if(dx > 0 && dy == 0)
        p->direction = RIGHT;

    /* Si le joueur appuie sur la touche pour poser une bombe */
    if(jeu->touches.keys_pressed[k+BOMB])
        poser_bomb(jeu, joueur);
}

/* Fonction qui met à jour la carte des touches pressées en fonction de l'évènement SDL reçus */
void maj_controles(Controls *controles, SDL_Event *event)
{
    if(event->type != SDL_KEYDOWN && event->type != SDL_KEYUP)
        return;

    int i;
    for(i = 0; i < controles->num_keys; i++)
    {
        if(event->key.keysym.sym == controles->key_map[i])
            controles->keys_pressed[i] = event->type == SDL_KEYDOWN;
    }
}

void maj_control_menu(SDL_Event *event,Menu *m)
{
    SDL_Rect spritepos;


        /* Si oui, quel type? */
	switch (event->type)
	{
          /* Appui sur une touche */

          /* Déplacement souris */
	  case SDL_MOUSEMOTION:
	       spritepos.x += event->motion.xrel;
	       spritepos.y += event->motion.yrel;
	       printf("Position souris: %d %d\n", event->motion.x,
		      event->motion.y);
	       break;
          /* Enfoncement bouton souris */
	  case SDL_MOUSEBUTTONDOWN:
	      if (!m->clic)
          {
               printf("Click bouton souris %d\n", event->button.button);
               m->clic = 1;
               maj_menu_clic(event->motion.x,event->motion.y,m);
          }

	       break;
          /* Relâchement bouton souris */
	  case SDL_MOUSEBUTTONUP:
	      if (m->clic)
          {
              printf("Relâchement bouton souris %d\n", event->button.button);
              m->clic =0;
          }

	       break;



    }

}

void maj_menu_clic(int x, int y,Menu *m)
{
        if (y> 4*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE && y< 4*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE +TAILLE_CHAR_H*RENDER_SCALE &&
        x> 20*TAILLE_CHAR_W*RENDER_SCALE && x<21*TAILLE_CHAR_W*RENDER_SCALE)
        {
            if (m->nb_joueurs>1){
                m->nb_joueurs--;
            }
        }
        if (y> 4*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE && y< 4*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE +TAILLE_CHAR_H*RENDER_SCALE &&
        x> 24*TAILLE_CHAR_W*RENDER_SCALE && x<25*TAILLE_CHAR_W*RENDER_SCALE)
        {
            if (m->nb_joueurs<4){
                m->nb_joueurs++;
            }
        }
        if (y> 6*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE && y< 6*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE +TAILLE_CHAR_H*RENDER_SCALE &&
        x> 15*TAILLE_CHAR_W*RENDER_SCALE && x<16*TAILLE_CHAR_W*RENDER_SCALE)
        {
            if (m->temps>0){
                m->temps -=10;
            }
        }
                 if (y> 6*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE && y< 6*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE +TAILLE_CHAR_H*RENDER_SCALE &&
        x> 23*TAILLE_CHAR_W*RENDER_SCALE && x<24*TAILLE_CHAR_W*RENDER_SCALE)
        {
            if (m->temps<3600){
                m->temps +=10;
            }
        }

         if (y> 2*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE && y< 2*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE +TAILLE_CHAR_H*RENDER_SCALE &&
        x> 14*TAILLE_CHAR_W*RENDER_SCALE && x<15*TAILLE_CHAR_W*RENDER_SCALE)
        {
            if (m->map_jeu>-1){
                m->map_jeu--;
            }
        }
                 if (y> 2*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE && y< 2*TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE +TAILLE_CHAR_H*RENDER_SCALE &&
        x> 26*TAILLE_CHAR_W*RENDER_SCALE && x<27*TAILLE_CHAR_W*RENDER_SCALE)
        {
            if (m->map_jeu<NB_MAP){
                m->map_jeu++;
            }
        }

                 if (y> MAP_HEIGHT*TILE_HEIGHT*RENDER_SCALE - TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE && y<MAP_HEIGHT*TILE_HEIGHT*RENDER_SCALE - TILE_HEIGHT*RENDER_SCALE + HUD_HEIGHT*RENDER_SCALE +TAILLE_CHAR_H*RENDER_SCALE &&
        x> MAP_WIDTH*TILE_WIDTH*RENDER_SCALE - 10*TAILLE_CHAR_W*RENDER_SCALE && x<MAP_WIDTH*TILE_WIDTH*RENDER_SCALE - 2*TAILLE_CHAR_W*RENDER_SCALE)
        {
            m->lancer_jeu = 1;
        }

         if (y> 8*TILE_HEIGHT*RENDER_SCALE  && y < (8*TILE_HEIGHT*RENDER_SCALE +6*TAILLE_CHAR_H*RENDER_SCALE) &&
        x> (MAP_WIDTH*TILE_WIDTH*RENDER_SCALE - 6*TAILLE_CHAR_W*RENDER_SCALE) && x<(MAP_WIDTH*TILE_WIDTH*RENDER_SCALE - 3*TAILLE_CHAR_W*RENDER_SCALE))
        {
            m->volume_son++;
            if (m->volume_son ==4)
                m->volume_son =0;
        }

}
Menu* init_menu()
{
    Menu *m = malloc(sizeof(Menu));
    m->clic = 0;
    m->nb_joueurs = NB_JOUEURS;
    m->temps = DUREE_DEFAUT_PARTIE;
    m->map_jeu = -1;
    m->lancer_jeu = 0;
    m->volume_son = 1;
    return m;
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

Explosion* init_explosion(Game *jeu, Bomb* origine, int x, int y, int aspect, int duree)
{
    Explosion *e = malloc(sizeof(Explosion));

    e->origine = origine;

    e->pos.x = x*TILE_WIDTH;
    e->pos.y = y*TILE_HEIGHT;
    e->pos.w = TILE_WIDTH;
    e->pos.h = TILE_HEIGHT;
    e->aspect = aspect;

    e->duree_de_vie = duree;
    e->temps_restant = duree;

    jeu->explosions[jeu->nb_explosions] = e;
    jeu->nb_explosions++;

    return e;
}

Objet* init_objet(int type)
{
    Objet* o = malloc(sizeof(Objet));
    o->type= type;
    o->pos.h = HITBOX_ITEM_W;
    o->pos.w = HITBOX_ITEM_H;

    return o;
}

void init_tile(Tile* t,int type, int etat)
{
    t->type = type;
    t->etat = etat;
}

Player* init_player(char *name, int id_player)
{
    Player *p = malloc(sizeof(Player));
    strncpy(p->nom,name,256);
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
    p->nb_bomb_max = 1;
    p->nb_bomb_jeu = 0;

    return p;
}

SDL_Event** init_events(int num)
{
    int i, event_num;
    SDL_Event **events;

    events = (SDL_Event**)malloc(num*sizeof(SDL_Event*));
    event_num = SDL_RegisterEvents(num);

    for(i = 0; i < num; i++)
    {
        events[i] = (SDL_Event*)malloc(sizeof(SDL_Event));
        events[i]->user.type = (Uint32)(event_num + i);
        events[i]->user.code = i;
        events[i]->user.data1 = NULL;
        events[i]->user.data2 = NULL;
    }

    return events;
}

void detruire_map(int **carte_data)
{
    int i,j;
    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            free(carte_data[i][j]);
        }
        free(carte_data[i]);
    }
    free(carte_data);
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

    for(i = 0; i < jeu->nb_explosions; i++)
    {
        free(jeu->explosions[i]);
    }

    /* Libération du tableau des objets */
    for(i = 0; i < jeu->nb_objets; i++)
    {
        free(jeu->objets[i]);
    }

    for(i = 0; i < NB_EVENTS; i++)
    {
        free(jeu->events[i]);
    }

    free(jeu->players);
    free(jeu->bombs);
    free(jeu->explosions);
    free(jeu->objets);

    free(jeu->events);

    /* Destruction du jeu */
    free(jeu);
}

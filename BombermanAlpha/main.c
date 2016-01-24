#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>

#include "constantes.h"
#include "jeu.h"
#include "graphismes.h"

#define graphismes 1

void affiche_carte(Tile ***carte)
{
    int i, j;
    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            printf("%d ", carte[i][j]->type);
        }
        printf("\n");
    }
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

    int i=0, x=0, y=0, x1=0, x2=0, y1=0, y2=0;
    const SDL_Rect pos = jeu->players[joueur]->pos, *pos_bomb;

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

int collision_joueur_objets(Game *jeu, int joueur)
{
    if(!ACTIVER_COLLISIONS)
        return 0;

    int i=0;

    /* test collision avec les bombes */
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        if(collision_tile_rect(jeu->bombs[i]->pos.x, jeu->bombs[i]->pos.y, jeu->players[joueur]->pos))
            return 1;
    }

    return 0;
}

int poser_bomb(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    int i = 0;
    if (p->nb_bomb_jeu < p->nb_bomb_max && jeu->nb_bombs < NB_BOMBES_MAX)
    {
        Bomb *b = malloc(sizeof(Bomb));
        *b = *(p->typebomb);
        int x1 = p->pos.x/TILE_WIDTH;
        int y1 = p->pos.y/TILE_HEIGHT;
        b->pos.x = x1;
        b->pos.y = y1;
        b->delai = DELAI_DEFAUT_BOMBE;

        for(i = 0; jeu->bombs[i] != NULL; i++)
        {
            if(jeu->bombs[i]->pos.x == b->pos.x && jeu->bombs[i]->pos.y == b->pos.y)
            {
                free(b);
                return -1;
            }
        }
        jeu->bombs[i] = b;
        jeu->nb_bombs++;
        p->nb_bomb_jeu++;
    }
    return 0;
}

int exploser_bombe(Game *jeu, int bombe)
{
    int i, j, x, y, x2, y2;
    Player *p = jeu->players[jeu->bombs[bombe]->id_proprietaire];
    Bomb *b = jeu->bombs[bombe];

    if(jeu->nb_bombs > 0 && b != NULL)
    {
        x = b->pos.x;
        y = b->pos.y;

        /* destruction du décor et des joueurs */

        /* verticalement */
        for(i = 0; i < b->puissance*2 + 1; i++)
        {
            y2 = y-b->puissance+i;
            if(y2 < MAP_HEIGHT && y2 >= 0)
            {
                switch(jeu->carte[y2][x]->type)
                {
                case 2:
                    jeu->carte[y2][x]->type = 0;
                    break;
                }

                for(j = 0; j < jeu->nb_joueurs; j++)
                {
                    if(collision_tile_rect(x, y2, jeu->players[j]->pos) && jeu->players[j]->vie > 0)
                    {
                        jeu->players[j]->vie--;
                    }
                }
            }
        }

        /* horizontalement */
        for(i = 0; i < b->puissance*2 + 1; i++)
        {
            x2 = x-b->puissance+i;
            if(x2 < MAP_WIDTH && x2 >= 0)
            {
                switch(jeu->carte[y][x2]->type)
                {
                case 2:
                    jeu->carte[y][x2]->type = 0;
                    break;
                }

                for(j = 0; j < jeu->nb_joueurs; j++)
                {
                    if(collision_tile_rect(x2, y, jeu->players[j]->pos) && jeu->players[j]->vie > 0)
                    {
                        jeu->players[j]->vie--;
                    }
                }
            }
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

void maj_bombs(Game *jeu, int dt)
{
    int i;
    for(i = 0; i < jeu->nb_bombs; i++)
    {
        if(jeu->bombs[i]->delai > 0)
        {
            /* gestion du delai avant explosion des bombes */
            jeu->bombs[i]->delai -= dt;
            printf("%d\n", jeu->bombs[i]->delai);
            if(jeu->bombs[i]->delai <= 0)
            {
                exploser_bombe(jeu, i);
                printf("BOOM!\n");
            }
        }
    }
}

void maj_joueur(Game *jeu, int joueur)
{
    Player *p = jeu->players[joueur];
    int i, k = p->keymap_offset, collision_decor = 0, collision_objets = 0;
    int move_x = 0, move_y = 0;

    if(p->vie <= 0)
        return;

    collision_decor = collision_joueur_decor(jeu, joueur);
    collision_objets = collision_joueur_objets(jeu, joueur);

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
        && (collision_joueur_decor(jeu, joueur) && !collision_decor
        || collision_joueur_objets(jeu, joueur) && !collision_objets))
        p->pos.x -= move_x;

    p->pos.y += move_y*p->vitesse;
    while(move_y
        && (collision_joueur_decor(jeu, joueur) && !collision_decor
        || collision_joueur_objets(jeu, joueur) && !collision_objets))
        p->pos.y -= move_y;

    /* Si le joueur appuie sur la touche pour poser une bombe */
    if(jeu->touches.keys_pressed[k+BOMB])
        poser_bomb(jeu, joueur);
}

int main(int agrc, char** argv)
{

    Game *jeu = init_jeu(0, 3, 0);

    affiche_carte(jeu->carte);

#if graphismes
    /* Ceci est un test de la SDL2 */
    SDL_Init(SDL_INIT_VIDEO);

    SDL_version version;
    SDL_VERSION(&version)

    SDL_Window *fenetre = init_fenetre("Bomberman Alpha", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN, 0);
    SDL_Renderer *renderer = SDL_GetRenderer(fenetre);

    SDL_RendererInfo info;
    SDL_GetRenderDriverInfo(0, &info);

    char message[50];
    sprintf(message, "Version SDL: %d.%d.%d\nMoteur de rendu: %s\nTaille de la fenetre: %d*%d\n", version.major, version.minor, version.patch, info.name, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SDL initialisee", message, fenetre);

    SDL_Texture *feuille_tiles = charger_sprite(renderer, "img/tileset.bmp");
    SDL_Texture *feuille_perso = charger_sprite(renderer, "img/character_silver.bmp");
    SDL_Texture *feuille_objets = charger_sprite(renderer, "img/atlas5_32.bmp");

    SDL_Rect pos;
    pos.x = 0;
    pos.y = 0;
    pos.w = TILE_WIDTH;
    pos.h = TILE_HEIGHT;

    /* Permet de récupérer une region spécifique de la feuille de tiles */
    SDL_Rect clip;
    clip.x = 0;
    clip.y = 0;
    clip.w = TILE_WIDTH;
    clip.h = TILE_HEIGHT;

    SDL_Rect clip_perso;
    clip_perso.x = PLAYER_INDEX_X;
    clip_perso.y = PLAYER_INDEX_Y;
    clip_perso.w = 23;
    clip_perso.h = 38;

    SDL_Rect pos_perso;
    pos_perso.x = 0;
    pos_perso.y = 0;
    pos_perso.w = 23;
    pos_perso.h = 38;

    int stop = 0, current_time = 0, previous_time = 0, previous_time2 = 0, frame_compte = 0, i, j, en_vie = 0, joueur = 0;
    int dt = 0;
    SDL_Event event;

    SDL_Rect pos_pres;

    while(!stop)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            stop = 1;

        switch(event.type)
        {
        case SDL_MOUSEMOTION:
            /*
            pos_pres = jeu->players[0]->pos;
            jeu->players[0]->pos.x = event.motion.x - pos.w/2;
            jeu->players[0]->pos.y = event.motion.y - pos.h/2;
            if(collision_joueur(jeu, 0))
                jeu->players[0]->pos = pos_pres;
            */
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            for(i = 0; i < jeu->touches.num_keys; i++)
            {
                if(event.key.keysym.sym == jeu->touches.key_map[i])
                    jeu->touches.keys_pressed[i] = (event.type == SDL_KEYDOWN) ? 1 : 0;
            }
            break;
        }

        current_time = SDL_GetTicks();
        dt = current_time - previous_time;
        if(dt >= 1000/MAJ_PAR_SEC)
        {
            en_vie = 0;
            for(i = 0; i < jeu->nb_joueurs; i++)
            {
                maj_joueur(jeu, i);
                if(jeu->players[i]->vie > 0)
                {
                    en_vie++;
                    joueur = i+1;
                }
            }
            maj_bombs(jeu, dt);
            if(en_vie == 1)
            {
                stop = 1;
                char *nom_joueur[256];
                sprintf(nom_joueur, "Joueur %d gagne!", joueur);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fin de partie", nom_joueur, fenetre);
            }
            else if(en_vie == 0)
            {
                stop = 1;
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Fin de partie", "Tout le monde est mort!", fenetre);
            }
            previous_time = current_time;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        /* Test dessin de la carte */
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
                /* Herbe */
                default:
                case 0:
                    clip.x = 0;
                    clip.y = 0;
                    break;
                /* Mur indestructible */
                case 1:
                    clip.x = 0;
                    clip.y = 10;
                    break;
                /* Mur desctructible */
                case 2:
                    clip.x = 1;
                    clip.y = 13;
                    /* Sprite alternatif si la case juste en dessous n'est pas vide */
                    if(i+1 < MAP_HEIGHT && jeu->carte[i+1][j] != NULL && jeu->carte[i+1][j]->type != 0)
                        clip.x = 0;
                    break;
                }

                clip.x *= TILE_WIDTH;
                clip.y *= TILE_HEIGHT;
                SDL_RenderCopy(renderer, feuille_tiles, &clip, &pos);
            }
        }

        for(i = 0; i < jeu->nb_bombs; i++)
        {
            clip.x = BOMB_INDEX_X*TILE_WIDTH;
            clip.y = BOMB_INDEX_Y*TILE_HEIGHT;
            pos.x = jeu->bombs[i]->pos.x*TILE_WIDTH;
            pos.y = jeu->bombs[i]->pos.y*TILE_HEIGHT;
            SDL_RenderCopy(renderer, feuille_objets, &clip, &pos);
        }

        for(i = 0; i < jeu->nb_joueurs; i++)
        {
            switch(jeu->players[i]->direction)
            {
            case DOWN:
                clip_perso.y = 0;
                break;
            case RIGHT:
                clip_perso.y = 38;
                break;
            case UP:
                clip_perso.y = 2*38;
                break;
            case LEFT:
                clip_perso.y = 3*38;
                break;
            }
            pos_perso.x = jeu->players[i]->pos.x;
            pos_perso.y = jeu->players[i]->pos.y - 16;

            if(jeu->players[i]->vie > 0)
                SDL_RenderCopy(renderer, feuille_perso, &clip_perso, &pos_perso);
        }

        SDL_RenderPresent(renderer);

        current_time = SDL_GetTicks();
        frame_compte++;
        if(current_time - previous_time2 >= 1000)
        {
            printf("FPS: %d\n", frame_compte);
            frame_compte = 0;
            previous_time2 = current_time;
        }

    }

    SDL_DestroyTexture(feuille_tiles);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
#endif // graphismes

    detruire_jeu(jeu);
    return 0;
}

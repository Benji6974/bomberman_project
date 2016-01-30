#include "menu.h"

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

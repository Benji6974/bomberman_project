#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

#include "constantes.h"

typedef struct Menu
{
    int clic;
    int nb_joueurs;
    int temps;
    int map_jeu;
    int lancer_jeu;
    int volume_son;
}Menu;

void maj_control_menu(SDL_Event *event,Menu *m);
void maj_menu_clic(int x, int y,Menu *m);
Menu* init_menu();

#endif // MENU_H

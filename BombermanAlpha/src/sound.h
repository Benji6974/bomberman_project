#ifndef SOUND_H
#define SOUND_H

#ifdef __linux__
    #include <SDL2/SDL.h>
    #include <SDL/SDL_mixer.h>
#elif _WIN32
    #include <SDL.h>
    #include <SDL_mixer.h>
#else

#endif

#include <stdio.h>
#include <stdlib.h>

#include "constantes.h"
#include "jeu.h"

#define NB_CANAUX 32

int init_audio(int);

void changer_volume(int);

int maj_audio(void*, SDL_Event*);

void detruire_audio();

#endif // SOUND_H

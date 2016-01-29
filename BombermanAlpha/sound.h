#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>

int init_audio(int);

int maj_audio(void*, SDL_Event*);

void detruire_audio();

#endif // SOUND_H

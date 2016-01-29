#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#include "sound.h"

/*int test()
{

    mix

   if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
      printf("%s", Mix_GetError());
   }
   Mix_AllocateChannels(32); //Allouer 32 canaux
   Mix_Volume(1, MIX_MAX_VOLUME/2); //Mettre à mi-volume le post 1
   Mix_Chunk *son;//Créer un pointeur pour stocker un .WAV
   Mix_Chunk *son2;
   son = Mix_LoadWAV("son1.wav"); //Charger un wav dans un pointeur
   son2 = Mix_LoadWAV("son2.wav");
   Mix_VolumeChunk(son, MIX_MAX_VOLUME/2); //Mettre un volume pour ce wav
   Mix_VolumeChunk(son2, MIX_MAX_VOLUME);

   Mix_PlayChannel(1, son, 0);//Joue le son 1 sur le canal 1 ; le joue une fois (0 + 1)


   Mix_FreeChunk(son);//Libération du son 1
   Mix_FreeChunk(son2);
   Mix_CloseAudio(); //Fermeture de l'API
   SDL_Quit();
   return EXIT_SUCCESS;
}*/

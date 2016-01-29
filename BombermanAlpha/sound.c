#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#include "sound.h"
#include "constantes.h"

Mix_Chunk *explosion,
          *bombe_posee,
          *bonus_obtenu;

int init_audio(int channels)
{
   if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
      printf("%s", Mix_GetError());
      return -1;
   }
   Mix_AllocateChannels(16); //Allouer 32 canaux
   Mix_Volume(1, MIX_MAX_VOLUME); //Mettre à mi-volume le post 1

   explosion    = Mix_LoadWAV("sound/explosion2.wav");
   bombe_posee  = Mix_LoadWAV("sound/woosh.wav");
   bonus_obtenu = Mix_LoadWAV("sound/bonus.wav");

   return 0;
}

int maj_audio(void* userdata, SDL_Event *event)
{
   if(event != NULL && event->type >= SDL_USEREVENT)
   {
      switch(event->user.code)
      {
      case BOMBE_EXPLOSE:
         Mix_PlayChannel(1, explosion, 0);
         break;
      case BOMBE_POSEE:
         Mix_PlayChannel(1, bombe_posee, 0);
         break;
      case BONUS_OBTENU:
         Mix_PlayChannel(1, bonus_obtenu, 0);
         break;
      }
      return 1;
   }
   return 0;
}

void detruire_audio()
{
   Mix_FreeChunk(explosion);//Libération du son 1
   Mix_FreeChunk(bombe_posee);
   Mix_CloseAudio(); //Fermeture de l'API
   Mix_Quit();
}

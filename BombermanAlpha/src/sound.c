#include "sound.h"

Mix_Chunk *explosion,
          *bombe_posee,
          *bonus_obtenu;

Uint32 timestamp = 0;

/* Initialisation de l'audio et chargement des sons */
int init_audio(int channels)
{
   if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
       printf("%s", Mix_GetError());
       return -1;
   }
   Mix_AllocateChannels(channels);

   explosion    = Mix_LoadWAV("data/sound/explosion.wav");
   bombe_posee  = Mix_LoadWAV("data/sound/woosh.wav");
   bonus_obtenu = Mix_LoadWAV("data/sound/bonus.wav");

   return 0;
}

/* Change le volume de chaque tout les canaux */
void changer_volume(int niveau)
{
   int i;
   for(i = 0; i < NB_CANAUX; i++)
   {
      Mix_Volume(i, niveau*MIX_MAX_VOLUME/3);
   }
}

/* Récupère un évenement utilisateur et joue le son approprié sur un canal libre */
int maj_audio(void* userdata, SDL_Event *event)
{
   int channel;
   if(event != NULL && event->type >= SDL_USEREVENT && event->user.timestamp != timestamp)
   {
      timestamp = event->user.timestamp;

      /* Determination d'un canal vide */
      for(channel = 1; channel <= NB_CANAUX && Mix_Playing(channel); channel++);

      switch(event->user.code)
      {
      case BOMBE_EXPLOSE:
         Mix_PlayChannel(channel, explosion, 0);
         break;
      case BOMBE_POSEE:
         Mix_PlayChannel(channel, bombe_posee, 0);
         break;
      case BONUS_OBTENU:
         Mix_PlayChannel(channel, bonus_obtenu, 0);
         break;
      }
      return 1;
   }
   return 0;
}

void detruire_audio()
{
   Mix_FreeChunk(explosion);
   Mix_FreeChunk(bombe_posee);
   Mix_FreeChunk(bonus_obtenu);
   Mix_CloseAudio();
   Mix_Quit();
}

#ifndef JEU_H
#define JEU_H


typedef struct Player{
    char name[256];
    int vie;
    int score;
    int bouclier;
    int sprite_index;
    int sprite_no;
    int x;
    int y;
}Player;

typedef struct Tile{
    int type;
    int etat;
    int sprite_index;
    int sprite_no;
}Tile;

typedef struct Bomb{
    int type;
    int puissance;
    int sprite_index;
    int sprite_no;
    int x;
    int y;
}Bomb;

typedef struct Game{
int type;
Player* players;
int time;
Tile** table;
}Game;

int init_jeu();

#endif // JEU_H

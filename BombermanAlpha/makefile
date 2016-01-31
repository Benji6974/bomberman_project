# Une variable pour le compilateur
CC = gcc
# Une variable regroupant les options de compilation
CFLAGS = -std=c99 -Wall -pedantic -D_GNU_SOURCE -lSDL2main -lSDL2 -lSDL_mixer 
# Une variable regroupant les options de l'edition de liens
LDFLAGS = -lSDL2main -lSDL2 -lSDL_mixer -lm
# Une variable contenant le nom des executables a  generer
EXEC = Bomberman
# Une variable contenant la liste des fichiers sources
SRC = $(wildcard src/*.c)
# Une variable contenant la liste des fichiers objets
OBJ = $(SRC:src/.c=src/.o)

# $@ : Le nom de la cible
# $< : Le nom de la premiere dépendance
# $^ : La liste des dependances
# $? : La liste des dependances plus recentes que la cible
# $* : Le nom du fichier sans suffixe

all: $(EXEC)

Bomberman: $(OBJ)
	   $(CC) $^ -o $@ $(LDFLAGS)
	   rm src/*.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
	
mrproper: clean
	rm $(EXEC)

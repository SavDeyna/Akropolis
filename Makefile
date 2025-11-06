# Nom de l'exécutable final
EXEC = programme

# Liste des fichiers sources
SRC = src/Hexagone.cpp src/main.cpp src/Participant.cpp src/Partie.cpp src/Plateau.cpp src/Tuile.cpp


OBJ = $(SRC:.cpp=.o)

# Options du compilateur
CFLAGS = -Wall -Wextra -g -Iinclude

CC = g++


all: $(EXEC)

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Lier les objets pour créer l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# Nettoyer les fichiers objets
clean:
	rm -f $(OBJ)

# Nettoyer tout (objets + exécutable)
mrproper: clean
	rm -f $(EXEC)

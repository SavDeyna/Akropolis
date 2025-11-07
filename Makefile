
EXEC =  build/Akropolis

# fichiers sources
SRC = src/Hexagone.cpp src/main.cpp src/Participant.cpp src/Partie.cpp src/Plateau.cpp src/Tuile.cpp

OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

# Options
CFLAGS = -Wall -Wextra -g -Iinclude

CC = g++

all: build $(EXEC)

build/%.o: src/%.cpp | build
	$(CC) $(CFLAGS) -c $< -o $@

# Création executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)


build :
	mkdir -p build

#Nettoyer fichies objets
clean :
	rm -rf build


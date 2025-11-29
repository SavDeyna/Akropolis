EXEC = build/Akropolis 

# fichiers sources
SRC = src/Hexagone.cpp src/main.cpp src/Participant.cpp src/Partie.cpp src/Plateau.cpp src/Tuile.cpp

OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

# Options
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude -finput-charset=UTF-8 -fexec-charset=UTF-8

all: build $(EXEC)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création executable
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

build:
	mkdir -p build

clean:
	rm -rf build

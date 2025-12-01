EXEC = build/Akropolis

SRC = src/Hexagone.cpp src/main.cpp src/Participant.cpp src/Partie.cpp src/Plateau.cpp src/Tuile.cpp
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude

all: build $(EXEC)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

build:
	mkdir -p build

clean:
	rm -rf build

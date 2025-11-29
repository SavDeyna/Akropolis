OS := $(shell uname 2>/dev/null || echo Windows)

EXEC = build/Akropolis

SRC = src/Hexagone.cpp src/main.cpp src/Participant.cpp src/Partie.cpp src/Plateau.cpp src/Tuile.cpp
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude

all: build $(EXEC)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)$(EXT)

build:
ifeq ($(OS),Windows)
	if not exist build mkdir build
else
	mkdir -p build
endif

clean:
ifeq ($(OS),Windows)
	if exist build rmdir /S /Q build
else
	rm -rf build
endif


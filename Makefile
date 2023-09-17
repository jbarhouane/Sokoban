CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic
LIB = -lsfml-window -lsfml-system -lsfml-graphics

all: Sokoban lint

lint:
	cpplint --recursive --quiet src/
Sokoban: main.o Sokoban.o 
	$(CC) $(CFLAGS) -o Sokoban main.o Sokoban.o $(LIB)
main.o: main.cpp Sokoban.cpp Sokoban.hpp
	$(CC) $(CFLAGS) -c main.cpp
Sokoban.o: Sokoban.cpp
	$(CC) $(CFLAGS) -c Sokoban.cpp
clean:
	rm *.o Sokoban

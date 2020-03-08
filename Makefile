CC = g++

CFLAGS = $(shell sdl2-config --cflags) 
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lopengl32 -lm -lpthread

all: breakout.exe

cp_functions.o: cp_functions.cpp
	$(CC) $(CFLAGS) -c cp_functions.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

breakout.exe: cp_functions.o main.o
	$(CC) -o breakout.exe cp_functions.o main.o $(LDFLAGS)

clean: 
	rm -f *.o breakout.exe

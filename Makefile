all:
	gcc -Isrc/include -L src/lib -o main main.c -lmingw32 -lSDL2main -lSDL2_ttf -lSDL2 
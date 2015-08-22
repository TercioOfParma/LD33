all:
	gcc	-c	src/main.h
	gcc	-c	src/init.c
	gcc	-c	src/main.c
	gcc	-o	Futility	main.o	init.o	-lmingw32	-lSDL2Main	-lSDL2	-lSDL2_image	-lSDL2_ttf	-lSDL2_mixer	-ljansson	-g
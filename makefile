all:
	gcc	-c	src/main.h	-g	
	gcc	-c	src/init.c	-g	
	gcc	-c	src/deinit.c	-g	
	gcc	-c	src/draw.c	-g	
	gcc	-c	src/input.c	-g	-Wall
	gcc	-c	src/main.c	-g	
	gcc	-o	Futility	main.o	init.o	deinit.o	draw.o	input.o	-lmingw32	-lSDL2Main	-lSDL2	-lSDL2_image	-lSDL2_ttf	-lSDL2_mixer	-ljansson	-g
	
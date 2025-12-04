all: main

main: main.c include/player.c include/entity.c include/generate.c include/render.c
	gcc main.c include/player.c include/entity.c include/generate.c include/render.c -o main -lncurses -I./include

clean:
	rm -f main

run: main
	./main

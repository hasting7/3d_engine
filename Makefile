build:
	gcc -Wall -Werror -std=c17 main.c linear.c engine.c -o play -I include -L lib -l SDL2-2.0.0 -lm

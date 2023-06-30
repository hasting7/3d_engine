#ifndef ENGINE
#define ENGINE

#include <SDL2/SDL.h>

struct matrix_struct;

void init();
void draw(SDL_Renderer *);
void key_press(char);
void events(SDL_Event);

extern char *NAME;
extern int WIDTH;
extern int HEIGHT;
extern SDL_Color bg;

#endif

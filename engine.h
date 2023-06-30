#ifndef ENGINE
#define ENGINE

#include <SDL2/SDL.h>

struct matrix_struct;


typedef struct camera_struct {
	struct matrix_struct *position;
	struct matrix_struct *rotation;
} Camera;

typedef struct shape_struct {

} Shape;


void init();
void draw(SDL_Renderer *);
void key_press(char);
void events(SDL_Event);

struct matrix_struct *rotational_transformation(const struct camera_struct);
struct camera_struct *create_camera(float *);

extern char *NAME;
extern int WIDTH;
extern int HEIGHT;
extern SDL_Color bg;

#endif

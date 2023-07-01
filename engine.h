#ifndef ENGINE
#define ENGINE

#include <SDL2/SDL.h>

struct matrix_struct;

struct list_element {
	void *data;
	struct list_element *next;
};

typedef struct edge_struct {
	int indices[2];
} Edge;

typedef struct camera_struct {
	void *(update)(struct camera_struct *);
	struct matrix_struct *position;
	struct matrix_struct *rotation;
	struct matrix_struct *direction;
} Camera;

typedef struct shape_struct {
	void (*render)(SDL_Renderer *, struct shape_struct *, struct camera_struct);
	struct matrix_struct *points;
	struct list_element *edges;
	SDL_Color color;
	int n_edges;
} Shape;


void init();
void draw(SDL_Renderer *);
void key_press(char);
void events(SDL_Event);

struct matrix_struct *rotational_transformation(struct matrix_struct);
void transform_shape(struct shape_struct *, struct matrix_struct); 
struct camera_struct *create_camera(float *, void (*)(struct camera_struct *));
struct shape_struct *create_shape(int, float *);
void free_shape(struct shape_struct **);
SDL_Point *project_shape(struct shape_struct, struct camera_struct); 
void render_shape(SDL_Renderer *, struct shape_struct *, struct camera_struct);
void rotate_shape(struct shape_struct *, struct matrix_struct);
struct shape_struct *populate_shape(char *);
void rotate_shape(struct shape_struct *, struct matrix_struct);

extern char *NAME;
extern int WIDTH;
extern int HEIGHT;
extern SDL_Color bg;
extern Camera *main_camera;
extern Shape *X;
extern Shape *Y;
extern Shape *Z;
extern float zero[3];
extern struct matrix_struct *invert;

#endif

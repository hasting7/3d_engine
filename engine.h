#ifndef ENGINE
#define ENGINE

#include <SDL2/SDL.h>

struct matrix_struct;

typedef struct camera_struct {
	float rotation[3];
	struct matrix_struct *position;
} Camera;

struct renderable_object {
	struct shape_struct *object;
	struct renderable_object *next;
	struct camera_struct *camera;
};

typedef struct edge_struct {
	struct edge_struct *next;
	int nodes[2];
} Edge;

typedef struct shape_struct {
	void (*update)(struct shape_struct *);
	void (*render)(SDL_Renderer *, struct shape_struct *, struct camera_struct *);
	struct matrix_struct *points;
	struct edge_struct *edges;
	struct matrix_struct *center;
	int n_points;
	int n_edges;
	SDL_Color color;
	char *name;
} Shape;

void init();
void draw(SDL_Renderer *);
void key_press(char);
void events(SDL_Event);
void render_shape(SDL_Renderer *, struct shape_struct *, struct camera_struct *);
SDL_Point *project_shape(struct shape_struct, struct camera_struct *); 
struct shape_struct *generate_object(char *, void (*)(struct shape_struct *), struct camera_struct *);
struct shape_struct *create_shape(int, void (*)(struct shape_struct *), struct camera_struct *);
void add_edge(struct shape_struct *, int *);
struct camera_struct *create_camera(float *);
extern char *NAME;
extern int WIDTH;
extern int HEIGHT;
extern SDL_Point origin;
extern SDL_Color bg;

#endif

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"

Matrix *projection;
Matrix *camera_inverse;
struct renderable_object *collection;
SDL_Color white = { .r = 255, .b = 255, .g = 255};
float projection_data[16] = {
	1,	0,	0,	0,
	0,	1,	0,	0,
	0,	0,	0,	0,
	0,	0,	0,	1
};

float camera_manuver[3] = {
	-1,	-1, 0
};

float distance(struct matrix_struct m1, struct matrix_struct m2) {
	assert((m1.n == 1) && (m2.n == 1));
	float diffrence[3] = { 0 };
	for (int i = 0; i < 3; i++) {
		diffrence[i] = pow(m1.content[i] - m2.content[i],2);
	}
	return sqrt(
		diffrence[0] + diffrence[1] + diffrence[2]
	);
}

Shape *create_shape(int points, void (*update_func)(struct shape_struct *), struct camera_struct *camera) {
	struct renderable_object *container = malloc(sizeof(struct renderable_object));
	Shape *new = malloc(sizeof(Shape));
	new->n_points = points;
	
	new->center = create_matrix(3, 1, NULL);
	new->points = create_matrix(4, points, NULL);
	new->update = update_func;
	new->render = &render_shape;

	container->object = new;
	container->camera = camera;
	container->next = collection;
	collection = container;

	return new;
}

SDL_Point *project_shape(struct shape_struct shape, struct camera_struct *camera) {
	SDL_Point *result = calloc(shape.n_points, sizeof(SDL_Point));


	projection = create_matrix(4,4, projection_data);
	printf("Distance from shape to camera: %.4f\n",distance(*shape.center, *camera->position));
	set_entry(projection, 3, 2, -1.0 / distance(*shape.center, *camera->position)); 
	// set_entry(projection, 3, 2, -1.0 / camera->position->content[2]); 

	Matrix *camera_offset = multiply_matrices(camera_inverse,camera->position);
	Matrix *temp = offset(*shape.points, *camera_offset);
	temp = offset(*temp, *shape.center);
	// Matrix *temp = shape.points;

	for (int i = 0; i < temp->n; i++) { // set H val of coords to 1;
		set_entry(temp, temp->m - 1, i, 1);
	}

	Matrix *product = multiply_matrices(projection, temp);
	for (int i = 0; i < shape.n_points; i++) {
		
		float scalar = *target_entry(*product, product->m - 1, i);

		for (int j = 0; j < product->m; j++) {
			float *target = target_entry(*product, j, i);
			*target /= scalar;
		}
		
		SDL_Point point = { .x = *target_entry(*product, 0, i), .y = *target_entry(*product, 1, i)};
		result[i] = point;	
	}

	display_matrix(shape.points);
	free(product);
	free(projection);
	free(temp);
	free(camera_offset);

	return result;
}

void render_shape(SDL_Renderer *render, struct shape_struct *shape, struct camera_struct *camera) {

	SDL_SetRenderDrawColor(render, shape->color.r, shape->color.g, shape->color.b, 255);

	shape->update(shape);

	SDL_Point *points = project_shape(*shape, camera);

	for (int i = 0; i < shape->n_points; i++) {
		printf("point (%d) at (x = %d, y = %d)\n", i, points[i].x, points[i].y);
		SDL_RenderDrawPoint(render, points[i].x, points[i].y);
	}

	Edge *walk = shape->edges;
	while (walk) {
		SDL_RenderDrawLine(render, points[walk->nodes[0]].x,
								   points[walk->nodes[0]].y,
								   points[walk->nodes[1]].x,
								   points[walk->nodes[1]].y
						    );
		walk = walk->next;
	}

	free(points);
}

void add_edge(Shape *shape, int *nodes) {
	Edge *new = malloc(sizeof(Edge));
	memcpy(new->nodes, nodes, sizeof(int) * 2);

	new->next = shape->edges;
	shape->edges = new;
}

struct shape_struct *generate_object(char *filename, void (*update_func)(struct shape_struct *), struct camera_struct *camera) {
	FILE *fp = fopen(filename, "r");
	assert(fp);

	char name[256] = { 0 };
	int points;
	int edges;
	float origin[3] = { 0 };

	fscanf(fp, "%255[^ ] %d %d\n", name, &points, &edges);
	fscanf(fp, "%f %f %f\n", &origin[0], &origin[1], &origin[2]);

	Shape *new = create_shape(points, update_func, camera);
	new->name = malloc(strlen(name) - 1);
	strncpy(new->name, name, strlen(name));
	new->n_points = points;
	new->points = create_matrix(4,points, NULL);
	new->center = create_matrix(3, 1, origin);
	new->render = &render_shape;
	new->color = white;
	
	for (int i = 0; i < points; i++) {
		float xyz[4] = { 0, 0, 0, 1 };
		fscanf(fp, "%f %f %f\n", &xyz[0], &xyz[1], &xyz[2]);
		for (int j = 0; j < 4; j++) {
			set_entry(new->points, j, i, xyz[j]);
		}
	}

	for (int i = 0; i < edges; i++) {
		int nodes[2];
		fscanf(fp, "%d %d\n", &nodes[0], &nodes[1]);
		add_edge(new, nodes);
	}

	printf("%s\n",new->name);
	display_matrix(new->points);

	Edge *walk = new->edges;
	while (walk) {
		printf("edge between: %d - %d\n",walk->nodes[0], walk->nodes[1]);
		walk = walk->next;
	}
	
	fclose(fp);
	return new;
}

struct camera_struct *create_camera(float *location) {
	struct camera_struct *new = malloc(sizeof(struct camera_struct));


	// TODO change this to 0, 0, 0
	float rotation[3] = { 0, 0 , 100};

	memcpy(new->rotation, rotation, sizeof(float) * 3);
	new->position = create_matrix(3, 1, (location) ? location : rotation);

	return new;
}

int main(int argc, char *argv[]) {

		camera_inverse = create_matrix(1,3,camera_manuver);

    	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        	printf("Could not init SDL: %s\n", SDL_GetError());
        	exit(1);
    	}	

    	SDL_Window *window = SDL_CreateWindow(
        	NAME,
        	SDL_WINDOWPOS_CENTERED,
        	SDL_WINDOWPOS_CENTERED,
        	WIDTH,
        	HEIGHT,
		SDL_WINDOW_OPENGL
    	);

    	if (!window) {
        	printf("Failed to init window: %s\n", SDL_GetError());
        	exit(1);
    	}

	SDL_Renderer *render = SDL_CreateRenderer(window,-1,0);

	init();
	
	// main loop
    	bool should_quit = false;
    	while (!should_quit) {

		// stuff to do
		
		SDL_RenderClear(render);

		struct renderable_object *walk = collection;
		while (walk) {
			walk->object->render(render, walk->object, walk->camera);
			walk = walk->next;
		}
		draw(render);

		SDL_SetRenderDrawColor(render, bg.r, bg.g, bg.b, 255);
    	SDL_RenderPresent(render);

		// key shit
        	SDL_Event event;

        	while (SDL_PollEvent(&event)) {
			if (event.key.keysym.sym == 'q' || event.type == SDL_QUIT) {
				should_quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				char key = event.key.keysym.sym;
				key_press(key);
			} else {
				events(event);
			}
         	}
    	}
	SDL_Quit();
   	return 0;
}

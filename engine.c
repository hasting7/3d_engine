#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"

SDL_Color white = { .r = 255, .g = 255, .b = 255};

struct list_element *Shapes;


struct matrix_struct *rotational_transformation(const struct camera_struct camera) {
	float x_matrix[9] = {
		1,	0,								  0,
		0,	cos(camera.rotation->content[0]), sin(camera.rotation->content[0]),
		0, -sin(camera.rotation->content[0]), cos(camera.rotation->content[0]),
	};
	float y_matrix[9] = {
		cos(camera.rotation->content[1]), 0, -sin(camera.rotation->content[1]),
		0,								  1,  0,
		sin(camera.rotation->content[1]), 0,  cos(camera.rotation->content[1])
	};
	float z_matrix[9] = {
		cos(camera.rotation->content[2]), sin(camera.rotation->content[2]), 0,
	   -sin(camera.rotation->content[2]), cos(camera.rotation->content[2]), 0,
	   0,								  0,								1
	};
	Matrix *x_rotation = create_matrix(3, 3, x_matrix);
	Matrix *y_rotation = create_matrix(3, 3, y_matrix);
	Matrix *z_rotation = create_matrix(3, 3, z_matrix);

	Matrix *p1 = multiply_matrices(x_rotation, y_rotation);
	Matrix *p2 = multiply_matrices(p1, z_rotation);

	free_matrix(&x_rotation);
	free_matrix(&y_rotation);
	free_matrix(&z_rotation);
	free_matrix(&p1);

	return p2;
}

struct camera_struct *create_camera(float *position) {
	float zero[3] = { 0, 0, 0};
	Camera *new = malloc(sizeof(Camera));

	new->position = create_matrix(3, 1, (position) ? position : zero);
	new->rotation = create_matrix(3 ,1, zero);

	return new;
}

struct shape_struct *create_shape(int points) {
	struct list_element *container = malloc(sizeof(struct list_element));
	Shape *new = malloc(sizeof(Shape));
	new->points = create_matrix(3, points, NULL);
	new->color = white;

	container->data = (void *) new;
	container->next = Shapes;
	Shapes = container;

	return new;
}

void free_shape(struct shape_struct **shape) { // MAYBE SEARCH THROUGH SHAPES THEN DELETE THE LIST_ELEMENT
	Shape *temp = *shape;
	free_matrix(&temp->points);
	free(temp);
	*shape = NULL;
}


int main(int argc, char *argv[]) {

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

		struct list_element *walk = Shapes;
		while (walk) {
			Shape *current = (Shape *) walk->data;
			if (!current) continue;										// REMOVE IDK MAN

			current->render(current);

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

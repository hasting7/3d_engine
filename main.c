#include <stdbool.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"

int WIDTH =  600;
int HEIGHT = 600;
char *NAME = "Shapes";
SDL_Color bg;

Matrix *rotation;
Shape *plane;
Shape *cube;

float plane_trans[9] = {
	10, 0, 0,
	0. , 0.2, 0,
	0, 0, 10
};

void init() {
	Matrix *scale = create_matrix(3, 3, plane_trans);
	cube = populate_shape("./shapes/cube");

	transform_shape(cube, *scale);

	rotation = create_matrix(3,1, zero);

	set_entry(rotation, 0, 0, 0.01);
	// set_entry(rotation, 0, 0, 0.07);

	// free_matrix(&rotation);
}

void draw(SDL_Renderer *render) {

	inc_entry(main_camera->rotation, 1, 0, 0.01);

	display_matrix(main_camera->rotation);
	// rotate_shape(cube, *rotation);
	// rotate_shape(X, *rotation);
	// rotate_shape(Y, *rotation);
	// rotate_shape(Z, *rotation);

}

void key_press(char key) {
}

void events(SDL_Event e) {

}



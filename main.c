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
Shape *cube2;

float plane_trans[9] = {
	2, 0, 0,
	0. , 0.1, 0,
	0, 0, 16
};

void init() {
	cube = populate_shape("./shapes/cube", 0, 0, 0);
	cube2 = populate_shape("./shapes/cube", 0, -10, 0);
	cube2->color = orange;

	scale(cube, 10, 0.5, 10);
	scale(cube2, 2, 2, 2);

	rotation = create_matrix(3,1, zero);

	set_entry(rotation, 0, 0, 0.01);
	set_entry(main_camera->position, 1, 0, -100);

}

void draw(SDL_Renderer *render) {

	inc_entry(main_camera->rotation, 0, 0, 0.01);

	rotate_shape(cube2, *rotation);

}

void key_press(char key) {
}

void events(SDL_Event e) {

}



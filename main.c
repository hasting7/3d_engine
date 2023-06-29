#include <stdbool.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"

int WIDTH =  600;
int HEIGHT = 600;
char *NAME = "Shapes";
SDL_Color bg = { .r = 0, .g = 0, .b = 0};


float theta = 0.01;

Shape *cube;
Shape *pyramid;

Camera *main_cam;

Matrix *rotate;
Matrix *scale;

void update_shape(struct shape_struct *shape) {
	static int counter = 0;
	float factor = 1 + sin(counter / 100.0) / 300;
	float scale_data[16] = {
		factor, 0, 0, 0,
		0, factor, 0, 0,
		0, 0, factor, 0,
		0, 0, 0,	  1,
	};
	scale = create_matrix(4,4,scale_data);

	// transform(rotate, shape->points);
	// transform(scale, shape->points);
	free(scale);

	counter++;
}

void init() {

	main_cam = create_camera(NULL);


	cube = generate_object("cube", &update_shape, main_cam);
	SDL_Color red = {.r = 255};
	cube->color = red;

	// pyramid = generate_object("pyramid", &update_shape);


	float rotate_data[16] = {
		cos(theta), 0,   sin(theta), 0,
		0,	 		1, 0,   		 0,
		-sin(theta),0,	 cos(theta), 0,
		0,	 		0,	 0,   		 1
	};

	rotate = create_matrix(4,4, rotate_data);

}

void draw(SDL_Renderer *render) {
	
	display_matrix(main_cam->position);
	
}

void key_press(char key) {
	int step = 5;
	if (key == 'w') {
		inc_entry(main_cam->position, 2, 0, -step);
	} else if (key == 's') {
		inc_entry(main_cam->position, 2, 0, step);
	} else if (key == 'a') {
		inc_entry(main_cam->position, 0, 0, -step);
	} else if (key == 'd') {
		inc_entry(main_cam->position, 0, 0, step);
	}
}

void events(SDL_Event e) {

}



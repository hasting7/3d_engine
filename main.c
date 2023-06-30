#include <stdbool.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"

int WIDTH =  600;
int HEIGHT = 600;
char *NAME = "Shapes";
SDL_Color bg;

Camera *main_cam;

void init() {
	main_cam = create_camera(NULL);

}

void draw(SDL_Renderer *render) {
	Matrix *rotation = rotational_transformation(*main_cam);

	display_matrix(rotation);

	free_matrix(&rotation);
}

void key_press(char key) {
}

void events(SDL_Event e) {

}



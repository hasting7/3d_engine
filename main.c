#include <stdbool.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"

int WIDTH =  600;
int HEIGHT = 600;
char *NAME = "Shapes";
SDL_Color bg;

Matrix *rotation;

void init() {

	rotation = create_matrix(3, 1, zero);
	set_entry(rotation, 1, 0, 0.01);
}

void draw(SDL_Renderer *render) {



	rotate_shape(X, *rotation);
	rotate_shape(Y, *rotation);	
	rotate_shape(Z, *rotation);	

}

void key_press(char key) {
}

void events(SDL_Event e) {

}



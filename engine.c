#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"

SDL_Color white = { .r = 255, .g = 255, .b = 255};
float zero[3] = { 0, 0, 0};
float invert_data[9] = {
	-1, 0, 0,
	0, -1, 0,
	0, 0, -1
};

Camera *main_camera;
Matrix *invert;
Shape *X;
Shape *Y;
Shape *Z;

struct list_element *Shapes;

void update_camera(struct camera_struct *camera) {
	for (int j = 0; j < 3; j++) {
		float *target = target_entry(*camera->rotation, j, 0);
		if (*target > M_PI * 2) *target -= M_PI * 2;
	}
}

struct matrix_struct *rotational_transformation(struct matrix_struct rotation) {
	assert(rotation.n == 1);

	float x_matrix[9] = {
		1,	0,						  0,
		0,	cos(rotation.content[0]), sin(rotation.content[0]),
		0, -sin(rotation.content[0]), cos(rotation.content[0]),
	};
	float y_matrix[9] = {
		cos(rotation.content[1]), 0, -sin(rotation.content[1]),
		0,						  1,  0,
		sin(rotation.content[1]), 0,  cos(rotation.content[1])
	};
	float z_matrix[9] = {
		cos(rotation.content[2]), sin(rotation.content[2]), 0,
	   -sin(rotation.content[2]), cos(rotation.content[2]), 0,
	    0,						  0,				        1
	};

	Matrix *x_rotation = create_matrix(3, 3, x_matrix);
	Matrix *y_rotation = create_matrix(3, 3, y_matrix);
	Matrix *z_rotation = create_matrix(3, 3, z_matrix);

	Matrix *p1 = multiply_matrices(*x_rotation, *y_rotation);
	Matrix *p2 = multiply_matrices(*p1, *z_rotation);

	free_matrix(&x_rotation);
	free_matrix(&y_rotation);
	free_matrix(&z_rotation);
	free_matrix(&p1);

	return p2;
}

struct camera_struct *create_camera(float *position, void *(update_func)(struct camera_struct *)) {
	float zero[3] = { 0, 0, 0};
	float pos[3] = {0, -30, 500};
	float dir[3] = {0, 0, -100}; // change to 1;
	Camera *new = malloc(sizeof(Camera));

	new->position = create_matrix(3, 1, (position) ? position : pos);
	new->rotation = create_matrix(3 ,1, zero);
	new->direction = create_matrix(3, 1, dir);
	new->update = update_func;

	return new;
}

void transform_shape(struct shape_struct *shape, struct matrix_struct transformation) {
	Matrix *product = multiply_matrices(transformation, *shape->points);

	free_matrix(&shape->points);

	shape->points = product;
}

void rotate_shape(struct shape_struct *shape, struct matrix_struct rotation) {
	Matrix *rotational_matrix = rotational_transformation(rotation);

	transform_shape(shape, *rotational_matrix);

	free_matrix(&rotational_matrix);
}

struct shape_struct *create_shape(int points, float *point_data) {
	struct list_element *container = malloc(sizeof(struct list_element));
	Shape *new = malloc(sizeof(Shape));
	new->points = create_matrix(3, points, point_data);
	new->render = &render_shape;
	new->color = white;
	new->n_edges = 0;

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

SDL_Point *project_shape(struct shape_struct shape, struct camera_struct camera) {
	// e represents location of 2d display surface relative to camera location
	// for now imma just have it {0, 0, 100} so its in front off 
	float e_pos[3] = {0, 0, -500}; // MAYBE CHANGE THIS TO -100

	float e_data[9] = {
		1, 0, e_pos[0] / e_pos[2],
		0, 1, e_pos[1] / e_pos[2],
		0, 0, 1 / e_pos[2]
	};

	int exclude[10] = { 0 };

	float dot_product;

	Matrix *from_cam = offset(*shape.points, *camera.position);

	for (int i = 0; i < shape.points->n; i++) {
		dot_product = 0;
		for (int j = 0; j < 3; j++) {
			dot_product += *target_entry(*camera.rotation, j, 0) * *target_entry(*from_cam, j, i);
		}
		printf("P%d dot product: %f\n", i, dot_product);
		if (dot_product < 0) {
			exclude[i] = 1;
			printf("EXCLUDING\n");
		}
	}

	free_matrix(&from_cam);

	// where result is saved
	SDL_Point *result = calloc(shape.points->n, sizeof(SDL_Point));

	// shift shape matrix with cameras offset, and get the rotational data
	Matrix *shape_camera_offset = offset(*shape.points, *camera.position);
	Matrix *camera_rotation = rotational_transformation(*camera.rotation);

	// transform the matrices
	Matrix *d = multiply_matrices(*camera_rotation, *shape_camera_offset);

	
	Matrix *e_homo = create_matrix(3, 3, e_data);

	Matrix *f = multiply_matrices(*e_homo, *d);

	for (int i = 0; i < f->n; i++) {
		if (exclude[i] == 1) {
			result[i].x = 0;
			result[i].y = 0;
			continue;
		}
		float scale = *target_entry(*f, 2, i);
		result[i].x = *target_entry(*f, 0 ,i) / scale + (WIDTH / 2);
		result[i].y = *target_entry(*f, 1 ,i) / scale + (HEIGHT / 2);
	}

	free_matrix(&shape_camera_offset);
	free_matrix(&camera_rotation);
	free_matrix(&e_homo);
	free_matrix(&f);
	return result;
}

void render_shape(SDL_Renderer *render, struct shape_struct *shape, struct camera_struct camera) {
	SDL_Point *points = project_shape(*shape, camera);

	for (int i = 0; i < shape->points->n; i++) {
		SDL_RenderDrawPoint(render, points[i].x, points[i].y);
	}

	struct list_element *walk = shape->edges;

	while (walk) {
		Edge *edge = (Edge *) walk->data;

		SDL_RenderDrawLine(render, 
				points[edge->indices[0]].x, 
				points[edge->indices[0]].y, 
				points[edge->indices[1]].x, 
				points[edge->indices[1]].y);

		walk = walk->next;
	}

	free(points);
}

struct shape_struct *populate_shape(char *filename) {
	FILE *fp = fopen(filename, "r");
	int r, g, b, points, edges;
	float xyz[3] = { 0 };

	fscanf(fp,"(%d,%d,%d) %d %d\n", &r, &g, &b, &points, &edges);
	SDL_Color color = {.r = r, .g = g, .b = b};

	Shape *new = create_shape(points, NULL);
	new->color = color;

	for (int i = 0; i < points; i++) {
		fscanf(fp, "%f %f %f\n", &xyz[0], &xyz[1], &xyz[2]);
		for (int j = 0; j < 3; j++) {
			set_entry(new->points, j, i, xyz[j]);
		}
	}

	int indices[2] = { 0 };
	for (int i = 0; i < edges; i++) {
		fscanf(fp, "%d %d\n", &indices[0], &indices[1]);

		struct list_element *container = malloc(sizeof(struct list_element));
		Edge *edge = malloc(sizeof(Edge));
		memcpy(edge->indices, indices, sizeof(int) * 2);

		container->next = new->edges;
		new->edges = container;
		container->data = (void *) edge;
	}

	printf("loaded shape\n");
	display_matrix(new->points);

	fclose(fp);
	return new;
}


int main(int argc, char *argv[]) {

		main_camera = create_camera(NULL, &update_camera);

		invert = create_matrix(3, 3, invert_data);


		// X = populate_shape("./axis/x-axis");
		// Y = populate_shape("./axis/y-axis");
		// Z = populate_shape("./axis/z-axis");



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

		main_camera->update(main_camera);

		struct list_element *walk = Shapes;
		while (walk) {
			Shape *current = (Shape *) walk->data;
			if (!current) continue;										// REMOVE IDK MAN

			SDL_SetRenderDrawColor(render, current->color.r, current->color.g, current->color.b, 255);
			current->render(render, current, *main_camera);

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

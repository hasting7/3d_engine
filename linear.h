#ifndef LINEAR
#define LINEAR

typedef struct matrix_struct {
	int m;
	int n;
	float *content;
} Matrix;

float *target_entry(struct matrix_struct, int, int);
void set_entry(struct matrix_struct *, int, int, float);
void set_matrix(struct matrix_struct *, float *);
void display_matrix(struct matrix_struct *);
struct matrix_struct *create_matrix(int, int, float *);
struct matrix_struct *multiply_matrices(struct matrix_struct, struct matrix_struct);
struct matrix_struct *offset(const struct matrix_struct, const struct matrix_struct);
void inc_entry(struct matrix_struct *, int, int, float);
struct matrix_struct *add_matricies(const struct matrix_struct, const struct matrix_struct);
void free_matrix(struct matrix_struct **);
void transform(struct matrix_struct, struct matrix_struct *);
float mag(struct matrix_struct, int);



#endif
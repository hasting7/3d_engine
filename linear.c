#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "linear.h"


float *target_entry(struct matrix_struct matrix, int m, int n) {
	assert(m < matrix.m);
	assert(m >= 0);
	assert(n < matrix.n);
	assert(n >= 0);
	return matrix.content + (m * matrix.n + n);
}

void set_entry(struct matrix_struct *matrix, int m, int n, float value) {
	float *entry = target_entry(*matrix, m, n);
	*entry = value;
}

void inc_entry(struct matrix_struct *matrix, int m, int n, float value) {
	float *entry = target_entry(*matrix, m, n);
	*entry += value;
}

void set_matrix(struct matrix_struct *matrix, float *entries) {
	for (int m = 0; m < matrix->m; m++) {
		for (int n = 0; n < matrix->n; n++) {
			set_entry(matrix, m, n, *(entries++));
		}
	}
}

void display_matrix(struct matrix_struct *matrix) {
	assert(matrix);
	printf("---%dx%d-Matrix---\n", matrix->m, matrix->n);
	for (int i = 0; i < matrix->m * matrix->n; i++) {
		if ((i % matrix->n == 0) && (i != 0)) printf("\n");
		printf("%6.2f ", *(matrix->content + i));
	}
	printf("\n");
}

struct matrix_struct *create_matrix(int m, int n, float *content) {
	Matrix *new = malloc(sizeof(Matrix));
	new->m = m;
	new->n = n;
	new->content = calloc(m*n,sizeof(float));

	if (content) set_matrix(new, content);

	return new;
}

struct matrix_struct *multiply_matrices(struct matrix_struct m1, struct matrix_struct m2) {
	assert(m1.n == m2.m);
	Matrix *product = create_matrix(m1.m, m2.n, NULL);

	for (int m = 0; m < product->m; m++) {

		for (int n = 0; n < product->n; n++) {
			float *product_entry = target_entry(*product, m, n);

			for (int i = 0; i < m1.n; i++) {
				*product_entry += *target_entry(m1, m, i) * *target_entry(m2, i, n);
			}
		}
	}
	return product;
}

float mag(struct matrix_struct matrix, int n) {
	float sum = 0;

	for (int i = 0; i < matrix.m; i++) {
		sum += pow(*target_entry(matrix, i, n), 2);
	}
	return sqrt(sum);
}

struct matrix_struct *add_matricies(const struct matrix_struct m1, const struct matrix_struct m2) {
	assert(m1.m == m2.m);
	assert(m1.n == m2.n);
	Matrix *sum = create_matrix(m1.m, m1.n, NULL); 

	for (int m = 0; m < m1.m; m++) {
		for (int n = 0; n < m1.n; n++) {
			float *target = target_entry(*sum, m, n);
			*target = *target_entry(m1, m, n) + *target_entry(m2, m, n);
		}
	}

	return sum;
}

struct matrix_struct *offset(const struct matrix_struct target, const struct matrix_struct offset) {
	assert(offset.n == 1);
	assert(target.m >= offset.m);

	Matrix *new = create_matrix(target.m, target.n, target.content);

	for (int i = 0; i < offset.m; i++) {
		for (int j = 0; j < target.n; j++) {
			float *target_num = target_entry(*new, i, j);
			*target_num -= *target_entry(offset, i, 0);
		}
	}

	return new;
}

void free_matrix(struct matrix_struct **matrix) {
	Matrix *temp = *matrix;
	free(temp->content);
	free(temp);
	*matrix = NULL;
}

#ifdef TEST
int main() {
	float a[4] = { 1, 2, 3, 4};
	float b[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	Matrix *m_a = create_matrix(2,2, a);
	Matrix *m_b = create_matrix(2,8, b);

	//Matrix *product = multiply_matrices(m_a, m_b);

	display_matrix(m_b);
	transform(m_a,m_b);
	display_matrix(m_b);

	// display_matrix(product);

	return 0;
}
#endif
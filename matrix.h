#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct v3 {
  float x, y, z, w;
};

typedef float matrix[4][4];

void apply_mm(matrix m1, matrix m2, matrix mm);
void apply_mp(matrix m, struct v3 *p, struct v3 *mp);
void apply_w(struct v3 *p);

void rot_x(float rot, matrix m);
void rot_y(float rot, matrix m);
void rot_z(float rot, matrix m);

void id(matrix m);
void rotate(struct v3 rot, matrix m);
void translate(struct v3 trans, matrix m);
void scale(struct v3 scale, matrix m);
void perspective(float u, float v, matrix m);

float determinant(matrix m);

#ifdef DEBUG
void printm(matrix m);
void printv3(struct v3 p);

void matrix_test();
#endif

#endif

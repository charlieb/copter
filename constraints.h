#ifndef CONSTRAINTS_H
#define CONSTRAINTS_H

#include "matrix.h"

enum constraint_type { GT_X, GT_Y, GT_Z, LT_X, LT_Y, LT_Z, PT_DIST }; 

struct constraint {
	enum constraint_type type;
	struct point *p;
	/* not all of these elements will be used
	 * for every constraint type */
	struct point *p2;
	float value;
	struct v3 pos;
};
struct point {
	struct v3 pos, vel, acc;
	float mass;
};
#ifdef DEBUG
void print_point(struct point *p);
#endif

void vadd(struct v3 *v1, struct v3 *v2);
void vsub(struct v3 *v1, struct v3 *v2);
void vmul(struct v3 *v, float m);
float vmag(struct v3 *v);
void vnorm(struct v3 *v);
void iterate_point(struct point *p);
				
void constraint_gt_x(struct constraint *c);
void constraint_gt_y(struct constraint *c);
void constraint_gt_z(struct constraint *c);
void constraint_lt_x(struct constraint *c);
void constraint_lt_y(struct constraint *c);
void constraint_lt_z(struct constraint *c);
void constraint_pt_dist(struct constraint *c);
void apply_constraints(struct constraint *constraints, int nconstraints);
void apply_force(struct point *p, struct v3 *f);

#endif

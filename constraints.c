#include "constraints.h"

void print_point(struct point *p)
{
	printf("Pos: "); printv3(p->pos);
	printf("Vel: "); printv3(p->vel);
	printf("Acc: "); printv3(p->acc);
	printf("-------------\n");
}

void vadd(struct v3 *v1, struct v3 *v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
	v1->z += v2->z;
	/* v1->x += v2->x; */
}
void vsub(struct v3 *v1, struct v3 *v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
	v1->z -= v2->z;
	/* v1->x -= v2->x; */
}
void vmul(struct v3 *v, float m)
{
	v->x *= m;
	v->y *= m;
	v->z *= m;
	/* v->x *= m; */
}
float vmag(struct v3 *v)
{
	return sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}
void vnorm(struct v3 *v)
{
	vmul(v, 1.0/vmag(v));
}
void iterate_point(struct point *p)
{
	vadd(&p->vel, &p->acc);	
	vadd(&p->pos, &p->vel);	
}
				
void constraint_gt_x(struct constraint *c) { if(c->p->pos.x < c->value) { c->p->pos.x = c->value; } }
void constraint_gt_y(struct constraint *c) { if(c->p->pos.y < c->value) { c->p->pos.y = c->value; } }
void constraint_gt_z(struct constraint *c) { if(c->p->pos.z < c->value) { c->p->pos.z = c->value; } }
void constraint_lt_x(struct constraint *c) { if(c->p->pos.x > c->value) { c->p->pos.x = c->value; } }
void constraint_lt_y(struct constraint *c) { if(c->p->pos.y > c->value) { c->p->pos.y = c->value; } }
void constraint_lt_z(struct constraint *c) { if(c->p->pos.z > c->value) { c->p->pos.z = c->value; } }
void constraint_pt_dist(struct constraint *c) { 
	struct v3 diff;
	float d = 0;
	
	memcpy(&diff, &c->p->pos, sizeof(struct v3));
	vsub(&diff, &c->p2->pos);
	d = vmag(&diff);
	/*          | size of change    | normalize | */
	vmul(&diff, (d - c->value) / (2 * d));
	vsub(&c->p->pos, &diff);
	vadd(&c->p2->pos, &diff);
}
void apply_constraints(struct constraint *constraints, int nconstraints)
{
	int i;
	for(i = 0; i < nconstraints; ++i) {
		switch (constraints[i].type) {
			case GT_X:
				constraint_gt_x(&constraints[i]);
				break;
			case GT_Y:
				constraint_gt_y(&constraints[i]);
				break;
			case GT_Z:
				constraint_gt_z(&constraints[i]);
				break;
			case LT_X:
				constraint_lt_x(&constraints[i]);
				break;

			case LT_Y:
				constraint_lt_y(&constraints[i]);
				break;
			case LT_Z:
				constraint_lt_z(&constraints[i]);
				break;
			case PT_DIST:
				constraint_pt_dist(&constraints[i]);
				break;
			default:
				printf("Invalid constraint type in apply_constraints\n");
				exit(1);
		}
	}
}

void apply_force(struct point *p, struct v3 *f)
{
	/* F = ma => a = F/m */
	struct v3 tmp;
	memcpy(&tmp, f, sizeof(struct v3));
	vmul(&tmp, 1/p->mass);
	vadd(&p->acc, &tmp);
}



#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "constraints.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL.h"

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16


void quit(int returnCode)
{
  SDL_Quit();
  exit(returnCode);
}

int resizeWindow(int width, int height)
{
  GLfloat ratio;
 
  if (height == 0)
    height = 1;

  ratio = (GLfloat)width / (GLfloat)height;

  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  return 0;
}

int initGL()
{
  GLfloat amb[] = { 0.25f, 0.25f, 0.25f, 1.0f };
  GLfloat spot[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat spot_pos[] = { 0.5f, 0.5f, 0.5f, 1.0f };

  /*glEnable(GL_TEXTURE_2D);*/
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
  glClearDepth(1.0f);
  /*
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  */

  glEnable(GL_BLEND);	
  glDisable(GL_DEPTH_TEST);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, spot);
  glLightfv(GL_LIGHT0, GL_POSITION, spot_pos);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
	
  return 0;
}

/********************/
int drawGLScene(struct point motors[4])
{
  /* These are to calculate our fps */
  /*static GLfloat xrot = 0, yrot = 0, zrot = 0;*/
	GLUquadricObj* quadrics[4];
	/* Clear The Screen And The Depth Buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -30.0f);
  
	glTranslatef(motors[0].pos.x, motors[0].pos.y, motors[0].pos.z);
	quadrics[0] = gluNewQuadric();
	gluSphere(quadrics[0], 1.0, 10, 10);
	/**/
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -30.0f);

	glTranslatef(motors[1].pos.x, motors[1].pos.y, motors[1].pos.z);
	quadrics[1] = gluNewQuadric();
	gluSphere(quadrics[1], 1.0, 10, 10);
	/**/
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -30.0f);

	glTranslatef(motors[2].pos.x, motors[2].pos.y, motors[2].pos.z);
	quadrics[2] = gluNewQuadric();
	gluSphere(quadrics[2], 1.0, 10, 10);
	/**/
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -30.0f);

	glTranslatef(motors[3].pos.x, motors[3].pos.y, motors[2].pos.z);
	quadrics[3] = gluNewQuadric();
	gluSphere(quadrics[3], 1.0, 10, 10);

  /* Draw it to the screen */
  SDL_GL_SwapBuffers();

  /* Gather our frames per second */
  /*Frames++;
    {
    GLint t = SDL_GetTicks();
    if (t - T0 >= 5000) {
    GLfloat seconds = (t - T0) / 1000.0;
    GLfloat fps = Frames / seconds;
    printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
    T0 = t;
    Frames = 0;
    }
    }
  */

  return 0;
}

void handleKeyPress(SDL_keysym *keysym, SDL_Surface *surface)
{
  switch ( keysym->sym ) {
  case SDLK_ESCAPE:
    /* ESC key was pressed */
    quit(0);
    break;
  case SDLK_F1:
    /* F1 key was pressed
     * this toggles fullscreen mode
     */
    SDL_WM_ToggleFullScreen(surface);
    break;
  default:
    break;
  }
  return;
}


int startGL(struct point motors[4])
{
	SDL_Surface *surface = NULL;
	int videoFlags;
	int done = 0;
	SDL_Event event;
	const SDL_VideoInfo *videoInfo;
	int isActive = 1;

	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)	{
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		quit(1);
	}

	videoInfo = SDL_GetVideoInfo();

	if (!videoInfo)	{
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		quit(1);
	}

	videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

	/* This checks to see if surfaces can be stored in memory */
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	/* This checks if hardware blits can be done */
	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			videoFlags);

	/* Verify there is a surface */
	if (!surface) {
		fprintf(stderr,  "Video mode set failed: %s\n", SDL_GetError());
		quit(1);
	}

	initGL();
	/*cube_list = load_mesh();*/
	resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);


	while (!done)	{
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_ACTIVEEVENT:
					/* Something's happend with our focus
					 * If we lost focus or we are iconified, we
					 * shouldn't draw the screen
					 */
					if (event.active.gain == 0)
						isActive = 0;
					else
						isActive = 1;
					break;			    
				case SDL_VIDEORESIZE:
					surface = SDL_SetVideoMode(event.resize.w,
							event.resize.h,
							16, videoFlags);
					if (!surface)	{
						fprintf(stderr, "Could not get a surface after resize: %s\n", SDL_GetError());
						quit(1);
					}
					resizeWindow(event.resize.w, event.resize.h);
					break;
				case SDL_KEYDOWN:
					handleKeyPress(&event.key.keysym, surface);
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}

		if (isActive) {
			drawGLScene(motors);
			/*iterate_grid(0);*/
		}
	}

	quit(0);

	/* Should never get here */
	return 0;
}

void test_constraints()
{
	struct point p1, p2;
	struct v3 g; /* gravity */
	struct constraint constraints[7];
	int i = 0;
	memset(constraints, 0, 7 * sizeof(struct constraint));
	memset(&p1, 0, sizeof(struct point));
	memset(&p2, 0, sizeof(struct point));
	p1.mass = 100;

	for(i = 0; i < 7; ++i) {
		constraints[i].p = &p1;
		if(i < 3)
			constraints[i].value = 0;
		else
			constraints[i].value = 10;
	}
	constraints[6].p2 = &p2;

	constraints[0].type = GT_X;
	constraints[1].type = GT_Y;
	constraints[2].type = GT_Z;
	constraints[3].type = LT_X;
	constraints[4].type = LT_Y;
	constraints[5].type = LT_Z;
	constraints[6].type = PT_DIST;

	p1.pos.x = p1.pos.y = p1.pos.z = 11;
	p2.pos.x = p2.pos.y = p2.pos.z = -1;
	
	print_point(&p1);
	print_point(&p2);
	printf("================\n");

	/* all but the last one */
	apply_constraints(constraints, 6);

	print_point(&p1);
	print_point(&p2);
	printf("================\n");
	
	apply_constraints(constraints, 7);

	print_point(&p1);
	print_point(&p2);
	printf("================\n");

	g.x = g.z = 0;
	g.y = -9.8;
	for(i = 0; i < 20; i++) {
		memset(&p1.acc, 0, sizeof(struct v3));
		apply_force(&p1, &g);
		iterate_point(&p1);
		apply_constraints(constraints, 7);
		print_point(&p1);
		print_point(&p2);
		printf("================\n");
	}
}

void make_copter(struct point points[4], struct constraint constraints[6])
{
	/*
	 *  p---c---p
	 *  |\     /|
	 *  | \   c |
	 *  |  \ /  |
	 *  c   x   c
	 *  |  /\   |
	 *  | /  c  |
	 *  |/    \ |
	 *  p---c---p
	 *
	 *  4 points (p) connected by 6 constraints (c)
	 */

	memset(points, 0, 4 * sizeof(struct point));
	memset(constraints, 0, 6 * sizeof(struct constraint));

	points[0].pos.x = -1;
	points[0].pos.y = -1;

	points[1].pos.x = -1;
	points[1].pos.y = 1;

	points[2].pos.x = 1;
	points[2].pos.y = -1;

	points[3].pos.x = 1;
	points[3].pos.y = 1;

	constraints[0].type= PT_DIST;
	constraints[0].p = &points[0];
	constraints[0].p2 = &points[1];
	constraints[0].value = 2;

	constraints[1].type= PT_DIST;
	constraints[1].p = &points[1];
	constraints[1].p2 = &points[2];
	constraints[1].value = 2;

	constraints[2].type= PT_DIST;
	constraints[2].p = &points[2];
	constraints[2].p2 = &points[3];
	constraints[2].value = 2;

	constraints[3].type= PT_DIST;
	constraints[3].p = &points[3];
	constraints[3].p2 = &points[0];
	constraints[3].value = 2;

	constraints[4].type= PT_DIST;
	constraints[4].p = &points[0];
	constraints[4].p2 = &points[2];
	constraints[4].value = sqrt(2*2 + 2*2);

	constraints[5].type= PT_DIST;
	constraints[5].p = &points[1];
	constraints[5].p2 = &points[3];
	constraints[5].value = sqrt(2*2 + 2*2);
}

void test()
{
	struct point ps[4];
	memset(ps, 0, 4 * sizeof(struct point));

	ps[0].pos.x = -5;
	ps[0].pos.y = -5;

	ps[1].pos.x = -5;
	ps[1].pos.y = 5;

	ps[2].pos.x = 5;
	ps[2].pos.y = -5;

	ps[3].pos.x = 5;
	ps[3].pos.y = 5;

	startGL(ps);
}

int main(int argc, char **argv)
{
	test();
	return 0;
}

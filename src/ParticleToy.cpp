// ParticleToy.cpp : Defines the entry point for the console application.
//

#include "Particle.h"
#include "Force.h"
#include "Gravity.h"
#include "Scene.h"
#include "SpringForce.h"
#include "RodConstraint.h"
#include "CircularWireConstraint.h"
#include "imageio.h"

#include <vector>
#include <stdlib.h>
#include <stdio.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* macros */

/* external definitions (from solver) */
extern void simulation_step(std::vector<Particle *> pVector, std::vector<Force *> forces, std::vector<Force *> constraints, float dt, int solver);

/* global variables */

static int N;
static float dt, d;
static int dsim;
static int dump_frames;
static int frame_number;

// static Particle *pList;
static std::vector<Particle *> pVector;

static int win_id;
static int win_x, win_y;
static int mouse_down[3];
static int mouse_release[3];
static int mouse_shiftclick[3];
static int omx, omy, mx, my;
static int hmx, hmy;

// Vector of generalized forces
static std::vector<Force *> forces;
static std::vector<Force *> constraints;

static SpringForce *delete_this_dummy_spring = NULL;
static RodConstraint *delete_this_dummy_rod = NULL;
static CircularWireConstraint *delete_this_dummy_wire = NULL;

// Init GLUT menu for solver
static int solver = 0;
enum MENU_TYPE
{
	MENU_EULER,
	MENU_MIDPOINT,
	MENU_RUNGE_KUTTA,
};
MENU_TYPE show = MENU_EULER;

int dragParticle;

/*
----------------------------------------------------------------------
free/clear/allocate simulation data
----------------------------------------------------------------------
*/

static void free_data(void)
{
	pVector.clear();
	if (delete_this_dummy_rod)
	{
		delete delete_this_dummy_rod;
		delete_this_dummy_rod = NULL;
	}
	if (delete_this_dummy_spring)
	{
		delete delete_this_dummy_spring;
		delete_this_dummy_spring = NULL;
	}
	if (delete_this_dummy_wire)
	{
		delete delete_this_dummy_wire;
		delete_this_dummy_wire = NULL;
	}
	forces.clear();
	constraints.clear();
}

static void clear_data(void)
{
	int ii, size = pVector.size();

	for (ii = 0; ii < size; ii++)
	{
		pVector[ii]->reset();
	}
}

/*
----------------------------------------------------------------------
OpenGL specific drawing routines
----------------------------------------------------------------------
*/

static void pre_display(void)
{
	glViewport(0, 0, win_x, win_y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

static void post_display(void)
{
	// Write frames if necessary.
	if (dump_frames)
	{
		const int FRAME_INTERVAL = 4;
		if ((frame_number % FRAME_INTERVAL) == 0)
		{
			const unsigned int w = glutGet(GLUT_WINDOW_WIDTH);
			const unsigned int h = glutGet(GLUT_WINDOW_HEIGHT);
			unsigned char *buffer = (unsigned char *)malloc(w * h * 4 * sizeof(unsigned char));
			if (!buffer)
				exit(-1);
			// glRasterPos2i(0, 0);
			glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			static char filename[80];
			sprintf(filename, "../snapshots/img%.5i.png", frame_number / FRAME_INTERVAL);
			printf("Dumped %s.\n", filename);
			saveImageRGBA(filename, buffer, w, h);

			free(buffer);
		}
	}
	frame_number++;

	glutSwapBuffers();
}

static void draw_particles(void)
{
	int size = pVector.size();

	for (int ii = 0; ii < size; ii++)
	{
		pVector[ii]->draw();
	}
}

static void draw_forces(void)
{
	for (auto force : forces)
	{
		force->draw();
	}
}

static void draw_constraints(void)
{
	for (auto constraint : constraints)
	{
		constraint->draw();
	}
}

/*
----------------------------------------------------------------------
relates mouse movements to particle toy construction
----------------------------------------------------------------------
*/

static void get_from_UI()
{
	int i, j;
	// int size, flag;
	int hi, hj;
	// float x, y;
	if (!mouse_down[0] && !mouse_down[2] && !mouse_release[0] && !mouse_shiftclick[0] && !mouse_shiftclick[2])
		return;

	i = (int)((mx / (float)win_x) * N);
	j = (int)(((win_y - my) / (float)win_y) * N);

	if (i < 1 || i > N || j < 1 || j > N)
		return;

	if (mouse_down[0]) // Left mouse button pressed
	{
		float px = (float)(i - 32) / 32;
		float py = (float)(j - 32) / 32;

		int ii = 0, size = pVector.size();

		if (dragParticle == size)
		{
			for (ii = 0; ii < size; ii++)
			{
				double x = pVector[ii]->m_Position[0], y = pVector[ii]->m_Position[1];
				if (px > x - 0.1 && px < x + 0.1 && py > y - 0.1 && py < y + 0.1)
				{
					dragParticle = ii;
					break;
				}
			}
		}

		if (dragParticle < size)
		{
			Vec2f mouse(px, py);
			pVector[dragParticle]->m_Position = mouse;
		}
	}

	if (mouse_down[2]) // Right mouse button pressed
	{
	}

	hi = (int)((hmx / (float)win_x) * N);
	hj = (int)(((win_y - hmy) / (float)win_y) * N);

	if (mouse_release[0]) // Left mouse button released (is indefinitely active after first press)
	{
		dragParticle = pVector.size();
	}

	omx = mx;
	omy = my;
}

static void remap_GUI()
{
	int ii, size = pVector.size();
	for (ii = 0; ii < size; ii++)
	{
		pVector[ii]->m_Position[0] = pVector[ii]->m_ConstructPos[0];
		pVector[ii]->m_Position[1] = pVector[ii]->m_ConstructPos[1];
	}
}

/*
----------------------------------------------------------------------
GLUT callback routines
----------------------------------------------------------------------
*/

static void key_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
	case 'C':
		clear_data();
		break;

	case 'd':
	case 'D':
		dump_frames = !dump_frames;
		break;

	case 'q':
	case 'Q':
		free_data();
		exit(0);
		break;

	case ' ':
		dsim = !dsim;
		// TODO maybe remove this, but it's annoying for now
		clear_data();
		break;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		free_data();
		Scene::setup(pVector, forces, constraints, dragParticle, key - '0');

		break;
	}
}

static void mouse_func(int button, int state, int x, int y)
{
	omx = mx = x;
	omx = my = y;

	if (!mouse_down[0])
	{
		hmx = x;
		hmy = y;
	}
	if (mouse_down[button])
	{
		mouse_release[button] = state == GLUT_UP;
	}
	if (mouse_down[button])
		mouse_shiftclick[button] = glutGetModifiers() == GLUT_ACTIVE_SHIFT;
	mouse_down[button] = state == GLUT_DOWN;
}

static void motion_func(int x, int y)
{
	mx = x;
	my = y;
}

static void reshape_func(int width, int height)
{
	glutSetWindow(win_id);
	glutReshapeWindow(width, height);

	win_x = width;
	win_y = height;
}

static void idle_func(void)
{
	get_from_UI();

	if (dsim)
		simulation_step(pVector, forces, constraints, dt, solver);
	else
	{
		remap_GUI();
	}

	glutSetWindow(win_id);
	glutPostRedisplay();
}

static void display_func(void)
{
	pre_display();

	draw_forces();
	draw_constraints();
	draw_particles();

	post_display();
}

/*
----------------------------------------------------------------------
open_glut_window --- open a glut compatible window and set callbacks
----------------------------------------------------------------------
*/

void menu(int item)
{
	solver = item;

	switch (item)
	{
	case MENU_EULER:
	case MENU_MIDPOINT:
	case MENU_RUNGE_KUTTA:
	{
		show = (MENU_TYPE)item;
	}
	break;
	}

	glutPostRedisplay();

	return;
}

static void open_glut_window(void)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(win_x, win_y);
	win_id = glutCreateWindow("Particletoys!");

	glutCreateMenu(menu);
	// Add menu items
	glutAddMenuEntry("Euler", MENU_EULER);
	glutAddMenuEntry("Midpoint", MENU_MIDPOINT);
	glutAddMenuEntry("RungeKutta", MENU_RUNGE_KUTTA);

	// Associate a mouse button with menu
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	pre_display();

	glutKeyboardFunc(key_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutReshapeFunc(reshape_func);
	glutIdleFunc(idle_func);
	glutDisplayFunc(display_func);
}

/*
----------------------------------------------------------------------
main --- main routine
----------------------------------------------------------------------
*/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	if (argc == 1)
	{
		N = 64;
		dt = 0.5f;
		d = 5.f;
		fprintf(stderr, "Using defaults : N=%d dt=%g d=%g\n",
				N, dt, d);
	}
	else
	{
		N = atoi(argv[1]);
		dt = atof(argv[2]);
		d = atof(argv[3]);
	}

	printf("\n\nHow to use this application:\n\n");
	printf("\t Toggle construction/simulation display with the spacebar key\n");
	printf("\t Change scenes using the number keys\n");
	printf("\t Dump frames by pressing the 'd' key\n");
	printf("\t Quit by pressing the 'q' key\n");

	dsim = 0;
	dump_frames = 0;
	frame_number = 0;

	Scene::setup(pVector, forces, constraints, dragParticle, 1);

	win_x = 512;
	win_y = 512;

	open_glut_window();

	glutMainLoop();

	exit(0);
}

#include <iostream>
#include "GLUT/glut.h"
#include "render.h"

// define width & height of openGL window
#define WIDTH 1000
#define HEIGHT 1000

// Instantiate the render class
Render theApp;

// Define the display function
void myDisplay()
{
	theApp.display();
}

// Define the reshape function
void myReshape(int w, int h)
{
	theApp.reshape(w, h);
}

// Define the mouse click events
void myMouseClick(int button, int state, int x, int y)
{
	theApp.mouseClick(button, state, x, y);
}

// Define the mouse drag events
void myMouseMove(int x, int y)
{
	theApp.mouseMove(x, y);
}

// Define keystroke events
void myKeyStroke(unsigned char key, int x, int y)
{
	theApp.keyPos(key, x, y);
}

void mySpecialKeyStroke(int key, int x, int y)
{
	theApp.specialKeyPos(key, x, y);
}

// The main function here
int main(int argc, char ** argv)
{
	// Initialize opengl & glut
	glutInit(&argc, argv);

	// Initialize display mode:
	// GLUT_DOUBLE defines double buffering
	// GLUT_RGB defines RGB color mode
	// GLUT_DEPTH defines that the display mode is 3D and depth is required
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_ACCUM);

	// Tell opengl how big your window size should be
	glutInitWindowSize(WIDTH, HEIGHT);

	// Tell opengl where on screen does the opengl window need to appear
	glutInitWindowPosition(100, 100);

	// Opengl initialization is over
	// Now tell it to create the opengl window
	int render_window = glutCreateWindow("Transformations And Lighting");

	// init() consists of all the user defined initialization functions & variables
	// example, whether the lighting is enabled, what is the background color, etc
	theApp.init();

	// tell opengl window to draw the scene that you created
	glutDisplayFunc(myDisplay);

	// specify how window reshape is handled
	glutReshapeFunc(myReshape);

	// specify the mouse click events to opengl
	glutMouseFunc(myMouseClick);

	// specify the mouse drag events
	glutMotionFunc(myMouseMove);

	// specify keyboard input events
	glutKeyboardFunc(myKeyStroke);

	// specify keyboard special key input events
	glutSpecialFunc(mySpecialKeyStroke);

	// Enter the opengl event processing loop
	// no draw events will be processed without glutMainLoop()
	// This is like saying - i have defined all opengl functions, now switch on the opengl button
	glutMainLoop();
	return 0;
}
